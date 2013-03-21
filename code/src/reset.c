#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "globals.h"
#include "extern_globals.h"

int reset(void) {    
	
	interruptionCamera(true);  // on fait la balance des blancs
        //SIU.PGPD0[2].R & = 0xf0f

        do
        {
            
s
        }
        while(SIU.PGPDI[2].R & 0x10000000);
        
        SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
        delay(10000000);
        SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
}
