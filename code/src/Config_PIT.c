#include "Config_PIT.h"
#include "extern_globals.h"

/***************************  Interruptions sur timer ***********************/
void Init_PIT(int8_t Channel, uint32_t Clock_Freq, float ExpectedTimeBase)  
{
	PIT.PITMCR.B.MDIS = 0;          	  								// Enable Clk for PIT.CH[Channel] 
	PIT.PITMCR.B.FRZ = 1;
																		// Timer continue running in debug mode 	
	PIT.CH[Channel].LDVAL.R = (uint32_t) (ExpectedTimeBase*Clock_Freq-1);// Fix the load value for CH[0]
}

void PIT_EnableINTC(int8_t Channel)										// Enable PIT.CH[Channel] interruptions
{
	PIT.CH[Channel].TCTRL.B.TIE = 1;
}

void PIT_DisableINTC(int8_t Channel)									// Disable PIT.CH[Channel] interruptions
{
	PIT.CH[Channel].TCTRL.B.TIE = 0;
}

void PIT_Enable_Channel(int8_t Channel)									// Enable PIT.CH[Channel]
{
	PIT.CH[Channel].TCTRL.B.TEN = 1;
}

void PIT_Disable_Channel(int8_t Channel)								// Disable PIT.CH[Channel] 
{
	PIT.CH[Channel].TCTRL.B.TEN = 0;
}

int8_t PIT_GetFlag(int8_t Channel)
{
    return PIT.CH[Channel].TFLG.B.TIF;
}
void PIT_ClearFlag(int8_t Channel)
{
    PIT.CH[Channel].TFLG.R=1;
}

void Boucle_principale(void)
{
	 PIT.CH[0].TFLG.R=1;
	 main_fin_boucle = 1;
}

void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
 // asm(" wrteei 1");	    	   /* Enable external interrupts */
}