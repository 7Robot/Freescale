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

#define FALSE 0
#define TRUE 1

extern IVOR4Handler();


uint8_t autor_blink = 1;


void main (void)
{	
	uint32_t i = 0;			    // Dummy idle counter 
	uint8_t toto = 0;
	float bidule = 0;
	uint16_t pb_calculs = 0;

	uint16_t buff1[128], buff2[128], buff4[128],buff3[128], buff5[128],buff6[128], buff7[128],buff8[128];
	
	
	// partie interface
	
	uint16_t menu[16] = {0};
	uint16_t parametreARegler=0, valeurDuParam=0;
	uint16_t R = TRUE; //La rampe de leds
	uint16_t ld[4] = {0};
	uint16_t nb_btn = 5; 
	uint16_t raccourciAffichage;
		
	uint32_t arret_a_faire = 300;
	
	init();
	
	Set_PWM_Leds(0);
	for (i = 0; i < 50000; i++);	// petit delai à la noix
	
	
	

	Set_Dir_Servo(0);
	
	Commande_Moteur(0,0);
	
	// attend avant de partir faire le premier tour de boucle (évite de déclencher le pb_calculs dès le debut) 
	for (i = 0; i < 50; i++)
	{
		autorisation_aquiz = 0;
		while (!autorisation_aquiz);
	}
	
	//initEMIOS_0ch4(); 					// Initialize eMIOS 0 channel 4 as OPWM, Servo moteur  
	TransmitData("\nI AM ALIVE !\n");

	
	// **********************************************************
	
	//while(menu[15] == 0) 
/*	while(0)
	{
		// limite : un tour de boucle toute les 10 ms
		while (autorisation_aquiz == 0);
		autorisation_aquiz = 0;
		toto ++;

		nb_btn = get_buttons(); 
		
		if ((nb_btn == 3)&&(!R))
			menu[parametreARegler] = get_potar();
		
		else
		{
			//traitement des modifs
			if (R) // On est dans le menu de choix des paramètres
			{
				switch (nb_btn)
				{
					case 0 : 	R = !R;
							valeurDuParam = 0;
						break;
					case 1 : 	parametreARegler = (parametreARegler+1) % 16; 
						break;
					case 2 : 	parametreARegler = (parametreARegler+15) % 16;
						break;
				}
			}
			else // On est dans le menu de modif du param actuel
			{
				switch (nb_btn)
				{
				case 0 : 	R = !R;
						parametreARegler = 0; 
					break;
				case 1 : 	valeurDuParam = (valeurDuParam+1) % 16;
						menu[parametreARegler] = valeurDuParam;
					break;
				case 2 : 	valeurDuParam = (valeurDuParam+15) % 16;
						menu[parametreARegler] = valeurDuParam;
					break;
				}
			}
		}
		

		// *******************************************  affichage leds  *********************************************** //

		if (R) // on doit afficher le numero de paramètre
		{
			ld[3] = (parametreARegler&1);
			ld[2] = (parametreARegler&2)>>1;
			ld[1] = (parametreARegler&4)>>2;
			ld[0] = (parametreARegler&8)>>3;	
		}
		else
		{
			if (raccourciAffichage = menu[parametreARegler] > 15)
				raccourciAffichage = menu[parametreARegler] / 64;
			else
				raccourciAffichage = menu[parametreARegler];
			
			ld[3] = (raccourciAffichage&1);
			ld[2] = ((raccourciAffichage&2) !=0);
			ld[1] = ((raccourciAffichage&4) !=0);
			ld[0] = ((raccourciAffichage&8) !=0);	

		}
		set_led(0,ld[0]);
		set_led(1,ld[1]);
		set_led(2,ld[2]);
		set_led(3,ld[3]);
		
		Set_PWM_Leds(50.0 * R);
		
		autor_blink = (menu[1] != 0);
		
		if (toto == 0)
		{
			for (i = 0; i <= 15; i++)
			{
				TransmitData("\n");
				printhex16(menu[i]);
			}
			TransmitData("    \n");
		}
	}*/
	
	
	
	// ***************************************************************************************
	// ************************* boucle principale ************************
	while (1)
	{ 

		if (autorisation_aquiz == 1)
		{
			if (mode_manuel == 0)
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
				
				analyse_cam_bis((toto == 130) && mode_spam);
				
				if (!autor_vitesse)
					arret_a_faire = 300;
				
				if (arret_a_faire >= 300)
					arret_a_faire ++;
				
				if (arret_a_faire >= 1000)
					arret_a_faire = 0;
				
				if (arret_a_faire == 0 && ligne_arrivee)
					arret_a_faire ++;
				
				if (arret_a_faire < 60 && arret_a_faire != 0)
					arret_a_faire ++;
				
				if (arret_a_faire == 60)
					autor_vitesse = 0;
				
				Asserv_Vitesse(autor_vitesse * calcul_consigne_vitesse(toto == 131 && mode_spam));
				
				Controle_Direction(0);
			

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
				else if (toto < 128 && mode_spam)
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
					TransmitData("    \n");
					
					
					
				}
				else if (toto == 129 && mode_spam)
				{
					TransmitData("\nmilieu1 ");
					printfloat(milieu1);
					TransmitData("\nmilieu2 ");
					printfloat(milieu2);
					
					
					if (pb_aquiz1 == 0)
					{
						TransmitData("\n\ncam1 :");
						for (i = 0; i < nb_bandes_1; i++)
						{
							TransmitData("\nBande ");
							if (bandes_1[i][0])
								TransmitData("blanche de ");
							else
								TransmitData("noire de ");
							printfloat(bandes_1[i][1]);
							TransmitData(" pixels");
						}
						
						if (ligne_arrivee)
							TransmitData("\nligne d'arrivée detectée");
					}
					else
						TransmitData("\nErreur Cam 1\n");
					
					if (pb_aquiz2 == 0)
					{
						TransmitData("\n\ncam2 :");
						for (i = 0; i < nb_bandes_2; i++)
						{
							TransmitData("\nBande ");
							if (bandes_2[i][0])
								TransmitData("blanche de ");
							else
								TransmitData("noire de ");
							printfloat(bandes_2[i][1]);
							TransmitData(" pixels");
						}
					}
					else
						TransmitData("\nErreur Cam 2\n");
					TransmitData("    \n");
				}
				else if (toto == 250 && mode_spam)
				{
					if (mode_spam != -1 && mode_spam != 0)
						mode_spam --;
					if (mode_spam == 0)
						TransmitData("\nspam OFF   \n");
				}
						
					
				
				if (autorisation_aquiz == 1)		// si l'autorisation de commencer les calculs a été donnée durant les calculs
					pb_calculs = 500;	// 5 sec		// ça veut dire que la boucle prends plus de 10 ms pour se faire
				
				
				if (pb_calculs)
				{
					set_led(2,1);
					pb_calculs --;
				}
				else
					set_led(2,0);
					
			}// fin mode automatique
			else	// mode manuel, pilotage par ordi
			{
				Set_Dir_Servo(rotation_manuelle);
				Commande_Moteur(vitesse_manuelle,vitesse_manuelle);
				rotation_manuelle = 0.98 * rotation_manuelle;
				vitesse_manuelle = 0.98 * vitesse_manuelle;
				if (rotation_manuelle < 0.05 && rotation_manuelle > - 0.05)
					rotation_manuelle = 0.0;
				if (vitesse_manuelle < 0.05 && vitesse_manuelle > - 0.05)
					vitesse_manuelle = 0.0;
				toto++;
				if (toto == 255 && mode_spam)
				{
					TransmitData("Vitesse : ");
					printfloat (vitesse_manuelle);
					TransmitData("\nRotation : ");
					printfloat(rotation_manuelle);
					TransmitData("    \n");										
				}
			}
		}
	}	
}

void Pit1ISR(void) {		// interrupt every ms
    autorisation_aquiz = 1;
  	if (autor_blink)
  		blink_led(0);
  	PIT.CH[1].TFLG.B.TIF = 1;    // MPC56xxP/B/S: CLear PIT 1 flag by writing 1 
}



