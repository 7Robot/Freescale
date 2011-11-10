#include "MPC5604B_M27V.h"

//#ifndef INIT.H

//#define INIT.H

void initModesAndClock(); 				/* Initialize mode entries and system clock */
void initPeriClkGen();  				/* Initialize peripheral clock generation for DSPIs */
void disableWatchdog(); 				/* Disable watchdog */
	
void initPads();             			/* Initialize pads used in example */
void initADC();              			/* Init. ADC for normal conversions but don't start yet*/
void initCTU();              			/* Configure desired CTU event(s) */
void initEMIOS_0();          			/* Initialize eMIOS channels as counter, SAIC, OPWM */
void initEMIOS_0ch3();					/* Initialize eMIOS 0 channel 3 as OPWM and channel 2 as SAIC*/ 
  	
void initEMIOS_0ch0(); 					/* Initialize eMIOS 0 channel 0 as modulus counter*/
void initEMIOS_0ch23(); 					/* Initialize eMIOS 0 channel 23 as modulus counter*/
void initEMIOS_0ch4(); 					/* Initialize eMIOS 0 channel 0 as OPWM, ch 4 as time base */
void initEMIOS_0ch6(); 					/* Initialize eMIOS 0 channel 0 as OPWM, ch 6 as time base */
void initEMIOS_0ch7(); 					/* Initialize eMIOS 0 channel 1 as OPWM, ch 7 as time base */
	
void init_LinFLEX_0_UART();

void Delay(void);
void Delaylong(void);
void Delaylonglong(void);
void Delaycamera(void);
void Delaywait(void);
void Delayled(void);

//#endif