#ifndef __INIT__H__
#define __INIT_H__

#include "MPC5604B_M27V.h"
#include "constantes.h"

void init(void);

void initModesAndClock(); 				/* Initialize mode entries and system clock */
void initPeriClkGen();  				/* Initialize peripheral clock generation for DSPIs */
void disableWatchdog(); 				/* Disable watchdog */
	
void initPads();             			/* Initialize pads */
void initADC();              			/* Init. ADC for normal conversions but don't start yet*/
void initCTU();              			/* Configure desired CTU event(s) */

/* Configuration module EMIOS */
void initEMIOS_0();          			/* Initialize eMIOS_0 */
void initEMIOS_0ch3();
void initEMIOS_0ch0(); 					/* Initialize eMIOS 0 channel 0 as modulus counter*/
void initEMIOS_0ch23(); 				/* Initialize eMIOS 0 channel 23 as modulus counter*/

void initEMIOS_0ch4(); 					/* Initialize eMIOS 0 channel 4 as OPWM, Servo-moteur */

void initEMIOS_0ch6(); 					/* Initialize eMIOS 0 channel 6 as OPWM, ch 0 as time base, Motor Left */
void initEMIOS_0ch7(); 					/* Initialize eMIOS 0 channel 7 as OPWM, ch 0 as time base  Motor Right*/
void initEMIOS_0ch8();
void initEMIOS_0ch16();

void initEMIOS_0ch11();	
void init_LinFLEX_0_UART();

void init_camera();

#endif
