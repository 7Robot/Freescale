#include "MPC5604B_M07N.h"

/* *********************************************************************************************** */
/*									Freescale cup 2013											   */
/*								      7Robot ENSEEIHT											   */
/*									Carte TRK-MCB5604B										       */
/*									   Liaison XBEE										           */
/* *********************************************************************************************** */

int main(void) {
  volatile int i = 0;
  
  	initModesAndClock(); 				/* Initialize mode entries and system clock */
	initPeriClkGen();  					/* Initialize peripheral clock generation for DSPIs */
	disableWatchdog(); 					/* Disable watchdog */
	
    initPads();             			/* Initialize pads used in example */
  	initADC();              			/* Init. ADC for normal conversions but don't start yet*/
  	initCTU();              			/* Configure desired CTU event(s) */
	init_LinFLEX_0_UART();              /* Init. UART transmission/Reception */

  	INTC_InitINTCInterrupts(); // Init interrupts
	INTC_InstallINTCInterruptHandler(Data_UART(), 146, 1);
   	enableIrq();
  

  /* Loop forever */
  for (;;) {
   
  		for(i=0;i<1000;i++)
		{
			TransmitData("Test");	
		}
	}
}



