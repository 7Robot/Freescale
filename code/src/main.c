#include "MPC5604B_M27V.h"
#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "globals.h"
#include "extern_globals.h"

int main(void) {

    uint8_t reset = 1, Moteur_F = 0, Servo_F = 0, Moteur_ON = 0,i=0, pot_enable=0;
    uint16_t tempo = 0;
    uint32_t potentiometer = 0;
   
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
  
        // Ici est le code d'init
        // FIXME: Appeler proprement une fonction de reset dans son fichier, qui permetta de régler les choses qui devront être réglées (comme le focus)
        do
        {
            POS_MILIEU_SERVO = potent_entre(700, 1300);  
            pot_enable = !(SIU.PGPDI[2].R & 0x10000000);  // Bouton 4
            EMIOS_0.CH[4].CBDR.R = POS_MILIEU_SERVO; // Euh, la position de milieu des servos est modifiée quand on appelle interruptionControle...
        }
        while(! pot_enable);
        SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
        delay(10000000);
        SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
        SIU.PGPDO[0].R = 0x00000000;		// Desactive les 2 moteurs

        do
        {
            /* Boutons de controle */
            main_fin_boucle = 0;
            interrupteur_balance_des_blancs =  ! (SIU.PGPDI[2].R & 0x80000000); //Bouton 1
            Moteur_ON = ! (SIU.PGPDI[2].R & 0x40000000); // Bouton 2
            reset = ! (SIU.PGPDI[2].R & 0x20000000);  // Bouton 3
            tempo++;
        
            if(interrupteur_balance_des_blancs)
                SIU.GPDO[68].B.PDO = 0; // LED 1
            
            if(Moteur_ON) // FIXME: Pourquoi on exécute ce code à chaque tour quand les moteurs sont allumés ?
            {
                SIU.PGPDO[0].R = 0x0000C000;		// Active les 2 moteurs
                SIU.GPDO[69].B.PDO = 0;     // LED 2		
                EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + 900;
            }
            
            interruptionCamera();
            
            interruptionControle();
            
            //SIU.GPDO[42].B.PDO = 1; // Freinage acif, activation de IN1 sur les Ponts-en-H cf schematique carte de puissance
            /*delay(100);
            SIU.GPDO[42].B.PDO = 1;*/
            /*if(Servo_F < 3) Servo_F += 1;
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
