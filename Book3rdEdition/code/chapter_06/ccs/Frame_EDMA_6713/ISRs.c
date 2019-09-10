// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

///////////////////////////////////////////////////////////////////////
// Filename: ISRs.c
//
// Synopsis: Interrupt service routines for EDMA on 6713 DSP
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h" 
#include "math.h"
#include "frames.h"  
  
// frame buffer declarations
#define BUFFER_COUNT		1024   // buffer length in McBSP samples (L+R)
#define BUFFER_LENGTH   	BUFFER_COUNT*2 // two shorts read from McBSP each time  
#define NUM_BUFFERS     	3     // don't change this! 

#pragma DATA_SECTION (buffer, "CE0"); // allocate buffers in SDRAM 
Int16 buffer[NUM_BUFFERS][BUFFER_LENGTH];
// there are 3 buffers in use at all times, one being filled from the McBSP,
// one being operated on, and one being emptied to the McBSP
// ready_index --> buffer ready for processing
volatile Int16 buffer_ready = 0, over_run = 0, ready_index = 0;

void EDMA_Init()
////////////////////////////////////////////////////////////////////////
// Purpose:   Configure EDMA controller to perform all McBSP servicing. 
//            EDMA is setup so buffer[2] is outbound to McBSP, buffer[0] is 
//            available for processing, and buffer[1] is being loaded.
//            Conditional statement ensure that the correct EDMA events are
//            used based on the McBSP that is being used.
//            Both the EDMA transmit and receive events are set to automatically
//            reload upon completion, cycling through the 3 buffers. 
//            The EDMA completion interrupt occurs when a buffer has been filled
//            by the EDMA from the McBSP.
//            The EDMA interrupt service routine updates the ready buffer index, 
//            and sets the buffer ready flag which is being polled by the main 
//            program loop
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	EDMA_params* param;

	// McBSP tx event params
	param = (EDMA_params*)(EVENTE_PARAMS);
	param->options = 0x211E0002;
	param->source = (Uint32)(&buffer[2][0]);
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = 0x34000000;
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTN_PARAMS & 0xFFFF);
	
	// set up first tx link param
	param = (EDMA_params*)EVENTN_PARAMS;
	param->options = 0x211E0002;
	param->source = (Uint32)(&buffer[0][0]);
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = 0x34000000;
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTO_PARAMS & 0xFFFF);
	
	// set up second tx link param
	param = (EDMA_params*)EVENTO_PARAMS;
	param->options = 0x211E0002;
	param->source = (Uint32)(&buffer[1][0]);
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = 0x34000000;
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTP_PARAMS & 0xFFFF);
	
	// set up third tx link param
	param = (EDMA_params*)EVENTP_PARAMS;
	param->options = 0x211E0002;
	param->source = (Uint32)(&buffer[2][0]);
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = 0x34000000;
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTN_PARAMS & 0xFFFF);
	
	
	// McBSP rx event params
	param = (EDMA_params*)(EVENTF_PARAMS);
	param->options = 0x203F0002;
	param->source = 0x34000000;
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = (Uint32)(&buffer[1][0]);
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTQ_PARAMS & 0xFFFF);
	
	// set up first rx link param
	param = (EDMA_params*)EVENTQ_PARAMS;
	param->options = 0x203F0002;
	param->source = 0x34000000;
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = (Uint32)(&buffer[2][0]);
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTR_PARAMS & 0xFFFF);
	
	// set up second rx link param
	param = (EDMA_params*)EVENTR_PARAMS;
	param->options = 0x203F0002;
	param->source = 0x34000000;
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = (Uint32)(&buffer[0][0]);
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTS_PARAMS & 0xFFFF);
	
	// set up third rx link param
	param = (EDMA_params*)EVENTS_PARAMS;
	param->options = 0x203F0002;
	param->source = 0x34000000;
	param->count = (0 << 16) + (BUFFER_COUNT);
	param->dest = (Uint32)(&buffer[1][0]);
	param->reload_link = (BUFFER_COUNT << 16) + (EVENTQ_PARAMS & 0xFFFF);
	
	*(volatile Uint32 *)ECR = 0xf000;	// clear all McBSP events
	*(volatile Uint32 *)EER = 0xC000;
	*(volatile Uint32 *)CIER = 0x8000; // interrupt on rx reload only
}

void ZeroBuffers() 
////////////////////////////////////////////////////////////////////////
// Purpose:   Sets all buffer locations to 0 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
    Int32 i = BUFFER_COUNT * NUM_BUFFERS;
    Int32 *p = (Int32 *)buffer;

    while(i--)
        *p++ = 0;
}

void ProcessBuffer()
///////////////////////////////////////////////////////////////////////
// Purpose:   Processes the data in buffer[ready_index] and stores
//  		  the results back into the buffer 
//            Data is packed into the buffer, alternating right/left
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{   
	Int16 *pBuf = buffer[ready_index];
	static float Left[BUFFER_COUNT], Right[BUFFER_COUNT];
	float *pL = Left, *pR = Right;
	Int32 i;
	float temp;     

	WriteDigitalOutputs(0); // set digital outputs low - for time measurement

	for(i = 0;i < BUFFER_COUNT;i++) { // extract data to float buffers
		*pR++ = *pBuf++;
		*pL++ = *pBuf++;
	}

	pL = Left; // reinitialize pointers
	pR = Right;        
/* gain */
	for(i=0;i < BUFFER_COUNT;i++){ 
		*pL++ *= 0.5;
		*pR++ *= 2.0;
	}  
  
/* zero out left channel 
	for(i=0;i < BUFFER_COUNT;i++){ 
		*pL = 0.0;
		pL++;
	}  */

 
/* zero out right channel 
	for(i=0;i < BUFFER_COUNT;i++){ 
		*pR = 0.0;
		pR++;
	}    */


/* reverb on right channel  
	for(i=0;i < BUFFER_COUNT-4;i++){ 
		*pR = *pR + (0.9 * pR[2]) + (0.45 * pR[4]);
		pR++;
	}              
*/ 
  
/* addition and subtraction 
	for(i=0;i < BUFFER_COUNT;i++){ 
		temp = *pL;        
		*pL = temp + *pR; // left = L+R
		*pR = temp - *pR; // right = L-R 
		pL++;
		pR++;
	}   */
             
                   
/* add a sinusoid    
	for(i=0;i < BUFFER_COUNT;i++){ 
		*pL = *pL + 1024*sinf(0.5*i);
		pL++;
	}    
*/                   
                   
/* AM modulation 
	for(i=0;i < BUFFER_COUNT;i++){
		*pR = *pL * *pR * (1/32768.0); // right = L*R 
		*pL = *pL + *pR; // left = L*(1+R) 
		pL++;
		pR++;
	} 
*/


 	pBuf = buffer[ready_index];
	pL = Left;
	pR = Right;

//	for(i = 0;i < BUFFER_COUNT;i++) { // pack into buffer without bounding
//		*pBuf++ = *pR++;
//		*pBuf++ = *pL++;
//	}

	for(i = 0;i < BUFFER_COUNT;i++) { // pack into buffer after bounding
		*pBuf++ = _spint(*pR++ * 65536) >> 16;
		*pBuf++ = _spint(*pL++ * 65536) >> 16;
	}
	
 	pBuf = buffer[ready_index];

	WriteDigitalOutputs(1); // set digital output bit 0 high - for time measurement
	buffer_ready = 0; // signal we are done
}

///////////////////////////////////////////////////////////////////////
// Purpose:   Access function for buffer ready flag 
//
// Input:     None
//
// Returns:   Non-zero when a buffer is ready for processing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
int IsBufferReady()
{
	return buffer_ready;
}

///////////////////////////////////////////////////////////////////////
// Purpose:   Access function for buffer overrun flag 
//
// Input:     None
//
// Returns:   Non-zero if a buffer overrun has occurred
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
int IsOverRun()
{
	return over_run;
}
 
interrupt void EDMA_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   EDMA interrupt service routine.  Invoked on every buffer 
//            completion 
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     Nothing
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{
	*(volatile Uint32 *)CIPR = 0xf000; // clear all McBSP events
	if(++ready_index >= NUM_BUFFERS) // update buffer index
		ready_index = 0;
	if(buffer_ready == 1) // set a flag if buffer isn't processed in time 
		over_run = 1;
	buffer_ready = 1; // mark buffer as ready for processing
}

