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

    uint8_t reset = 1, Moteur_F = 0, Servo_F = 0, Moteur_ON = 0;
   
    /******************************************************
    Tps acquisition cam + traitement = 4,2 ms
    Tps de mise à jour du servo = 20 ms
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
    pos_min_servo = 845;
    pos_max_servo = 1417;
    pos_milieu_servo = 1126;
    
   
    /**************** Scheduled algorithm *****************/

 
    while(1)
    {
  
        // Ici est le code de reset
        SIU.PGPDO[0].R = 0x00000000;		// Desactive les 2 moteurs
        reload();
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
            }
            
            Acquisition_Camera(0);

            //SIU.GPDO[42].B.PDO = 1; // Freinage acif, activation de IN1 sur les Ponts-en-H cf schematique carte de puissance
            /*delay(100);
            SIU.GPDO[42].B.PDO = 1;*/
            
            if(Servo_F < 2)
                Servo_F++;
            else 
            {
                Controle_Direction();
                Servo_F = 0;
            }
		    if(Moteur_F < 7)
                Moteur_F++;
            else
            {
                Asserv_Vitesse();
                Moteur_F = 0;
            }
            do
            {
                asm("wait");
            }
            while(! main_fin_boucle); // Evite de revenir dans la boucle quand il y a des interruptions sur le capteur de vitesse
            
            

        }
        while(! reset );
        delay(10000000);

    }	
}
