// fonctions d'initialisation

#include "init.h"
#include "Config_PIT.h"
#include "moteur.h"
#include "extern_globals.h"
#include "liaison_serie.h"


void init()
{

    /************************ Initialisation de la carte ****************************/

  	initModesAndClock(); 				/* Initialize mode entries and system clock */
	initPeriClkGen();  					/* Initialize peripheral clock generation for DSPIs */
	disableWatchdog(); 					/* Disable watchdog */
	
    initPads();             			/* Initialize pads */
  	initADC();              			/* Init. ADC for normal conversions but don't start yet*/
  	initCTU();              			/* Configure desired CTU event(s) */
  	
  	initEMIOS_0();          			/* Initialize eMIOS_0 */
  	initEMIOS_0ch3();					/* Initialize eMIOS 0 channel 3 as OPWM and channel 2 as SAIC*/ 
  
  
  	initEMIOS_0ch0(); 					/* Initialize eMIOS 0 Time bases */
	initEMIOS_0ch8();
	initEMIOS_0ch16();
	initEMIOS_0ch23(); 					
    //initEMIOS_0ch3();                   /* Initialize eMIOS 0 channel 3 as OPWM, servo moteur sans passer par la carte de puissance */
	initEMIOS_0ch4(); 					/* Initialize eMIOS 0 channel 4 as OPWM, Servo moteur  */
	initEMIOS_0ch6(); 					/* Initialize eMIOS 0 channel 6 as OPWM, Moteur Gauche */
	initEMIOS_0ch7(); 					/* Initialize eMIOS 0 channel 7 as OPWM, Motuer Droit  */
	initEMIOS_0ch11(); 					/* Initialize eMIOS 0 channel 7 as OPWM, Motuer Droit  */
	
	init_LinFLEX_0_UART();              /* Liaison Série */
	
    /**********************  Activation des H-Bridge *******************************/  
	
	SIU.PCR[16].R = 0x0200;				//Motor pins set as output pins
	SIU.PCR[17].R = 0x0200;		
        
   
    SIU.PGPDO[2].R |= 0x0f000000; // Extinction des leds
    
    init_camera();

    Init_PIT(0,64000000, main_timer_period); // Boucle principale
    INTC_InitINTCInterrupts();
    INTC_InstallINTCInterruptHandler(Compteur_Moteur,146,3);
    INTC_InstallINTCInterruptHandler(Boucle_principale,59,1);
    INTC_InstallINTCInterruptHandler(Data_uart,79,1);
    enableIrq();
    PIT_EnableINTC(0);
    PIT_Enable_Channel(0);

}

/*************************  Initialisation de la carte    ************************/ 

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
	ME.PCTL[92].R = 0x01;
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
		CGM.SC_DC[0].R = 0x80; 			                        /* Initialize eMIOS 0 channel 4 as OPWM, Servo moteur  */
		CGM.SC_DC[2].R = 0x80;            
}

/*************************  Initialisation de la carte    ************************/ 

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
	SIU.PCR[42].R = 0x0200;				/* Initialise la pin de contrôle du freinage en sortie -> PC[10]*/
	SIU.PCR[57].R = 0x2000;          	/* MPC56xxB: Initialize PD[9] as ANP13 -> potentiomètre */
	SIU.PCR[58].R = 0x2000;          	/* MPC56xxB: Initialize PD[10] as ANP14 -> Camera */
	SIU.PCR[64].R = 0x0100;				/* Initialise PE[0] (S1) en entrÃ©e */
	SIU.PCR[65].R = 0x0100;				/* Initialise PE[1] (S2) en entrÃ©e */
	SIU.PCR[66].R = 0x0100;				/* Initialise PE[2] (S2) en entrÃ©e */
	SIU.PCR[67].R = 0x0100;				/* Initialise PE[3] (S2) en entrÃ©e */
	SIU.PCR[68].R = 0x0200;				/* Initialise la LED 1 en sortie */
	SIU.PCR[69].R = 0x0200;				/* Initialise la LED 2 en sortie */
	SIU.PCR[70].R = 0x0200;				/* Initialise la LED 3 en sortie */
	SIU.PCR[71].R = 0x0200;				/* Initialise la LED 4 en sortie */
}

/*************************** Conversion Analogique-NumÃ©rique ***************************/
void initADC(void) {
	//ADC.MCR.R = 0x20020000;         	/* Initialize ADC scan mode*/
	ADC.MCR.R = 0x00000000;         	/* Initialize ADC one shot mode*/
	ADC.NCMR[0].R = 0x00006017;      	/* Select ANP0:2 and 4 inputs for normal conversion */
	ADC.CTR[0].R = 0x00008606;       	/* Conversion times for 32MHz ADClock */
	ADC.CTR[0].B.INPSAMP=25;
}


/*************************  Trigger ***************************************************/
void initCTU(void) {
  	CTU.EVTCFGR[2].R = 0x00008000;  	 /* Config event on eMIOS Ch 2 to trig ANP[0] */
}



/***************** Initialisation de l'EMIOS *****************************/

/*******************     Time base  **************************************/
void initEMIOS_0(void) {  
	EMIOS_0.MCR.B.GPRE= 63;   			/* Divide 64 MHz sysclk by 63+1 = 64 for 1MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;			/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;  			/* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    			/* Enable stopping channels when in debug mode */
}

void initEMIOS_0ch3(void) { // servo
	EMIOS_0.CH[3].CADR.R = 0;     		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[3].CBDR.R = POS_MILIEU_SERVO;  /* Trailing edge when channel counter bus=1400 Middle, 1650 Right Max, 1150 Left Max*/
	EMIOS_0.CH[3].CCR.B.BSL = 0x01;  	/* Use counter bus B -> Time base channel 0*/
	EMIOS_0.CH[3].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[3].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[3].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 3 to pad PA[3]*/
	
	// Ancienne version inutile
	//EMIOS_0.CH[3].CADR.R = 250;      	/* Ch 3: Match "A" is 250 */
	//EMIOS_0.CH[3].CBDR.R = 500;      	/* Ch 3: Match "B" is 500 */
	//EMIOS_0.CH[3].CCR.R= 0x000000E0; 	/* Ch 3: Mode is OPWMB, time base = ch 23 */
	//EMIOS_0.CH[2].CCR.R= 0x01020082; 	/* Ch 2: Mode is SAIC, time base = ch 23 */
}


void initEMIOS_0ch0(void) {        		/* EMIOS 0 CH 0: Modulus Up Counter */
	EMIOS_0.CH[0].CADR.R = 19999;   	/* Period will be 19999+1 = 20000 clocks (20 msec)*/
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[0].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[0].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
}

void initEMIOS_0ch8(void) {        		/* EMIOS 0 CH 0: Modulus Up Counter */
	EMIOS_0.CH[8].CADR.R = 0xFFFFFE;   	/* Period will be 19999+1 = 20000 clocks (20 msec)*/
	EMIOS_0.CH[8].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;   	/* 0b11 -> Use internal counter */
	EMIOS_0.CH[8].CCR.B.UCPRE = 3;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[8].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
	//EMIOS_0.CH[11].CCR.B.DMA = 0; // generate interrupt
	//EMIOS_0.CH[11].CCR.B.FEN = 1;
}

void initEMIOS_0ch16(void)
{
	EMIOS_0.CH[16].CADR.R = 1570;   	/* Period will be 19999+1 = 20000 clocks (20 msec)*/
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[16].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[16].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
}

void initEMIOS_0ch23(void) {        	/* EMIOS 0 CH 23: Modulus Up Counter */
	EMIOS_0.CH[23].CADR.R = 999;      	/* Period will be 999+1 = 1000 clocks (1 msec)*/
	EMIOS_0.CH[23].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[23].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[23].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[23].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[23].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
}

/**************************** Canaux *************************************/

void initEMIOS_0ch4(void) {        		/* EMIOS 0 CH 4: Servo-moteur */
	EMIOS_0.CH[4].CADR.R = 0;     		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[4].CBDR.R = POS_MILIEU_SERVO;  /* Trailing edge when channel counter bus=1400 Middle, 1650 Right Max, 1150 Left Max*/
	EMIOS_0.CH[4].CCR.B.BSL = 0x01;  	/* Use counter bus B -> Time base channel 0*/
	EMIOS_0.CH[4].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[28].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad PB[12]*/
}

void initEMIOS_0ch6(void) {        		/* EMIOS 0 CH 6: Motor Left*/
	EMIOS_0.CH[6].CADR.R = 0;     	/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[6].CBDR.R = 250;//785;   /* Trailing edge when channel counter bus=500*/
	EMIOS_0.CH[6].CCR.B.BSL = 0x0;  	/* Use counter bus A -> Time base channel 23 */
	EMIOS_0.CH[6].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[6].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[30].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad PB[13]*/
}

void initEMIOS_0ch7(void) {        		/* EMIOS 0 CH 7: Motor Right*/
	EMIOS_0.CH[7].CADR.R = 500;    		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[7].CBDR.R = 750;//285;   /* Trailing edge when channel's counter bus=999*/
	EMIOS_0.CH[7].CCR.B.BSL = 0x0; 		/* Use counter bus A -> Time base channel 23 */
	EMIOS_0.CH[7].CCR.B.EDPOL = 1; 		/* Polarity-leading edge sets output*/
	EMIOS_0.CH[7].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[31].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 7 to pad PB[14]*/
}

void initEMIOS_0ch11(void) // capteur de vitesse
{
	EMIOS_0.CH[11].CCR.B.BSL = 0x01;				/* Use counter bus C, paramétré par le canal 8 */
	EMIOS_0.CH[11].CCR.B.EDPOL = 1;					/* trigger quand on passe de zéro à un */
	EMIOS_0.CH[11].CCR.B.EDSEL = 0;					/* capturer que quand on passe de zéro à un (défini par EDPOL) */
	EMIOS_0.CH[11].CCR.B.FCK = 1;	  	     		/* Input filter bit clock reference is the system clock*/
    EMIOS_0.CH[11].CCR.B.IF = 1;				    /* Input filter bit of 2 clock cycles, p684 (éliminer les filtres) */ 
	EMIOS_0.CH[11].CCR.B.MODE = 0b0000101;  		/* Mode is IPM */
	//EMIOS_0.CH[11].CCR.B.FREN = 1;	    		/* Freeze channel counting when in debug mode */
  	//EMIOS_0.CH[11].CCR.B.IF = 1;
	EMIOS_0.CH[11].CCR.B.DMA = 0;					 // generate interrupt
	EMIOS_0.CH[11].CCR.B.FEN = 1;                   /* 0 -> pas d'interruption, pas de trigger */
	SIU.PCR[11].R = 0x0500;                         /* PA[11] initialisée en entrée */ 
}


/*****************************  Liaison série **************************************/

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
	
	LINFLEX_0.LINIER.R = 0x0004; // Interruption sur donnée reçue
}

/**************************** Camera *****************************************/

void init_camera (void)
{
        SIU.PCR[27].R = 0x0200;                         /* Program the Sensor read start pin as output*/
        SIU.PCR[29].R = 0x0200;                         /* Program the Sensor Clock pin as output*/
}
