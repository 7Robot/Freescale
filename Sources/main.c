// main.c - Hardware vector mode program using C isr 
// Feb 12, 2009 S.Mihalik Initial version based on previous AN2865 example 
// May 22 2009 S. Mihalik- disabled watchdog, simplified mode, clock code 
// Jul 03 2009 S Mihalik - Simplified code 
// Mar 14 2010 SM - modified initModesAndClock, updated header file 
// Copyright Freescale Semiconductor, Inc. 2009. All rights reserved. 

#include "init.h"
#include "liaison_serie.h"
#include "globals.h"
#include "moteur.h"
#include "camera.h"
#include "controle.h"
#include "leds_boutons.h"


extern IVOR4Handler();



void main (void)
{	
	uint32_t i = 0;			    // Dummy idle counter 
	uint8_t toto = 0;
	float bidule = 0;
	
	uint32_t Port;
	int sens = 1;
	uint16_t buff1[128], buff2[128];
		
	init();

	while (1)
	{ 

		if (autorisation_aquiz == 1)
		{
			
			autorisation_aquiz = 0;
			Acquisitions_Cameras();
			
			Asserv_Vitesse(bidule/30.0);
			Set_Dir_Servo(bidule);
			Set_PWM_Leds(50 + bidule/2.0);
			bidule += 0.05;
			if (bidule >=100)
				bidule = -100;
				
			toto++;
			if (toto == 255)
			{
				for (i = 0; i< 128; i++)
				{
					buff1[i] = camera1_valeurs[i];
					buff2[i] = camera2_valeurs[i];
				}
				TransmitData("\n\n\nstart:\n\n");
			}
			if (toto < 128)
			{
				printhex16(buff1[toto]);
				TransmitCharacter(' ');
				printhex16(buff2[toto]);
				TransmitCharacter('\n');
			}
			


		//	SIU.GPDO[68].B.PDO = !SIU.GPDO[68].B.PDO;
		/*	
			if (sens == 1)
			{
				CMD += 0.005;
				if (CMD >= 2)
					sens = 0;
			}
			else
			{
				CMD -= 0.005;
				if (CMD <= -2)
					sens = 1;
			}
			//Asserv_Vitesse(CMD);*/
		
			/*TransmitData("main : \n");
			TransmitCharacter(LINFLEX_3.BDRM.B.DATA4);
			TransmitCharacter('\n');
			printhex16(	LINFLEX_3.LINCR1.R);
			TransmitCharacter('\n');
			printhex16(	LINFLEX_3.LINSR.R);
			TransmitCharacter('\n');
			printhex16(	LINFLEX_3.UARTCR.R);
			TransmitCharacter('\n');
			printhex16(	LINFLEX_3.UARTSR.R);
			TransmitCharacter('\n');
  			SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
  			EMIOS_0.CH[4].CBDR.R = pos_milieu_servo;
  			printfloat((float)pos_milieu_servo);*/

			
	
		
  	//LINFLEX03.BDRL.R = ('M'<<24)|('A'<<16)|('0'<<8)|('\n');
		/*	printfloat(CMD);
			TransmitData("\nDRIE ");
			printhex8(LINFLEX_0.LINIER.B.DRIE);
			TransmitData("\nDRF ");
			//printhex8(LINFLEX_0.UARTSR.B.DRF);
			TransmitData("\nRMB ");
			//printhex8(LINFLEX_0.UARTSR.B.RMB);			
			TransmitData("\nbuff ");
		//	printhex8(LINFLEX_0.BDRM.B.DATA4);
			TransmitData("\nUARTSR ");
			//printhex32(LINFLEX_0.UARTSR.R);
			TransmitData("\nLINIER ");
			//printhex32(LINFLEX_0.LINIER.R);
			TransmitData("\nLINSR ");
			//printhex32(LINFLEX_0.LINSR.R);
			
			TransmitData("\nnb ");*/
			
			
		/*	if (autorisation_aquiz == 0)
				TransmitData("\nok        ");
		}
		
	//	if (LINFLEX_0.UARTSR.B.DRF == 1)
	//		LINFLEX_0.UARTSR.B.DRF = 1;
		*/
		}
	//	SIU.GPDO[15].B.PDO = LINFLEX_0.UARTSR.B.RMB;
	//	SIU.GPDO[14].B.PDO = LINFLEX_0.UARTSR.B.DRF;
		
	}	
}

void Pit1ISR(void) {		// interrupt every ms
    autorisation_aquiz = 1;
  	blink_led(0);
  	PIT.CH[1].TFLG.B.TIF = 1;    // MPC56xxP/B/S: CLear PIT 1 flag by writing 1 
}



