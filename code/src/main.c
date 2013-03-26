#include "MPC5604B_M27V.h"
#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "globals.h"
#include "extern_globals.h"
#include "reset.h"

int main(void) {

    uint8_t reset = 1, Moteur_F = 0, Servo_F = 0, Moteur_ON = 0, Moteur_OFF = 0;
   
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
    
    // Initialisation variables servo
    pos_min_servo = 0x382;
    pos_max_servo = 0x5e8;
    pos_milieu_servo = 0x4C3;
    
   
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
                SIU.PGPDO[0].R = 0x0000C000;		// Active les 2 moteurs
                SIU.GPDO[69].B.PDO = 0;     // LED 2 ON	
                Moteur_ON = 0;
                moteur_integrale = 0;
                controle_integrale = 0;
            }
            if(Moteur_OFF)
            {
            	SIU.PGPDO[0].R = 0x00000000;		// Désactive les 2 moteurs
            	Moteur_OFF = 0;
            }
            else if(compteur_acquisitions_invalides > 500)
            	Moteur_OFF = 1;
            
            Acquisition_Camera(!(SIU.PGPDI[2].R & 0x40000000)); // bouton 2

            //SIU.GPDO[42].B.PDO = 1; // Freinage acif, activation de IN1 sur les Ponts-en-H cf schematique carte de puissance
            /*delay(100);
            SIU.GPDO[42].B.PDO = 1;*/

            Controle_Direction();
            
		    if(Moteur_F < 5)
                Moteur_F++;
            else
            {
                Asserv_Vitesse();
                Moteur_F = 0;
            }
            
            //do
            //{
            //    asm("wait");
            //}
            //while(! main_fin_boucle); // Evite de revenir dans la boucle quand il y a des interruptions sur le capteur de vitesse
            
            SIU.GPDO[68].B.PDO = !	SIU.GPDO[68].B.PDO;

        }
        while(! reset );
        delay(10000000);

    }	
}
