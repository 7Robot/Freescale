// fonctions d'initialisation

#include "init.h"
#include "MPC5604B_M27V.h"
#include "constantes.h"
#include "extern_globals.h"


extern uint32_t __IVPR_VALUE; // Interrupt Vector Prefix value from link file

void init()
{

    // ************************ Initialisation de la carte ****************************

  	initModesAndClock(); 				// Initialize mode entries and system clock   
	initPeriClkGen();  					// Initialize peripheral clock generation for DSPIs   
	disableWatchdog(); 					// Disable watchdog   
	
    initPads();             			// Initialize pads   
  	initADC();              			// Init. ADC for normal conversions but don't start yet  
  	initCTU();              			// Configure desired CTU event(s)   
  	
  	initEMIOS_0();          			// Initialize eMIOS_0
  	initEMIOS_1();
  
  	initEMIOS_0ch0(); 					// Initialize eMIOS 0 Time bases   
	initEMIOS_0ch8();
	initEMIOS_0ch16();
	initEMIOS_0ch23();
	initEMIOS_1ch23();					// Initialize eMIOS 1 Time bases  
			
	initEMIOS_0ch4(); 					// Initialize eMIOS 0 channel 4 as OPWM, Servo moteur    
	
  	initEMIOS_0ch3(); 					// Initialize eMIOS 0 channel 3 as OPWM, Moteur Gauche B   
	initEMIOS_0ch5(); 					// Initialize eMIOS 0 channel 5 as OPWM, Moteur Droit  B    

	initEMIOS_0ch6(); 					// Initialize eMIOS 0 channel 6 as OPWM, Moteur Gauche F  
	initEMIOS_0ch7(); 					// Initialize eMIOS 0 channel 7 as OPWM, Moteur Droit  F 
	
	initEMIOS_1ch11();					// Initialize eMIOS 1 channel 11 as OPWM, Rampe LEDs Avant
	initEMIOS_1ch13(); 					// Initialize eMIOS 1 channel 20 as capture (capteur1 roue)  
	initEMIOS_1ch15(); 					// Initialize eMIOS 1 channel 22 as capture (capteur2 roue)  
	
	init_LinFLEX_3_UART();
	init_LinFLEX_0_UART();              // Liaison Série   


    
    initIrqVectors();
    initPIT1();
    
	initINTC(); 

}

// *********************  Initialisation de la carte    ***********************   

void initModesAndClock(void) {
	ME.MER.R = 0x0000001D;          	// Enable DRUN, RUN0, SAFE, RESET modes   
	                              		// Initialize PLL before turning it on:   
	CGM.FMPLL_CR.R = 0x02400100;    	// 8 MHz xtal: Set PLL0 to 64 MHz      
	ME.RUN[0].R = 0x001F0074;       	// RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL   
	
	ME.RUNPC[1].R = 0x00000010;     	// Peri. Cfg. 1 settings: only run in RUN0 mode   
	ME.PCTL[32].R = 0x01;       		// MPC56xxB ADC 0: select ME.RUNPC[1]   
  	ME.PCTL[57].R = 0x01;       		// MPC56xxB CTUL: select ME.RUNPC[1] 
  	  
  	ME.PCTL[48].R = 0x01;           	// MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] 
  	ME.PCTL[50].R = 0x01;           	// MPC56xxB/P/S LINFlex 2: select ME.RUNPC[1] 
  	ME.PCTL[51].R = 0x01;           	// MPC56xxB/P/S LINFlex 3: select ME.RUNPC[1] 
  	  
	ME.PCTL[68].R = 0x01;           	// MPC56xxB/S SIUL:  select ME.RUNPC[1]   
	ME.PCTL[72].R = 0x01;           	// MPC56xxB/S EMIOS 0:  select ME.RUNPC[1]
	ME.PCTL[73].R = 0x01;           	// MPC56xxB/S EMIOS 1:  select ME.RUNPC[1]
	   
	ME.PCTL[92].R = 0x01;				// PIT, RTI: select ME_RUN_PC[1]   	   
	
	                              		// Mode Transition to enter RUN0 mode:   
	ME.MCTL.R = 0x40005AF0;         	// Enter RUN0 Mode & Key   
	ME.MCTL.R = 0x4000A50F;         	// Enter RUN0 Mode & Inverted Key     
	while (ME.GS.B.S_MTRANS) {}     	// Wait for mode transition to complete       
	                          			// Note: could wait here using timer and/or I_TC IRQ   
	while(ME.GS.B.S_CURRENTMODE != 4) {}// Verify RUN0 is the current mode   
}


void initPeriClkGen(void) {
		CGM.SC_DC[0].R = 0x80; 			// donne la	 clock générale aux périphériques
		CGM.SC_DC[1].R = 0x80;
		CGM.SC_DC[2].R = 0x80;
		
}

// *************************  desactivations du watchdog   ************************

void disableWatchdog(void)
{
	SWT.SR.R = 0x0000c520;     			// Write keys to clear soft lock bit   
  	SWT.SR.R = 0x0000d928; 
  	SWT.CR.R = 0x8000010A;     			// Clear watchdog enable (WEN)   
}


//  ********************** Partie interrutions************************ 
asm void initIrqVectors(void) {
  lis	   r3, __IVPR_VALUE@h   // IVPR value is passed from link file   
  ori      r3, r3, __IVPR_VALUE@l 
  mtivpr   r3									 
}

void initINTC(void) {	
  INTC.MCR.B.HVEN = 1;       // Single core: initialize for HW vector mode   
//} 
								// pepino : liaison des 2 fonctions
//void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          // Single Core: Lower INTC's current priority 
  asm(" wrteei 1");	    	   // Enable external interrupts 
}

void initPads (void) {

	// SIU.PCR[2].R = 0x0503;           	// MPC56xxB: Initialize PA[2] as eMIOS[2] input   
	// SIU.PCR[3].R = 0x0600;           	// MPC56xxB: Initialize PA[3] as eMIOS[3] output   
	// SIU.PCR[20].R = 0x2000;          	// MPC56xxB: Initialize PB[4] as ANP0   
	
	SIU.PCR[23].R = 0x2000;          	// MPC56xxB: Initialize PB[7] as AD3 : current sense for motor 1   => ANP3
	SIU.PCR[24].R = 0x2000;          	// MPC56xxB: Initialize PB[8] as AD4 : current sense for motor 2   => ANS0
	
	//SIU.PCR[42].R = 0x0200;				// Initialise la pin de contrôle du freinage en sortie -> PC[10]  
	
	// potar (si remapé au fil .... )
	SIU.PCR[57].R = 0x2000;          	// MPC56xxB: Initialize PD[9] as ANP13 -> potentiomètre  
	
	
	// new cameras
	SIU.PCR[80].R = 0x2000;				// PF[0]  = entrée analogique cam1 => ANS8
	SIU.PCR[81].R = 0x2000;				// PF[1]  = entrée analogique cam2 => ANS9
	
	SIU.PCR[82].R = 0x0200;				// PF[2]  = SI  cam 1 
	SIU.PCR[83].R = 0x0200;				// PF[3]  = SI  cam 2
	SIU.PCR[84].R = 0x0200;				// PF[4]  = CLK cam 1 
	SIU.PCR[85].R = 0x0200;				// PF[5]  = CLK cam 2 
	SIU.PCR[86].R = 0x0200;				// PF[6]  = VDD cam 1 
	SIU.PCR[87].R = 0x0200;				// PF[7]  = VDD cam 2
	SIU.PCR[88].R = 0x0200;				// PF[8]  = GND cam 1 
	SIU.PCR[89].R = 0x0200;				// PF[9]  = GND cam 2 
	
	// alimentation
	SIU.GPDO[86].R = 1;
	SIU.GPDO[87].R = 1;
	SIU.GPDO[88].R = 0;
	SIU.GPDO[89].R = 0;
	// Fin Camera
	
	
	// boutons et leds
	SIU.PCR[64].R = 0x0100;				// Initialise PE[0] (S1) en entrÃ©e   
	SIU.PCR[65].R = 0x0100;				// Initialise PE[1] (S2) en entrÃ©e   
	SIU.PCR[66].R = 0x0100;				// Initialise PE[2] (S2) en entrÃ©e   
	SIU.PCR[67].R = 0x0100;				// Initialise PE[3] (S2) en entrÃ©e   
	SIU.PCR[68].R = 0x0200;				// Initialise la LED 1 en sortie   
	SIU.PCR[69].R = 0x0200;				// Initialise la LED 2 en sortie   
	SIU.PCR[70].R = 0x0200;				// Initialise la LED 3 en sortie   
	SIU.PCR[71].R = 0x0200;				// Initialise la LED 4 en sortie   
	
	SIU.GPDO[68].B.PDO = 1;				// mises des leds en OFF;
	SIU.GPDO[69].B.PDO = 1;
	SIU.GPDO[70].B.PDO = 1;
	SIU.GPDO[71].B.PDO = 1;
	
	
	    // **********************  Activation des H-Bridge *******************************
	
	SIU.PCR[16].R = 0x0200;				// Motor enable pins set as output pins
	SIU.PCR[17].R = 0x0200;		
	SIU.GPDO[16].B.PDO = 1;				// Motors enable pins set 
	SIU.GPDO[17].B.PDO = 1;
	
	// masse de nappe pépino
	// vu que cette carte est déjà reliée à la masse ailleurs, on peut s'en passer 
	SIU.PCR[96].R = 0x0100;				// set as input pin
	
	// Pin débug
	SIU.PCR[15].R = 0x0200;				// PIN A15 en sortie
	SIU.PCR[14].R = 0x0200;				// PIN A14 en sortie
	
}

// ********************** Conversion Analogique-NumÃ©rique **************************  
void initADC(void) {
	//ADC.MCR.R = 0x20020000;         	// Initialize ADC scan mode  
	ADC.MCR.R = 0x00000000;         	// Initialize ADC one shot mode  
	//ADC.NCMR[0].R = 0x00006017;      	// Select ANP0:2 and 4 inputs for normal conversion   
	
	// ADC_S[0] = ch[32]
	// ADC_S[8] = ch[40]
	ADC.NCMR[1].R = 0x0300;				// select ANS8 et ANS9 pour la conversion "normale"
	
	ADC.CTR[0].R = 0x00008606;       	// Conversion times for 32MHz ADClock	for precision chanels
	ADC.CTR[1].R = 0x00008606;       	// Conversion times for 32MHz ADClock	for standards chanels
	
	ADC.CTR[0].B.INPLATCH = 1;
	ADC.CTR[0].B.INPCMP = 3;
	ADC.CTR[0].B.INPSAMP = 25;
	
	ADC.CTR[1].B.INPLATCH = 1;
	ADC.CTR[1].B.INPCMP = 3;
	ADC.CTR[1].B.INPSAMP = 25;
	
}


// *************************  Trigger ***************************************************
void initCTU(void) {
  	CTU.EVTCFGR[2].R = 0x00008000;  	 // Config event on eMIOS Ch 2 to trig ANP[0]   
}



// ************* Initialisation de l'EMIOS ****************************  
// on utilise les 2 EMIOS
// ***************     Time base  *************************************  
// initialise l'EMIOS, ce qui modifie tous les chanels
void initEMIOS_0(void) {  
	EMIOS_0.MCR.B.GPRE= 7;   			// Divide 64 MHz sysclk by 7+1 = 8 for 8MHz eMIOS clk  
	EMIOS_0.MCR.B.GPREN = 1;			// Enable eMIOS clock   
	EMIOS_0.MCR.B.GTBE = 1;  			// Enable global time base   
	EMIOS_0.MCR.B.FRZ = 1;    			// Enable stopping channels when in debug mode   
}

void initEMIOS_1(void) {  
	EMIOS_1.MCR.B.GPRE= 7;   			// Divide 64 MHz sysclk by 7+1 = 8 for 8MHz eMIOS clk  
	EMIOS_1.MCR.B.GPREN = 1;			// Enable eMIOS clock   
	EMIOS_1.MCR.B.GTBE = 1;  			// Enable global time base   
	EMIOS_1.MCR.B.FRZ = 1;    			// Enable stopping channels when in debug mode   
}

// initialisation bus de l'EMIOS

// fourni l'horloge pour le servo moteur => bus horloge B
void initEMIOS_0ch0(void) {        		// EMIOS 0 CH 0: Modulus Up Counter   
	EMIOS_0.CH[0].CADR.R = 39999;   	// Period will be 39999+1 = 40000 clocks (20 msec)   
										// clk @ 8MHz prescaler à 4 => 2MHz 
										// pour compter jusqu'à 40000 => 20 ms
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; 	// Modulus Counter Buffered (MCB)   
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;   	// Use internal counter   (EMIOS0 @ 8 MHz)
	EMIOS_0.CH[0].CCR.B.UCPRE=3;     	// Set channel prescaler to divide by 4
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   	// Enable prescaler;
	EMIOS_0.CH[0].CCR.B.FREN = 1;   	// Freeze channel counting when in debug mode  
}

// je pense useless
void initEMIOS_0ch8(void) {        		// EMIOS 0 CH 0: Modulus Up Counter   
	EMIOS_0.CH[8].CADR.R = 0xFFFFFE;   	// Period will be 19999+1 = 20000 clocks (20 msec) 
	EMIOS_0.CH[8].CCR.B.MODE = 0x50; 	// Modulus Counter Buffered (MCB)   
	EMIOS_0.CH[8].CCR.B.BSL = 0x3;   	// 0b11 -> Use internal counter   
	EMIOS_0.CH[8].CCR.B.UCPRE = 3;     	// Set channel prescaler to divide by 4  
	EMIOS_0.CH[8].CCR.B.UCPEN = 1;   	// Enable prescaler; uses default divide by 1  
	EMIOS_0.CH[8].CCR.B.FREN = 1;   	// Freeze channel counting when in debug mode  
	//EMIOS_0.CH[11].CCR.B.DMA = 0; // generate interrupt
	//EMIOS_0.CH[11].CCR.B.FEN = 1;
}
// je pense useless aussi
void initEMIOS_0ch16(void)
{
	EMIOS_0.CH[16].CADR.R = 2100;   	// Period will be 19999+1 = 20000 clocks (20 msec)  
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; 	// Modulus Counter Buffered (MCB)   
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;   	// Use internal counter   
	EMIOS_0.CH[16].CCR.B.UCPRE=0;     	// Set channel prescaler to divide by 1   
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   	// Enable prescaler; uses default divide by 1  
	EMIOS_0.CH[16].CCR.B.FREN = 1;   	// Freeze channel counting when in debug mode  
}

// fourni le compteur pour les PWM des moteurs : de 0 à 999 en 125µs
void initEMIOS_0ch23(void) {        	// EMIOS 0 CH 23: Modulus Up Counter   
	EMIOS_0.CH[23].CADR.R = 999;      	// Period will be 999+1 = 1000 clocks (125 µs = 8kHz)  
	EMIOS_0.CH[23].CCR.B.MODE = 0x50; 	// Modulus Counter Buffered (MCB)   
	EMIOS_0.CH[23].CCR.B.BSL = 0x3;   	// Use internal counter @ 8MHz 
	EMIOS_0.CH[23].CCR.B.UCPRE=0;     	// Set channel prescaler to divide by 1   
	EMIOS_0.CH[23].CCR.B.UCPEN = 1;   	// Enable prescaler; uses default divide by 1  
	EMIOS_0.CH[23].CCR.B.FREN = 1;   	// Freeze channel counting when in debug mode
}

// fourni le compteur pour le PWM des LEDS : de 0 à 999 en 125µs
void initEMIOS_1ch23(void) {        	// EMIOS 1 CH 23: Modulus Up Counter   
	EMIOS_1.CH[23].CADR.R = 999;      	// Period will be 999+1 = 1000 clocks (125 µs = 8kHz)  
	EMIOS_1.CH[23].CCR.B.MODE = 0x50; 	// Modulus Counter Buffered (MCB)   
	EMIOS_1.CH[23].CCR.B.BSL = 0x3;   	// Use internal counter @ 8MHz 
	EMIOS_1.CH[23].CCR.B.UCPRE=0;     	// Set channel prescaler to divide by 1   
	EMIOS_1.CH[23].CCR.B.UCPEN = 1;   	// Enable prescaler; uses default divide by 1  
	EMIOS_1.CH[23].CCR.B.FREN = 1;   	// Freeze channel counting when in debug mode
}

// ************************ Canaux ************************************  

void initEMIOS_0ch4(void) {        			// EMIOS 0 CH 4: Servo-moteur   
	EMIOS_0.CH[4].CADR.R = 0;     			// Rising edge when channel counter bus=0  
	EMIOS_0.CH[4].CBDR.R = pos_milieu_servo;// falling edge when channel counter bus=pos_milieu_servo  
	EMIOS_0.CH[4].CCR.B.BSL = 0x01;  		// Use counter bus B -> Time base channel 0  
	EMIOS_0.CH[4].CCR.B.EDPOL = 1;  		// Polarity-leading edge sets output   
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; 		// Mode is OPWM Buffered   
	SIU.PCR[28].R = 0x0600;           		// MPC56xxS: Assign EMIOS_0 ch 4 to pad PB[12]  
}


// voir si 0x400 serait pas suffisant? par ce que le 4 donne l'alternate function,
// donc pas besoin de mettre les buffer (2 en sortie et 1 en entrée) en plus?
// peut être le 1 en entrée pour être capable de lire la pin quand même
// le 2 semble en effet superflu ...

void initEMIOS_0ch3(void) {        		// EMIOS 0 CH 3: Motor Left Backward
	EMIOS_0.CH[3].CADR.R = 0;     		// Leading edge when channel counter bus=0  
	EMIOS_0.CH[3].CBDR.R = 000;			// Trailing edge when channel counter bus=0 // max à 999
	EMIOS_0.CH[3].CCR.B.BSL = 0x0;  	// Use counter bus A -> Time base channel 23   
	EMIOS_0.CH[3].CCR.B.EDPOL = 1;  	// Polarity-leading edge sets output   
	EMIOS_0.CH[3].CCR.B.MODE = 0x60; 	// Mode is OPWM Buffered   
	SIU.PCR[27].R = 0x0600;           	// MPC56xxS: Assign EMIOS_0 ch 3 to pad PB[11]  
}

void initEMIOS_0ch5(void) {        		// EMIOS 0 CH 5: Motor Right Backward
	EMIOS_0.CH[5].CADR.R = 0;    		// Leading edge when channel counter bus=0  
	EMIOS_0.CH[5].CBDR.R = 000;			// Trailing edge when channel's counter bus=0 // max à 999
	EMIOS_0.CH[5].CCR.B.BSL = 0x0; 		// Use counter bus A -> Time base channel 23   
	EMIOS_0.CH[5].CCR.B.EDPOL = 1; 		// Polarity-leading edge sets output  
	EMIOS_0.CH[5].CCR.B.MODE = 0x60; 	// Mode is OPWM Buffered   
	SIU.PCR[29].R = 0x0600;           	// MPC56xxS: Assign EMIOS_0 ch 5 to pad PB[13]  
}


void initEMIOS_0ch6(void) {        		// EMIOS 0 CH 6: Motor Left Forward
	EMIOS_0.CH[6].CADR.R = 0;     		// Leading edge when channel counter bus=0  
	EMIOS_0.CH[6].CBDR.R = 000;			// Trailing edge when channel counter bus=0 // max à 999
	EMIOS_0.CH[6].CCR.B.BSL = 0x0;  	// Use counter bus A -> Time base channel 23   
	EMIOS_0.CH[6].CCR.B.EDPOL = 1;  	// Polarity-leading edge sets output   
	EMIOS_0.CH[6].CCR.B.MODE = 0x60; 	// Mode is OPWM Buffered   
	SIU.PCR[30].R = 0x0600;           	// MPC56xxS: Assign EMIOS_0 ch 6 to pad PB[14]  
}

void initEMIOS_0ch7(void) {        		// EMIOS 0 CH 7: Motor Right Forward
	EMIOS_0.CH[7].CADR.R = 0;    		// Leading edge when channel counter bus=0  
	EMIOS_0.CH[7].CBDR.R = 000;			// Trailing edge when channel's counter bus=0 // max à 999
	EMIOS_0.CH[7].CCR.B.BSL = 0x0; 		// Use counter bus A -> Time base channel 23   
	EMIOS_0.CH[7].CCR.B.EDPOL = 1; 		// Polarity-leading edge sets output  
	EMIOS_0.CH[7].CCR.B.MODE = 0x60; 	// Mode is OPWM Buffered   
	SIU.PCR[31].R = 0x0600;           	// MPC56xxS: Assign EMIOS_0 ch 7 to pad PB[15]  
}



// nappe pepino :
// masse sur G[0] soit PCR[96]  // dans init_pads()
// controle leds   sur G[2] PCR[98]  EMIOS1[11]
// Input capteur 1 sur G[4] PCR[100] EMIOS1[13]
// Input capteur 2 sur G[6] PCR[102] EMIOS1[15]


void initEMIOS_1ch11(void)        		// EMIOS 0 CH 18: LED
{
	EMIOS_1.CH[11].CADR.R = 0;    		// Leading edge when channel counter bus=0  
	EMIOS_1.CH[11].CBDR.R = 500;		// Trailing edge when channel's counter bus=500 // max à 999
	EMIOS_1.CH[11].CCR.B.BSL = 0x0; 	// Use counter bus A -> Time base channel 23   
	EMIOS_1.CH[11].CCR.B.EDPOL = 1; 	// Polarity-leading edge sets output  
	EMIOS_1.CH[11].CCR.B.MODE = 0x60; 	// Mode is OPWM Buffered   
	SIU.PCR[98].R = 0x0600;           	// MPC56xxS: Assign EMIOS_1 ch 11 to pad PG[2]
}

void initEMIOS_1ch13(void) // capteur de vitesse 1
{
	EMIOS_1.CH[13].CCR.B.BSL = 0x0;		// Use counter bus A -> Time base channel 23
											// mais sert à rien vu qu'on regarde pas les valeurs capturées   
	EMIOS_1.CH[13].CCR.B.MODE = 0x02;  	// Mode is SAIC
	EMIOS_1.CH[13].CCR.B.EDSEL = 1;		// générer interrupt sur front montant et descendant 
	EMIOS_1.CH[13].CCR.B.FCK = 1;	  	// Input filter bit clock reference is the system clock  
    EMIOS_1.CH[13].CCR.B.IF = 1;		// Input filter bit of 2 clock cycles, p684 (éliminer les filtres) 
    
	EMIOS_1.CH[13].CCR.B.FEN = 1;       // 1 -> flag enable
	EMIOS_1.CH[13].CCR.B.DMA = 0;		// envoie le flag sur interruption (avec ligne de dessus)
	SIU.PCR[100].R = 0x0500;             // PG[4] initialisée en entrée
	INTC.PSR[163].R = 7;           		// EMIOS0[20]  interrupt vector with priority 5
}
void initEMIOS_1ch15(void) // capteur de vitesse 2
{
	EMIOS_1.CH[15].CCR.B.BSL = 0x0;		// Use counter bus A -> Time base channel 23
											// mais sert à rien vu qu'on regarde pas les valeurs capturées
	EMIOS_1.CH[15].CCR.B.MODE = 0x02;  	// Mode is SAIC
	EMIOS_1.CH[15].CCR.B.EDSEL = 1;		// générer interrupt sur front montant et descendant 
	EMIOS_1.CH[15].CCR.B.FCK = 1;	  	// Input filter bit clock reference is the system clock  
    EMIOS_1.CH[15].CCR.B.IF = 1;		// Input filter bit of 2 clock cycles, p684 (éliminer les filtres)    

	EMIOS_1.CH[15].CCR.B.FEN = 1;       // 1 -> flag enable
	EMIOS_1.CH[15].CCR.B.DMA = 0;		// envoie le flag sur interruption (avec ligne de dessus)
	SIU.PCR[102].R = 0x0500;             // PG[6] initialisée en entrée
	INTC.PSR[164].R = 7;           		// EMIOS0[22] interrupt vector with priority 5
}

// *************************  Liaison série *************************************  
// module UART pour l'emission
void init_LinFLEX_0_UART (void) 
{	

	// enter INIT mode   
	LINFLEX_0.LINCR1.B.SLEEP = 0;
	LINFLEX_0.LINCR1.B.INIT = 1; 

	// wait for the INIT mode   
	while (LINFLEX_0.LINSR.B.LINS != 1) {}

	// configure pads   
	SIU.PCR[18].R = 0x0604;     		// Configure pad PB2 for AF1 func: LIN0TX   

	// configure for UART mode   old version 
	//LINFLEX_0.UARTCR.R = 0x0001; 		// set the UART bit first to be able to write the other bits   
	//LINFLEX_0.UARTCR.R = 0x6013; 		// 8bit data, no parity, Tx and Rx enabled, UART mode   
						 				// Transmit buffer size = 4 (TDFL = 0   
						 				// Receive buffer size = 1 (RDFL = 0)   */
	LINFLEX_0.UARTCR.B.UART = 1;		// set the UART bit first to be able to write the other bits
	LINFLEX_0.UARTCR.B.WL = 1;			// 8bit data 0 parity
	LINFLEX_0.UARTCR.B.TXEN = 1;		// TX Enable
	LINFLEX_0.UARTCR.B.RXEN = 0;		// RX Disable 
	LINFLEX_0.UARTCR.B.RDFL = 0;		// buffer de réception de 1 mot (8 bit)
	LINFLEX_0.UARTCR.B.TDFL = 3;		// buffer d'emmission  de 4 mots
	
	
	// configure baudrate 115200 
	// assuming 64 MHz peripheral set 1 clock 		
	LINFLEX_0.LINFBRR.R = 12;
	LINFLEX_0.LINIBRR.R = 34;

	// enter NORMAL mode 
	LINFLEX_0.LINCR1.B.INIT = 0;

	LINFLEX_0.BDRL.B.DATA0 = '\n';		// data d'initialisation
										// pour activer le systeme avec les interruptions
	
	INTC.PSR[80].R = 5;					// donne une priorité de 5 pour l'interruption TX
	//l'interruption TX n'est pas activée, elle l'est uniquement si le buffer est suffisament rempli
}

// module UART pour la reception
void init_LinFLEX_3_UART (void) 
{	

	// enter INIT mode   
	LINFLEX_3.LINCR1.B.SLEEP = 0;
	LINFLEX_3.LINCR1.B.INIT = 1; 

	// wait for the INIT mode   
	while (LINFLEX_3.LINSR.B.LINS != 1) {}

	// configure pads    
	SIU.PCR[75].R = 0x0106;     		// Configure pad PE11 as input,
//	SIU.PCR[8].R = 0x0100;     			// Configure pad PA8 as input // that's the other pad for LIN3RX
	SIU.PSMI[31].R = 1;					// select PE11 as the input of LIN3RX

	

	LINFLEX_3.UARTCR.B.UART = 1;		// set the UART bit first to be able to write the other bits
	LINFLEX_3.UARTCR.B.WL   = 1;		// 8bit data 0 parity
	LINFLEX_3.UARTCR.B.TXEN = 0;		// TX Disable
	LINFLEX_3.UARTCR.B.RXEN = 1;		// RX Enable
	LINFLEX_3.UARTCR.B.RDFL = 0;		// buffer de réception de 1 mot (8 bit)
	
//	LINFLEX_3.LINCR1.B.RBLM = 0;		// over write RX buffer???
	
	LINFLEX_3.UARTSR.R = 0xFFFF;
	
	// configure baudrate 115200 
	// assuming 64 MHz peripheral set 1 clock 		
	LINFLEX_3.LINFBRR.R = 12;
	LINFLEX_3.LINIBRR.R = 34;

	

	LINFLEX_3.LINCR1.B.INIT = 0;


	 
	LINFLEX_3.LINIER.B.DRIE = 1;		// active l'interruption en réception
	INTC.PSR[122].R = 2;				// donne une priorité de 2 à l'interruption de réception
	INTC.PSR[4].R = 1;					// et une priorité de 1 pour la Software interrupt 4 qui s'occupe de "comprendre" les messages reçus



}


void initPIT1(void) {
	PIT.PITMCR.B.MDIS = 0;			// PIT Master_Disable = 0
	PIT.PITMCR.B.FRZ = 1;	     	// Freeze in debug = 1
	PIT.CH[1].LDVAL.R = 64000000/100; // Timeout= 6 400 000 sysclks x 1sec/64M sysclks = 100 ms 
	PIT.CH[1].TCTRL.B.TEN = 1;		// Ch[1] Timer Enable
	PIT.CH[1].TCTRL.B.TIE = 1;		// Ch[1] Interrupt enable
	INTC.PSR[60].R = 2;           	// PIT 1 interrupt vector with priority 2
}