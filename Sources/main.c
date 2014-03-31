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
			Controle_Direction(); 
			Asserv_Vitesse();
			Set_PWM_Leds(led_power);
			
			toto++;
			if (toto == 255)
			{
				TransmitData("\n\n\nstart:\n\n");
				for (i = 0; i< 128; i++)
				{
					printhex16(camera1_valeurs[i]);
					TransmitCharacter(' ');
					printhex16(camera2_valeurs[i]);
					TransmitCharacter('\n');
				}
				TransmitData("coucou, je suis une phrase\n");
				
			}
		}
	}	
}

void Pit1ISR(void) {		// interrupt every 10ms
    autorisation_aquiz = 1;
  	blink_led(0);
  	PIT.CH[1].TFLG.B.TIF = 1;    // MPC56xxP/B/S: CLear PIT 1 flag by writing 1 
}



