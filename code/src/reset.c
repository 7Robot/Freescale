#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "reset.h"

void reload(void)
{    
    interruptionCamera(1);  // on fait la balance des blancs
    //SIU.PGPD0[2].R & = 0xf0f

    do
    {
            
    }
    while(SIU.PGPDI[2].R & 0x10000000);
        
    SIU.PGPDO[2].R &= 0xf0ffffff; // Enable all leds
    delay(10000000);
    SIU.PGPDO[2].R |= 0x0f000000; // Disable all leds  	  
}
