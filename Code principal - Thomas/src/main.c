#include "MPC5604B_M27V.h"
#include "init.h"
#include "base.h"

/*
voiture 1 : positions (min, moy, max) : (?,1264,?)
											1342 ... ?

voiture 2 : positions (min, moy, max) : (?,?,?)
											688

voiture 3 : positions (min, moy, max) : (?,?,?)
											1296 ... ?
*/

int main(void) {
	volatile int i = 0;
	volatile uint32_t resultat = 0;
  
  	initModesAndClock(); 				/* Initialize mode entries and system clock */
	initPeriClkGen();  					/* Initialize peripheral clock generation for DSPIs */
	disableWatchdog(); 					/* Disable watchdog */
	
    initPads();             			/* Initialize pads used in example */
  	initADC();              			/* Init. ADC for normal conversions but don't start yet*/
  	initCTU();              			/* Configure desired CTU event(s) */
  	initEMIOS_0();          			/* Initialize eMIOS channels as counter, SAIC, OPWM */
  	initEMIOS_0ch3();					/* Initialize eMIOS 0 channel 3 as OPWM and channel 2 as SAIC*/ 
  	
  	initEMIOS_0ch0(); 					/* Initialize eMIOS 0 channel 0 as modulus counter*/
	initEMIOS_0ch23(); 					/* Initialize eMIOS 0 channel 23 as modulus counter*/
	initEMIOS_0ch4(); 					/* Initialize eMIOS 0 channel 0 as OPWM, ch 4 as time base */
	initEMIOS_0ch6(); 					/* Initialize eMIOS 0 channel 0 as OPWM, ch 6 as time base */
	initEMIOS_0ch7(); 					/* Initialize eMIOS 0 channel 1 as OPWM, ch 7 as time base */
	
	init_LinFLEX_0_UART();
  
  
	SIU.PCR[16].R = 0x0100;				/* Potentiomètre en Input"

	/* Loop forever */
	for (;;) {
		
			
	
	/* Code pour régler le servo : alignage des roues 
		ADC.MCR.B.NSTART=1;     			// Trigger normal conversions for ADC0
		while (ADC.MSR.B.NSTART == 1) {};
		resultat = ADC.CDR[4].B.CDATA;
		resultat = resultat*2;
	//	if (resultat > 1750) resultat=1750;
	//	else if (resultat < 1250) resultat=1250;
		EMIOS_0.CH[4].CBDR.R = resultat;
		i++;
	*/
		
		//boutonLed();
		//SERVO();
	}
}