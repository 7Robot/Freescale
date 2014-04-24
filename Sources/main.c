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
	uint16_t buff1[128], buff2[128], buff3[128],buff4[128];
	uint16_t max_lum;

		
	init();

	while (1)
	{ 

		if (autorisation_aquiz == 1)
		{
			
			autorisation_aquiz = 0;
			max_lum = Acquisitions_Cameras();
			asserv_leds(max_lum);
			
			coeffs_moy_cam(1);
			//Controle_Direction(0);
			Set_PWM_Leds(led_power);
		/*	Asserv_Vitesse(bidule/30.0);
			Set_Dir_Servo(bidule);
			Set_PWM_Leds(50 + bidule/2.0);
			bidule += 0.05;
			if (bidule >=100)
				bidule = -100;
				*/
			toto++;
			if (toto == 255)
			{
				for (i = 0; i< 128; i++)
				{
					buff1[i] = camera1_valeurs[i];
					buff2[i] = camera2_valeurs[i];
					buff3[i] = camera1_valeurs_t[i];
					buff4[i] = camera2_valeurs_t[i];
				}
				TransmitData("\n\n\nstart:\n\n");
			}
			if (toto < 128)
			{
				printhex16(buff1[toto]);
				TransmitCharacter(' ');
				printhex16(buff2[toto]);
				TransmitCharacter(' ');
				printhex16(buff3[toto]);
				TransmitCharacter(' ');
				printhex16(buff4[toto]);
				TransmitCharacter('\n');
			}
			else if (toto == 128)
			{
				TransmitData("\nlum : ");
				printhex16(max_lum);
				TransmitCharacter('\n');
				TransmitCharacter('\n');
				TransmitCharacter('\n');
				TransmitCharacter('\n');
				TransmitCharacter('\n');				
			}
			
			
			if (autorisation_aquiz == 1)
				set_led(2,1);
			else
				set_led(2,0);
			
		}
	}	
}

void Pit1ISR(void) {		// interrupt every ms
    autorisation_aquiz = 1;
  	blink_led(0);
  	PIT.CH[1].TFLG.B.TIF = 1;    // MPC56xxP/B/S: CLear PIT 1 flag by writing 1 
}



