#include "extern_globals.h"
#include "MPC5604B_M27V.h"
#include "camera.h"

void Acquisitions_Cameras()
{
    uint8_t i;
    
    /* rappel des pattes conect�es
    SIU.PCR[82].R = 0x0200;				// PF[2]  = SI  cam 1 
	SIU.PCR[83].R = 0x0200;				// PF[3]  = SI  cam 2
	SIU.PCR[84].R = 0x0200;				// PF[4]  = CLK cam 1 
	SIU.PCR[85].R = 0x0200;				// PF[5]  = CLK cam 2 */
	
	
	//SIU.PGPD[2].R = PORT[E] & PORT[F];// mappage du port pour visualisation
	
	// d�but de la s�quence : coup de clock avec bit de start (SI)
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~(BITS_SI | BITS_CLK);	// mise � 0 de tous les bits
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_SI;					// start = 1
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;					// CLK = 1
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_SI;					// start = 0
	delay(DELAY_CLK_CAM/2);
	
	// s�quence de r�cup�ration des bits
	for (i = 0; i < 128; i++)
	{
		SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
		ADC.MCR.B.NSTART=1;                     				// d�marre la s�rie de conversions (cam1 et cam2)
		delay(DELAY_CLK_CAM);
		while (ADC.MCR.B.NSTART);								// v�rifie que les 2 conversion sont finies
		SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;				// CLK = 1
		camera1_valeurs[127-i] = ADC.CDR[40].B.CDATA;					// on r�cup�re les donn�es converties par l'ADC
		camera2_valeurs[i] = ADC.CDR[41].B.CDATA;						// la premi�re CAM est mont�e en inverse de la premi�re => il faut inverser les indices
		delay(DELAY_CLK_CAM);		
	}
	
	// remise de la clock � 0 � la fin de la s�quence
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
	
    // old version avec 1 seule cam�ra
   /* // En passant directement sur la carte m�re 
    SIU.PGPDO[1].R &= ~0x0000000C;          // All port line low 
    SIU.PGPDO[1].R |= 0x00000008;           // Sensor read start High 
    delay(250);
    SIU.PGPDO[1].R |= 0x00000004;           // Sensor Clock High 
    delay(250);
    SIU.PGPDO[1].R &= ~0x00000008;          // Sensor read start Low  
    delay(250);
    SIU.PGPDO[1].R &= ~0x00000004;          // Sensor Clock Low 
  
    delay(250);
    
    for (i=0;i<128;i++)
    {
        delay(250);
        SIU.PGPDO[1].R |= 0x00000004;   // Sensor Clock High 
        ADC.MCR.B.NSTART=1;                     // Trigger normal conversions for ADC0 
        while (ADC.MCR.B.NSTART == 1) {};
        adcdata = ADC.CDR[14].B.CDATA; // Mettre la sortie de la camera sur PD[10] 
        delay(250);
        SIU.PGPDO[1].R &= ~0x00000004;  // Sensor Clock Low 
        //if(balance_des_blancs)
        //    camera_valeurs_blanc[i] = adcdata;
        //else
            camera_valeurs[i] = adcdata ; 
        // j'ai enlev� cette division
        // je divise par deux pour �viter un overflow
		// deux ou quatre?
     }*/
     
}

// attends une valeur en %
void Set_PWM_Leds(float consigne)
{
	if (consigne > 100.0)
		consigne = 100;
	else if (consigne < 0)
		consigne = 0;
	
	// valeur max : 1000 => * 10
	EMIOS_1.CH[11].CBDR.R = (uint16_t)(consigne * 10);
}

void delay(uint32_t nb_tours)
{
    uint32_t i;
    for(i = 0; i < nb_tours; i++) {};
}

