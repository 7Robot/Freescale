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
   
    /*******************************************
    Tps acquisition cam + traitement = 4,2 ms
    Tps de mise à jour du servo = 20 ms
    Tps de la boucle principale: main_timer_period 
    
    Sources d'interruption:
    
        PIT: Channel 0 -> Boucle principale (10ms)
        
        EMIOS: Emios0.Channel 11 -> Compteur de front montant
                            
    *******************************************/
   
    main_timer_period = 0.01; // Boucle principale: 0.010 s
  
    init();
  
    // moteurs
    moteur_derniere_erreur = 0;
    moteur_integrale = 0;
   
    /************ Scheduled algorithm **********/

 
    while(1)
    {
  
        // Ici est le code de reset
        reload();
    SIU.PGPDO[0].R = 0x00000000;		// Desactive les 2 moteurs
        do
        {
            /* Boutons de controle */
            main_fin_boucle = 0;
            
            Moteur_ON = ! (SIU.PGPDI[2].R & 0x40000000); // Bouton 2
            reset = ! (SIU.PGPDI[2].R & 0x20000000);  // Bouton 3
            
            if(Moteur_ON) 
            {
                SIU.PGPDO[0].R = 0x0000C000;		// Active les 2 moteurs
                SIU.GPDO[69].B.PDO = 0;     // LED 2 ON	
                Moteur_ON = 0;
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
            /*
		    if(Moteur_F < 7)
                Moteur_F++;
            else
            {
                Controle_Vitesse();
                Moteur_F = 0;
            }*/           
            do
            {
                asm("wait");
            }
            while(! main_fin_boucle); // Evite de revenir dans la boucle quand il y a des interruptions sur le capteur de vitesse
            
            

        }
        while(! reset );

    }	
}
