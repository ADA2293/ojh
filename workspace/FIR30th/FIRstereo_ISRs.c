// Welch, Wright, & Morrow, 
// Real-time Digital Signal Processing, 2017

///////////////////////////////////////////////////////////////////////
// Filename: FIRstereo_ISRs.c
//
// Synopsis: Interrupt service routine for codec data transmit/receive
//
///////////////////////////////////////////////////////////////////////

#include "DSP_Config.h" 
#include "FIR30th.h"	// load the filter coefficients, B[n] ... extern
  
// Data is received as 2 16-bit words (left/right) packed into one
// 32-bit word.  The union allows the data to be accessed as a single 
// entity when transferring to and from the serial port, but still be 
// able to manipulate the left and right channels independently.

#define LEFT  0
#define RIGHT 1

volatile union {
	Uint32 UINT;
	Int16 Channel[2];
} CodecDataIn, CodecDataOut;


/* add any global variables here */

float xLeft[B_LENGTH+1], xRight[B_LENGTH+1], yLeft, yRight;
Int32 i;


interrupt void Codec_ISR()
///////////////////////////////////////////////////////////////////////
// Purpose:   Codec interface interrupt service routine  
//
// Input:     None
//
// Returns:   Nothing
//
// Calls:     CheckForOverrun, ReadCodecData, WriteCodecData
//
// Notes:     None
///////////////////////////////////////////////////////////////////////
{                    
	/* add any local variables here */


 	if(CheckForOverrun())					// overrun error occurred (i.e. halted DSP)
		return;								// so serial port is reset to recover

  	CodecDataIn.UINT = ReadCodecData();		// get input data samples
	
	/* I added my FIR filter routine here */ 
	xLeft[0]  = CodecDataIn.Channel[LEFT];	// current LEFT input value
	xRight[0] = CodecDataIn.Channel[RIGHT];	// current RIGHT input value
	yLeft  = 0;				// initialize the LEFT output value
	yRight = 0;				// initialize the RIGHT output value

	for (i = 0; i <= B_LENGTH; i++) {
		yLeft += xLeft[B_LENGTH - i]*B[i];	// perform the LEFT dot-product
		yRight = xRight[i];	// perform the RIGHT dot-product
	}
	
	// Shift the signal further to 1 bit and leave space for the new signal. [OK]
	for (i = B_LENGTH; i > 0; i--) {
		xLeft[ i] = xLeft[ i-1];        // setup xLeft[] for the next input
		xRight[i] = xRight[i-1];     	// setup xRight[] for the next input
	}
	
	CodecDataOut.Channel[LEFT]  = yLeft;	// setup the LEFT value	
	CodecDataOut.Channel[RIGHT] = CodecDataIn.Channel[RIGHT];	// setup the RIGHT value	
	/* end of my routine */	


	WriteCodecData(CodecDataOut.UINT);		// send output data to  port
}

