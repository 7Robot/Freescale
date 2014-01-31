#include "extern_globals.h"
#include "delay.h"
#include "camera.h"

/* revoir l'ordre des etapes !!
void Aquisition_Camera_Interrupt(void)
{
	if (i_camera_interrupt == 0)  
	{
		SIU.PGPDO[1].R &= ~0x0000000C;          // mets les 2 pattes (Clk et Start) à 0 All port line low 
   		SIU.PGPDO[1].R |= 0x00000008;           // mets start read à 1 Sensor read start High 
	}
	else if (i_camera_interrupt & 0x000001 == 1)
	{
		 SIU.PGPDO[1].R |= 0x00000004;            // clk à 1 Clock High 
	}
	else if (i_camera_interrupt == 2)
	{
   	    SIU.PGPDO[1].R &= ~0x00000008;          // start à 0
    	SIU.PGPDO[1].R &= ~0x00000004;          // clk à 0
	}
	else if (i_camera_interrupt < 260)  // 260 = 4 (i-start) + 128*2
	{
		SIU.PGPDO[1].R |= 0x00000004;   // clk à 1
        ADC.MCR.B.NSTART=1;             // launch adc conversion  Trigger normal conversions for ADC0 
	}
	else 
	{
		//desactiver ces interruptions
	}
	
	if (i_camera_inerrupt == 260) i_camera_interrupt = 0;
	else i_camera_interrupt ++;
	
}

void ADC_Interupt(void)
{
	static uint16_t valeurs_camera[128];
	valeurs_camera[i_camera_interupt/2] = ADC.CDR[14].B.CDATA; // range la sortie de l'ADC dans 
}

*/


void Acquisition_Camera(void)
{
    uint8_t i;

    /* Code original */
    //SIU.PGPDO[0].R &= ~0x00000014;          /* All port line low */
    //SIU.PGPDO[0].R |= 0x00000010;           /* Sensor read start High */
    //delay(250);
    //SIU.PGPDO[0].R |= 0x00000004;           /* Sensor Clock High */
    //delay(250);
    //SIU.PGPDO[0].R &= ~0x00000010;          /* Sensor read start Low */ 
    //delay(250);
    //SIU.PGPDO[0].R &= ~0x00000004;          /* Sensor Clock Low */
    //delay(250);
    
    /* En passant directement sur la carte mère */
    SIU.PGPDO[1].R &= ~0x0000000C;          /* All port line low */
    SIU.GPDO[38].B.PDO = 0;	//start cam2
    SIU.GPDO[36].B.PDO = 0;	//clk cam2
    
    SIU.PGPDO[1].R |= 0x00000008;           /* Sensor read start High */
    SIU.GPDO[38].B.PDO = 1;	//start cam2
    delay(250);
    SIU.PGPDO[1].R |= 0x00000004;           /* Sensor Clock High */
    SIU.GPDO[36].B.PDO = 1;	//clk cam2
    delay(250);
    SIU.PGPDO[1].R &= ~0x00000008;          /* Sensor read start Low */ 
    SIU.GPDO[38].B.PDO = 0;	//start cam2
    delay(250);
    SIU.PGPDO[1].R &= ~0x00000004;          /* Sensor Clock Low */
    SIU.GPDO[36].B.PDO = 0;	//clk cam2
    delay(250);
    for (i=0;i<128;i++)
    {
        delay(250);
        SIU.PGPDO[1].R |= 0x00000004;   /* Sensor Clock High */
    	SIU.GPDO[36].B.PDO = 1;	//clk cam2
        ADC.MCR.B.NSTART=1;             /* Trigger normal conversions for ADC0 */
        while (ADC.MCR.B.NSTART == 1) {};
        //adcdata = ADC.CDR[0].B.CDATA; // En passant par la carte de puissance...
        camera_valeurs_brutes1[127-i] = (uint16_t)ADC.CDR[4].B.CDATA; // Mettre la sortie de la camera sur PD[0]
        camera_valeurs_brutes2[i] = (uint16_t)ADC.CDR[14].B.CDATA; // Mettre la sortie de la camera sur PD[10]
         
        delay(250);
        SIU.PGPDO[1].R &= ~0x00000004;  /* Sensor Clock Low */
	    SIU.GPDO[36].B.PDO = 0;	//clk cam2

    }
}

uint16_t* Filtrer_Valeurs_Camera(int camera) 
{
	uint16_t* valeurs_brutes;
	if (camera == 1)
	{
		valeurs_brutes = camera_valeurs_brutes1;
	}
	else
	{
		valeurs_brutes = camera_valeurs_brutes2;
	}
	return valeurs_brutes;
}
