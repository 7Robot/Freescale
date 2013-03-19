#include "Capteurs_IR.h"

void acquisitionLineSensors(void)
{
	
	uint32_t Counter_value = EMIOS_0.CH[16].CCNTR.B.CCNTR;

	SIU.PGPDO[1].B.PPD0 = 0xfe000000; //3f810000;   // Pins level HIGH

    while(((EMIOS_0.CH[16].CCNTR.B.CCNTR - Counter_value)%EMIOS_0.CH[16].CADR.R) < 10) {}   // Charging the sensor for 10 us
	
	SIU.PCR[32].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[33].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[34].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[35].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[36].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[37].R = 0x0100;        // Pin set as an  INPUT
	SIU.PCR[38].R = 0x0100;        // Pin set as an  INPUT
	
	Capteurs_resultat=0b11111111;
	
	while(EMIOS_0.CH[16].CCNTR.B.CCNTR < 1525)// Temps total d'acquisition
	{
		if(SIU.GPDI[40].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 500)//798
		{
			Capteurs_resultat &= 0b11101111;//Middle sensor
		}
		if(SIU.GPDI[35].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 500)
		{
			Capteurs_resultat &= 0b11110111; //Right_1 

		}
		if(SIU.GPDI[38].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 1523)
		{
			Capteurs_resultat &= 0b11011111;//Left_1
		}
		if(SIU.GPDI[39].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 500)
		{
			Capteurs_resultat &= 0b11111011;//Right_2
		}
		if(SIU.GPDI[36].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 686)
		{
			Capteurs_resultat &= 0b10111111;//Left_2
		}
		if(SIU.GPDI[37].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 686)
		{
			Capteurs_resultat &= 0b11111101;//Right_3
		}
		
		if(SIU.GPDI[34].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< 686)//686
		{
			Capteurs_resultat &= 0b01111111;//Left_3
		}
		/*if(SIU.GPDI[47].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< sensibility )//893)
		{			
			
				SIU.PGPDO[2].R &= 0x0e000000;		/* Enable LED4*/
		//}*/
					
	}
	
	SIU.PCR[32].R = 0x0200;        // Pin set as an  OUTPUT  // PC[0]
	SIU.PCR[33].R = 0x0200;        // Pin set as an  OUTPUT  // PC[1]
	SIU.PCR[34].R = 0x0200;        // Pin set as an  OUTPUT  // PC[2]
	SIU.PCR[35].R = 0x0200;        // Pin set as an  OUTPUT  // PC[3]
	SIU.PCR[36].R = 0x0200;        // Pin set as an  OUTPUT  // PC[4]
	SIU.PCR[37].R = 0x0200;        // Pin set as an  OUTPUT  // PC[5]
	SIU.PCR[38].R = 0x0200;        // Pin set as an  OUTPUT  // PC[6]
	
}
