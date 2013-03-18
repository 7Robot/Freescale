<<<<<<< HEAD
#include "extern_globals.h"
#include "delay.h"
#include "camera.h"

void interruptionCamera(void)
{
    uint8_t i;
    uint32_t adcdata;
    static int8_t camera_valeurs_blanc[128];
    
    if(interrupteur_balance_des_blancs)
        SIU.PGPDO[2].R &= 0x07000000; // allumer la led

    SIU.PGPDO[0].R &= ~0x00000014;          /* All port line low */
    SIU.PGPDO[0].R |= 0x00000010;           /* Sensor read start High */
    delay(250);
    SIU.PGPDO[0].R |= 0x00000004;           /* Sensor Clock High */
    delay(250);
    SIU.PGPDO[0].R &= ~0x00000010;          /* Sensor read start Low */ 
    delay(250);
    SIU.PGPDO[0].R &= ~0x00000004;          /* Sensor Clock Low */
    delay(250);
    for (i=0;i<128;i++)
    {
        delay(250);
        SIU.PGPDO[0].R |= 0x00000004;   /* Sensor Clock High */
        ADC.MCR.B.NSTART=1;                     /* Trigger normal conversions for ADC0 */
        while (ADC.MCR.B.NSTART == 1) {};
        adcdata = ADC.CDR[0].B.CDATA;
        delay(250);
        SIU.PGPDO[0].R &= ~0x00000004;  /* Sensor Clock Low */
        if(interrupteur_balance_des_blancs)
            *(camera_valeurs_blanc + i) = (int8_t)(adcdata >> 2);
        else
            *(camera_valeurs + i) = *(camera_valeurs_blanc + i) - (int8_t)(adcdata >> 2);
    }
}
=======
#include "extern_globals.h"
#include "delay.h"
#include "camera.h"

void interruptionCamera(void)
{
    uint8_t i;
    uint32_t adcdata;
    int8_t balance_des_blancs = ! (SIU.PGPDI[2].R & 0x80000000);
    static int8_t camera_valeurs_blanc[128];
    
    if(balance_des_blancs)
        SIU.PGPDO[2].R &= 0x07000000; // allumer la led

    SIU.PGPDO[0].R &= ~0x00000014;          /* All port line low */
    SIU.PGPDO[0].R |= 0x00000010;           /* Sensor read start High */
    delay(250);
    SIU.PGPDO[0].R |= 0x00000004;           /* Sensor Clock High */
    delay(250);
    SIU.PGPDO[0].R &= ~0x00000010;          /* Sensor read start Low */ 
    delay(250);
    SIU.PGPDO[0].R &= ~0x00000004;          /* Sensor Clock Low */
    delay(250);
    for (i=0;i<128;i++)
    {
        delay(250);
        SIU.PGPDO[0].R |= 0x00000004;   /* Sensor Clock High */
        ADC.MCR.B.NSTART=1;                     /* Trigger normal conversions for ADC0 */
        while (ADC.MCR.B.NSTART == 1) {};
        adcdata = ADC.CDR[0].B.CDATA;
        delay(250);
        SIU.PGPDO[0].R &= ~0x00000004;  /* Sensor Clock Low */
        if(balance_des_blancs)
            *(camera_valeurs_blanc + i) = (int8_t)(adcdata >> 2);
        else
            *(camera_valeurs + i) = *(camera_valeurs_blanc + i) - (int8_t)(adcdata >> 2);
    }
}
>>>>>>> 8cc3d4b7c04b365f75ff9ec34344faf004f2dc92
