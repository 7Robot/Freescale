#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "reset.h"

void reload(void) {  
	  
    uint8_t milieu, incertitude;
    uint32_t pos_servo = 1200;
	
    SIU.GPDO[71].B.PDO = 0; // LED4 ON
    
    // On coupe les moteurs:
    SIU.PGPDO[0].R = 0x00000000;		// Active les 2 moteurs
    

    do
    {
		// Balance des blancs (Bouton1 enfonce):
        if ((SIU.PGPDI[2].R & 0x80000000) == 0x00000000)
        {
	    	Acquisition_Camera(1); // on fait la balance des blancs
            /* REGLAGE DU SERVO
            pos_servo = potent_entre(700,1500);
            EMIOS_0.CH[4].CBDR.R = pos_servo;
            printhex32(pos_servo);*/
        }        
        // Reglage du focus de la camera (Bouton2 enfonce):
        else if ((SIU.PGPDI[2].R & 0x40000000) == 0x00000000)
        {
            Acquisition_Camera(0);
		    milieu_ligne(&milieu, &incertitude);
			if (incertitude < 10 )
   	            SIU.GPDO[70].B.PDO = 0;
			else SIU.GPDO[70].B.PDO = 1;
			if (incertitude < 14 )
			    SIU.GPDO[69].B.PDO = 0;
			else SIU.GPDO[69].B.PDO = 1;   
			if (incertitude < 18 )
			    SIU.GPDO[68].B.PDO = 0;
			else SIU.GPDO[68].B.PDO = 1; 
  		}
        // Detection ligne d'arrivé (Bouton3 enfonce):
        else if ((SIU.PGPDI[2].R & 0x20000000) == 0x00000000)
        {
            Acquisition_Camera(0);
            if (code_arret_cam())
   	            SIU.GPDO[70].B.PDO = 0;                
  	    }  	
    
  	 asm("wait");
         SIU.GPDO[68].B.PDO = 1;
         SIU.GPDO[69].B.PDO = 1;
         SIU.GPDO[70].B.PDO = 1;

    }
    while(SIU.PGPDI[2].R & 0x10000000);
        
    SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
    delay(10000000);
    SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
}
