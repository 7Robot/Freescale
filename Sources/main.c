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

	uint16_t buff1[128], buff2[128], buff4[128],buff3[128], buff5[128],buff6[128], buff7[128],buff8[128];
		
	init();
	for (i = 0; i < 1000000; i++);	// petit delai à la noix
	
	TransmitData("\nI AM ALIVE !\n");
	
	while (1)
	{ 

		if (autorisation_aquiz == 1)
		{
			
			set_led(3,1);
				for (i = 0; i < 1128; i++);
			set_led(3,0);
			
			
			autorisation_aquiz = 0;
			max_lum = Acquisitions_Cameras();
			asserv_leds(max_lum);
			set_led(3,1);
			moy_cam(1);
			set_led(3,0);
				for (i = 0; i < 1128; i++);
			set_led(3,1);
			calcul_courbe();
			set_led(3,0);
				for (i = 0; i < 1128; i++);
			set_led(3,1);
			retranche_courbe();
			set_led(3,0);
			
			//Controle_Direction(0);
			//Set_PWM_Leds(led_power);
		/*	Asserv_Vitesse(bidule/30.0);*/

			toto++;
			if (toto == 255)
			{
					
						
				for (i = 0; i< 128; i++)
				{
					buff1[i] = 5*camera1_valeurs[i];
					buff2[i] = 5*camera2_valeurs[i];
					buff3[i] = camera1_valeurs_m[i];
					buff4[i] = camera2_valeurs_m[i];
					buff5[i] = camera1_courbe[i];
					buff6[i] = camera2_courbe[i];
					buff7[i] = camera1_p[i];
					buff8[i] = camera2_p[i];
					
				}
				if (mode_spam)
					TransmitData("\n\n\nstart:\n\n");
					
				//toto = 0;
			}
			if (toto < 128 && mode_spam)
			{
				printhex16(buff1[toto]);
				TransmitCharacter(' ');
				printhex16(buff2[toto]);
				TransmitCharacter(' ');
				printhex16(buff3[toto]);
				TransmitCharacter(' ');
				printhex16(buff4[toto]);
				TransmitCharacter(' ');
				printhex16(buff5[toto]);
				TransmitCharacter(' ');
				printhex16(buff6[toto]);
				TransmitCharacter(' ');
				printhex16(buff7[toto]);
				TransmitCharacter(' ');
				printhex16(buff8[toto]);
				TransmitCharacter('\n');

			}
			else if (toto == 128 && mode_spam)
			{
				TransmitData("\nlum : ");
				printhex16(max_lum);
				TransmitData("\n    ");
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



