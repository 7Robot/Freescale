#include "camera.h"
#include "milieu_ligne.h"
#include "moteur.h"
#include "delay.h"
#include "reset.h"

void reload(void) {  
	  /*
    uint8_t milieu, incertitude;
    uint32_t pos_servo = 1200;
    
	
	
    SIU.GPDO[71].B.PDO = 0; // LED4 ON

    do
    {
         REGLAGE DU SERVO  //(bouton1)
		if (!(SIU.PGPDI[2].R & 0x80000000))
        {
 	        SIU.GPDO[68].B.PDO = 0;    
      
            // Position min (bouton2)
            while (!(SIU.PGPDI[2].R & 0x80000000) && !(SIU.PGPDI[2].R & 0x40000000) 
                && (SIU.PGPDI[2].R & 0x20000000) && (SIU.PGPDI[2].R & 0x10000000))
            {
                pos_servo = potent_entre(700,1600);                
                EMIOS_0.CH[4].CBDR.R = pos_servo;
                pos_min_servo = pos_servo;               
                printhex32(pos_servo);                	
            }
            // Position max (bouton3)
            while (!(SIU.PGPDI[2].R & 0x80000000) && (SIU.PGPDI[2].R & 0x40000000) 
                && !(SIU.PGPDI[2].R & 0x20000000) && (SIU.PGPDI[2].R & 0x10000000))
            {
                pos_servo = potent_entre(700,1500);                
                EMIOS_0.CH[4].CBDR.R = pos_servo;
                pos_max_servo = pos_servo;               
                printhex32(pos_servo);                	
            } 
            // Position milieu (bouton4)
            while (!(SIU.PGPDI[2].R & 0x80000000) && (SIU.PGPDI[2].R & 0x40000000) 
                && (SIU.PGPDI[2].R & 0x20000000) && !(SIU.PGPDI[2].R & 0x10000000))
            {
                pos_servo = potent_entre(700,1500);                
                EMIOS_0.CH[4].CBDR.R = pos_servo;
                pos_milieu_servo = pos_servo;               
                printhex32(pos_servo);                	
            }
            printhex32(pos_servo);
                                   
  	    }

         BALANCE DES BLANCS //(bouton2)
        else if (!(SIU.PGPDI[2].R & 0x40000000))
        {
	    	SIU.GPDO[69].B.PDO = 0;
         	Acquisition_Camera(1); // on fait la balance des blancs
        }   
 		
       
         REGLAGE DU FOCUS  //(bouton3):
        else if (!(SIU.PGPDI[2].R & 0x20000000))
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
  		
  	     //asm("wait");
         SIU.GPDO[68].B.PDO = 1;
         SIU.GPDO[69].B.PDO = 1;
         SIU.GPDO[70].B.PDO = 1;


    } // Sortie (Bouton4 enfonce):
    while(!(SIU.PGPDI[2].R & 0x80000000) || !(SIU.PGPDI[2].R & 0x40000000) 
        || !(SIU.PGPDI[2].R & 0x20000000) || (SIU.PGPDI[2].R & 0x10000000));
        
    SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
    delay(10000000);
    SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
    
    */
}
