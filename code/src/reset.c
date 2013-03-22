#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "reset.h"

void reload(void) {  
	  
    uint8_t milieu, incertitude;
	
    SIU.GPDO[71].B.PDO = 0; // LED4 ON   

    do
    {
		// Balance des blancs (Bouton1 enfonce):
        if ((SIU.PGPDI[2].R & 0x80000000) == 0x00000000)
        {
	    	Acquisition_Camera(1); // on fait la balance des blancs
   	        SIU.GPDO[68].B.PDO = 0;
        }        
        // Reglage du focus de la camera (Bouton2 enfonce):
        else if ((SIU.PGPDI[2].R & 0x40000000) == 0x00000000)
        {
            Acquisition_Camera(0);
		    milieu_ligne(&milieu, &incertitude);
			if(incertitude < 20 )
   	            SIU.GPDO[70].B.PDO = 0;
			else SIU.GPDO[70].B.PDO = 1;
			if(incertitude < 100 )
			    SIU.GPDO[69].B.PDO = 0;
			else SIU.GPDO[69].B.PDO = 1;   
  	    }
  	    asm("wait");
        SIU.GPDO[68].B.PDO = 1; //LED1 OFF
    }
    while(SIU.PGPDI[2].R & 0x10000000);
        
    SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
    delay(10000000);
    SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
}
