* Welch, Wright, & Morrow, 
* Real-time Digital Signal Processing, 2017

*//////////////////////////////////////////////////////////////////////
*/ Filename: vectors_EDMA.asm
*/
*/ Synopsis: Interrupt vector table for EDMA apps
*/
*//////////////////////////////////////////////////////////////////////
    .ref    _c_int00
    .ref	_EDMA_ISR       
  
    .sect   "vectors"
RESET_RST:
    
    MVKL .S2 _c_int00, B0
    MVKH .S2 _c_int00, B0
    B    .S2 B0
	NOP
	NOP
	NOP
	NOP
    NOP
NMI_RST:    
    NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

RESV1:
    NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

RESV2:
    NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT4:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT5:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
                   
INT6: NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT7:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT8:   b _EDMA_ISR
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT9:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT10:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT11:  NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT12:  NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT13:   NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT14:  NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

INT15:  NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
 

