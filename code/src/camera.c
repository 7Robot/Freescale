#include "extern_globals.h"
#include "delay.h"
#include "camera.h"

void Acquisition_Camera(uint8_t balance_des_blancs)
{
    uint8_t i;
    uint32_t adcdata;
    static uint8_t camera_valeurs_blanc[128];

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
        //adcdata = ADC.CDR[0].B.CDATA; // En passant par la carte de puissance...
        adcdata = ADC.CDR[14].B.CDATA; // Mettre la sortie de la camera sur PD[10] 
        delay(250);
        SIU.PGPDO[0].R &= ~0x00000004;  /* Sensor Clock Low */
        if(balance_des_blancs)
            camera_valeurs_blanc[i] = (adcdata >> 2);
        else
            camera_valeurs[i] = ((int16_t)camera_valeurs_blanc[i] - (int16_t)(adcdata >> 2)) / 2; // je divise par deux pour éviter un overflow
    }
}
