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
        reset();

        do
        {
            /* Boutons de controle */
            main_fin_boucle = 0;
            interrupteur_balance_des_blancs =  ! (SIU.PGPDI[2].R & 0x80000000); //Bouton 1
            Moteur_ON = ! (SIU.PGPDI[2].R & 0x40000000); // Bouton 2
            reset = ! (SIU.PGPDI[2].R & 0x20000000);  // Bouton 3
        
            if(interrupteur_balance_des_blancs)
                SIU.GPDO[68].B.PDO = 0; // LED 1
            
            if(Moteur_ON) 
            {
                SIU.PGPDO[0].R = 0x0000C000;		// Active les 2 moteurs
                SIU.GPDO[69].B.PDO = 0;     // LED 2		
<<<<<<< HEAD
=======
                //Moteurs en série:
                //EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + 800;// Moteurs en serie
                //Moteurs en parallèle : 
                //EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + 350;//HBridge gauche
                //EMIOS_0.CH[7].CBDR.R = EMIOS_0.CH[7].CADR.R + 350;//HBridge Droit
>>>>>>> 1be41b8831ef494f2fddd0eaca1613c7267e8a9d
                Moteur_ON =0;
            }
            
            interruptionCamera(false);
            
            interruptionControle();
            
            //SIU.GPDO[42].B.PDO = 1; // Freinage acif, activation de IN1 sur les Ponts-en-H cf schematique carte de puissance
            /*delay(100);
            SIU.GPDO[42].B.PDO = 1;*/
           
            if(Servo_F < 2) Servo_F += 1;
                else 
                {
                    Servo_F = 0;
                    interruptionControle();	
                }
		
            do
            {
            asm("wait");
            }while(! main_fin_boucle); // Evite de revenir dans la boucle quand il y a des interruptions sur le capteur de vitesse
            */

        }
        while(! reset );

    }	
}
