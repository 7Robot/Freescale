#include "init.h"

volatile uint32_t dly, lly;

void initModesAndClock(void) {
	ME.MER.R = 0x0000001D;          	/* Enable DRUN, RUN0, SAFE, RESET modes */
	                              		/* Initialize PLL before turning it on: */
										/* Use 1 of the next 2 lines depending on crystal frequency: */
	CGM.FMPLL_CR.R = 0x02400100;    	/* 8 MHz xtal: Set PLL0 to 64 MHz */   
	/*CGM.FMPLL_R = 0x12400100;*/     	/* 40 MHz xtal: Set PLL0 to 64 MHz */   
	ME.RUN[0].R = 0x001F0074;       	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	
	//ME.RUNPC[0].R = 0x00000010; 	  	/* Peri. Cfg. 0 settings: only run in RUN0 mode */
   										/* Use the next lines as needed for MPC56xxB/S: */  	    	
	//ME.PCTL[48].R = 0x0000;         	/* MPC56xxB LINFlex0: select ME.RUNPC[0] */	
	//ME.PCTL[68].R = 0x0000;         	/* MPC56xxB/S SIUL:  select ME.RUNPC[0] */	
	
	ME.RUNPC[1].R = 0x00000010;     	/* Peri. Cfg. 1 settings: only run in RUN0 mode */
	ME.PCTL[32].R = 0x01;       		/* MPC56xxB ADC 0: select ME.RUNPC[1] */
  	ME.PCTL[57].R = 0x01;       		/* MPC56xxB CTUL: select ME.RUNPC[1] */
  	ME.PCTL[48].R = 0x01;           	/* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] */
	ME.PCTL[68].R = 0x01;           	/* MPC56xxB/S SIUL:  select ME.RUNPC[1] */
	ME.PCTL[72].R = 0x01;           	/* MPC56xxB/S EMIOS 0:  select ME.RUNPC[1] */
	                              		/* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0;         	/* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         	/* Enter RUN0 Mode & Inverted Key */  
	while (ME.GS.B.S_MTRANS) {}     	/* Wait for mode transition to complete */    
	                          			/* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {}/* Verify RUN0 is the current mode */
	
	//while (ME.IS.B.I_MTC != 1) {}    /* Wait for mode transition to complete */    
	//ME.IS.R = 0x00000001;           /* Clear Transition flag */ 
}


void initPeriClkGen(void) {
	CGM.SC_DC[0].R = 0x80; 				/* MPC56xxB/S: Enable peri set 1 sysclk divided by 1 */
  	CGM.SC_DC[2].R = 0x80;         		/* MPC56xxB: Enable peri set 3 sysclk divided by 1*/
}

void disableWatchdog(void) {
	SWT.SR.R = 0x0000c520;     			/* Write keys to clear soft lock bit */
  	SWT.SR.R = 0x0000d928; 
  	SWT.CR.R = 0x8000010A;     			/* Clear watchdog enable (WEN) */
}

void initPads (void) {
	SIU.PCR[2].R = 0x0503;           	/* MPC56xxB: Initialize PA[2] as eMIOS[2] input */
	SIU.PCR[3].R = 0x0600;           	/* MPC56xxB: Initialize PA[3] as eMIOS[3] output */
	SIU.PCR[20].R = 0x2000;          	/* MPC56xxB: Initialize PB[4] as ANP0 */
	SIU.PCR[21].R = 0x2000;          	/* MPC56xxB: Initialize PB[5] as ANP1 */
	SIU.PCR[22].R = 0x2000;          	/* MPC56xxB: Initialize PB[6] as ANP2 */
	SIU.PCR[48].R = 0x2000;          	/* MPC56xxB: Initialize PD[0] as ANP4 */
}

void initADC(void) {
	//ADC.MCR.R = 0x20020000;         	/* Initialize ADC scan mode*/
	ADC.MCR.R = 0x00000000;         	/* Initialize ADC one shot mode*/
	ADC.NCMR[0].R = 0x00000017;      	/* Select ANP1:2 inputs for normal conversion */
	ADC.CTR[0].R = 0x00008606;       	/* Conversion times for 32MHz ADClock */
}

void initCTU(void) {
  	CTU.EVTCFGR[2].R = 0x00008000;  	 /* Config event on eMIOS Ch 2 to trig ANP[0] */
}

void initEMIOS_0(void) {  
	EMIOS_0.MCR.B.GPRE= 63;   			/* Divide 64 MHz sysclk by 63+1 = 64 for 1MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;			/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;  			/* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    			/* Enable stopping channels when in debug mode */
}

void initEMIOS_0ch3(void) {
	EMIOS_0.CH[3].CADR.R = 250;      	/* Ch 3: Match "A" is 250 */
	EMIOS_0.CH[3].CBDR.R = 500;      	/* Ch 3: Match "B" is 500 */
	EMIOS_0.CH[3].CCR.R= 0x000000E0; 	/* Ch 3: Mode is OPWMB, time base = ch 23 */
	EMIOS_0.CH[2].CCR.R= 0x01020082; 	/* Ch 2: Mode is SAIC, time base = ch 23 */
}

void initEMIOS_0ch0(void) {        		/* EMIOS 0 CH 0: Modulus Up Counter */
	EMIOS_0.CH[0].CADR.R = 19999;   	/* Period will be 19999+1 = 20000 clocks (20 msec)*/
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[0].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[0].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
}

void initEMIOS_0ch23(void) {        	/* EMIOS 0 CH 23: Modulus Up Counter */
	EMIOS_0.CH[23].CADR.R = 999;      	/* Period will be 999+1 = 1000 clocks (1 msec)*/
	EMIOS_0.CH[23].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[23].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[23].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[23].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[23].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
}

void initEMIOS_0ch4(void) {        		/* EMIOS 0 CH 4: Output Pulse Width Modulation*/
	EMIOS_0.CH[4].CADR.R = 0;     		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[4].CBDR.R = 1500;      	/* Trailing edge when channel counter bus=1400 Middle, 1650 Right Max, 1150 Left Max*/
	EMIOS_0.CH[4].CCR.B.BSL = 0x01;  	/* Use counter bus B */
	EMIOS_0.CH[4].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[28].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad */
}

void initEMIOS_0ch6(void) {        		/* EMIOS 0 CH 6: Output Pulse Width Modulation*/
	EMIOS_0.CH[6].CADR.R = 500;     	/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[6].CBDR.R = 850;     	/* Trailing edge when channel counter bus=500*/
	EMIOS_0.CH[6].CCR.B.BSL = 0x0;  	/* Use counter bus A (default) */
	EMIOS_0.CH[6].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[6].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[30].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad */
}

void initEMIOS_0ch7(void) {        		/* EMIOS 0 CH 7: Output Pulse Width Modulation*/
	EMIOS_0.CH[7].CADR.R = 0;    		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[7].CBDR.R = 350;     	/* Trailing edge when channel's counter bus=999*/
	EMIOS_0.CH[7].CCR.B.BSL = 0x0; 		/* Use counter bus A (default) */
	EMIOS_0.CH[7].CCR.B.EDPOL = 1; 		/* Polarity-leading edge sets output*/
	EMIOS_0.CH[7].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[31].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 7 to pad */
}

void Delay(void){
  for(dly=0;dly<250;dly++){}
}

void Delaylong(void){
  for(dly=0;dly<20000;dly++){}
}

void Delaylonglong(void){
  for(lly=0;lly<1;lly++) Delaylong();
}

void Delaycamera(void){
  for(lly=0;lly<10;lly++) Delaylong();
}

void Delaywait(void){
  for(lly=0;lly<500;lly++) Delaylong();
}

void Delayled(void){
  for(lly=0;lly<500;lly++) Delaylong();
}

void init_LinFLEX_0_UART (void) 
{	

	/* enter INIT mode */
	LINFLEX_0.LINCR1.R = 0x0081; 		/* SLEEP=0, INIT=1 */
	
	/* wait for the INIT mode */
	while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}
		
	/* configure pads */
	SIU.PCR[18].R = 0x0604;     		/* Configure pad PB2 for AF1 func: LIN0TX */
	SIU.PCR[19].R = 0x0100;     		/* Configure pad PB3 for LIN0RX */	
	
	/* configure for UART mode */
	LINFLEX_0.UARTCR.R = 0x0001; 		/* set the UART bit first to be able to write the other bits */
	LINFLEX_0.UARTCR.R = 0x0033; 		/* 8bit data, no parity, Tx and Rx enabled, UART mode */
								 		/* Transmit buffer size = 1 (TDFL = 0 */
								 		/* Receive buffer size = 1 (RDFL = 0) */
	
	/* configure baudrate 115200 */
	/* assuming 64 MHz peripheral set 1 clock */		
	LINFLEX_0.LINFBRR.R = 12;
	LINFLEX_0.LINIBRR.R = 34;
		
	/* enter NORMAL mode */
	LINFLEX_0.LINCR1.R = 0x0080; /* INIT=0 */	
}
