#include "MPC5604B_M07N.h"
#include "InitCarte.h"


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
	SIU.PCR[16].R = 0x0200;				/*Motor pins set as output pins */
	SIU.PCR[17].R = 0x0200;				/*Motor pins set as output pins */
	SIU.PCR[16].R += 0x0100;			/* Potentiometer pin set as an input */
	SIU.PCR[20].R = 0x2000;          	/* MPC56xxB: Initialize PB[4] as ANP0 */
	SIU.PCR[21].R = 0x2000;          	/* MPC56xxB: Initialize PB[5] as ANP1 */
	SIU.PCR[22].R = 0x2000;          	/* MPC56xxB: Initialize PB[6] as ANP2 */
	SIU.PCR[48].R = 0x2000;          	/* MPC56xxB: Initialize PD[0] as ANP4 */
	SIU.PCR[64].R = 0x0100;				/* Initialise PE[0] (S1) en entrée */
	SIU.PCR[65].R = 0x0100;				/* Initialise PE[1] (S2) en entrée */
	SIU.PCR[68].R = 0x0200;				/* Initialise la LED 1 en sortie */
	SIU.PCR[69].R = 0x0200;				/* Initialise la LED 2 en sortie */
	SIU.PCR[70].R = 0x0200;				/* Initialise la LED 3 en sortie */
	SIU.PCR[71].R = 0x0200;				/* Initialise la LED 4 en sortie */
	
								   /* Capteurs infrarouges */
	SIU.PCR[34].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[2] */      
	SIU.PCR[35].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[3] */
	SIU.PCR[36].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[4] */
	SIU.PCR[37].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[5] */
	SIU.PCR[38].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[6] */
	SIU.PCR[39].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[7] */
	SIU.PCR[40].R = 0x0200;        /* Pin set as an  OUTPUT  -> PC[8] */
	
	SIU.PCR[11].R = 0x0500;        /* Capteur vitesse, PA[11] set as an INPUT */
}

void initADC(void) {
	//ADC.MCR.R = 0x20020000;         	/* Initialize ADC scan mode*/
	ADC.MCR.R = 0x00000000;         	/* Initialize ADC one shot mode*/
	ADC.NCMR[0].R = 0x00000017;      	/* Select ANP0:2 and 4 inputs for normal conversion */
	ADC.CTR[0].R = 0x00008606;       	/* Conversion times for 32MHz ADClock */
	ADC.CTR[0].B.INPSAMP=25;
}

void initCTU(void) {
  	CTU.EVTCFGR[2].R = 0x00008000;  	 /* Config event on eMIOS Ch 2 to trig ANP[0] */
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
	
	/* configure baudrate 19200 */
	/* assuming 64 MHz peripheral set 1 clock */		
	LINFLEX_0.LINFBRR.R = 5;
	LINFLEX_0.LINIBRR.R = 208;
		
	/* enter NORMAL mode */
	LINFLEX_0.LINCR1.R = 0x0080; /* INIT=0 */	
}

void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
 // asm(" wrteei 1");	    	   /* Enable external interrupts */
}
