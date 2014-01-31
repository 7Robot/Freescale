#include "typedefs.h"
#include "MPC5604B_M07N.h"
#include "camera.h"
#include "milieu_ligne.h"
#include "orientation.h"
#include "moteur.h"
#include "delay.h"
#include "globals.h"
#include "extern_globals.h"
#include "liaison_serie.h"
#include "reset.h"

//pepino
#include <stdio.h>
#include <string.h>

int main(void) {

    uint8_t reset = 1, Moteur_F=0, Servo_F = 0, Moteur_ON = 0, Moteur_OFF = 0;
   	uint16_t titi = 0;
   	uint8_t milieu_camera_1 = 0;
   	static uint8_t milieu_camera_1_old = 0;
   	uint8_t milieu_camera_2 = 0;
   	static uint8_t milieu_camera_2_old = 0;
   	uint32_t bouton, old_bouton;
   	uint16_t* camera_valeurs_filtrees_1;
   	uint16_t* camera_valeurs_filtrees_2;
   	float commande_base;
   	float commande_orientation_proche;
   	float commande_finale;
    /******************************************************
    Tps acquisition cam + traitement = 4,2 ms
    Tps de mise Ã  jour du servo = 20 ms
    Tps de la boucle principale: main_timer_period 
    
    Sources d'interruption:
    
        PIT: Channel 0 -> Boucle principale (10ms)
        
        EMIOS: Emios0.Channel 11 -> Compteur de front montant
    *******************************************************/
                            
                            
    /************** Initialisation variables **************/
   
    main_timer_period = 0.01; // Boucle principale: 0.010 s
  
    init();
  
    // Initialisation variables moteurs
    moteur_derniere_erreur = 0;
    moteur_integrale = 0;
    moteur_compteur = 0;
    
    // Initialisation variables servo
    /*
    pos_min_servo = 0x3d0;	//0x3b0 = real min
    pos_max_servo = 0x610;	//0x630 = real max
    pos_milieu_servo = 0x4f0;
    */
    pos_min_servo = 0x3b0;	//0x3b0 = real min
    pos_max_servo = 0x5f0;	//0x630 = real max
    pos_milieu_servo = 0x4d0;
   //pos_max_servo = 0x4d0;  pos_min_servo = 0x4d0;
   
   	old_bouton = SIU.PGPDI[2].R & 0x80000000;
    /**************** Scheduled algorithm *****************/

 
    while(1)
    {
  		
        // Ici est le code de reset
        SIU.PGPDO[0].R = 0x00000000;		// Desactive les 2 moteurs
        //reload();
        do
        {
        	/* Boutons de controle */
            main_fin_boucle = 0;
            
            Moteur_ON = ! (SIU.PGPDI[2].R & 0x80000000); // Bouton 1
            reset = ! (SIU.PGPDI[2].R & 0x10000000);  // Bouton 4
            
            if(Moteur_ON) 
            {
                SIU.PGPDO[0].R = 0x0000C000;                // Active les 2 moteurs
                SIU.GPDO[69].B.PDO = 0;     // LED 2 ON        
                Moteur_ON = 0;
                moteur_integrale = 0;
                controle_integrale = 0;
            }
            if(Moteur_OFF)
            {
                    SIU.PGPDO[0].R = 0x00000000;                // Désactive les 2 moteurs
                    Moteur_OFF = 0;
            }
            
            // gestion de la camera
            Acquisition_Camera();
            // traitement des signaux
            camera_valeurs_filtrees_1 = Filtrer_Valeurs_Camera(1);
            camera_valeurs_filtrees_2 = Filtrer_Valeurs_Camera(2);
            // detection du milieu de la ligne avec les valurs jutes récupérées
           	milieu_camera_1_old = milieu_camera_1;
           	milieu_camera_1 =  milieu_ligne(camera_valeurs_filtrees_1);
           	milieu_camera_2_old = milieu_camera_2;
           	milieu_camera_2 =  milieu_ligne(camera_valeurs_filtrees_2);
           
           	// asservissement en direction
           	commande_base = pos_milieu_servo;
           	commande_orientation_proche = get_commande_orientation(milieu_camera_2,milieu_camera_2_old);
           	commande_finale = combiner(commande_base, commande_orientation_proche, milieu_camera_1);
           	// application de la commande
           	EMIOS_0.CH[4].CBDR.R = ((uint16_t) commande_finale);
           	
           	// asserv en vitesse
           	if(Moteur_F < 5)
                Moteur_F++;
            else
            {
            	objectif_vitesse = 2;
                Asserv_Vitesse();
                Moteur_F = 0;
            }
            
			
            //while(! main_fin_boucle) asm("wait"); // attend l'interruption sur timer pour redemarer un tour de boucle // Evite de revenir dans la boucle quand il y a des interruptions sur le capteur de vitesse
            //main_fin_boucle = 0;
            
            SIU.GPDO[68].B.PDO = !	SIU.GPDO[68].B.PDO;

        }
        while(! reset );
        delay(10000000);
    }	
}


			