#include "Capteurs_IR.h"

void acquisitionLineSensors(void)
{
	
	uint32_t Counter_value_start = EMIOS_0.CH[16].CCNTR.B.CCNTR;
	uint32_t Counter_value_tmp = 0;
	uint8_t i = 0;
	uint8_t capteur_resultat[7];
	uint8_t seuil_capteur[7];
	uint8_t k;
	
	while(i<7) // Boucle de test pour chaque Capteur
	{	
		seuil_capteur[i] = potent_entre(400,1600); // On lit sur le potent le seuil du capteur i
		// Affichage textuel en xbee
		TransmitData("\n\nOn teste le capteur :");
		TransmitCharacter(i);
		TransmitData("\nLe temps est : ");
		TransmitCharacter(seuil_capteur[i]);
		TransmitData("\nLe capteur detecte :");
		PIT_Enable_Channel(1);
		do
		{
			SIU.PGPDO[1].B.PPD0 = 0xfe000000; //3f810000;   // Pins level HIGH
		}while(PIT.CH[6].CVAL.R < 9 );
		PIT_Disable_Channel(1); // Charging the sensor for 10 us
	
		SIU.PCR[32].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[33].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[34].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[35].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[36].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[37].R = 0x0100;        // Pin set as an  INPUT
		SIU.PCR[38].R = 0x0100;        // Pin set as an  INPUT
		
		// Réinitialisation des valeurs lues sur les capteurs
		for(k = 0; k < 7; k++)
		{
			capteur_resultat[k] = 1;
		}
		Counter_value_start = EMIOS_0.CH[16].CCNTR.B.CCNTR;
		
		while(((Counter_value_tmp - Counter_value_start) % EMIOS_0.CH[16].CADR.R) < 2000)// Temps total d'acquisition // On fait l'aquisition <1600 et on fait varier le potentiometre entre 400 et 1600
		{
			if(SIU.GPDI[32].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[0])//798
			{
				capteur_resultat[0] = 0;
			}
			if(SIU.GPDI[33].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[1])
			{
				capteur_resultat[1] = 0;

			}
			if(SIU.GPDI[34].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[2])
			{
				capteur_resultat[2] = 0;
			}
			if(SIU.GPDI[35].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[3])
			{
				capteur_resultat[3] = 0;
			}
			if(SIU.GPDI[35].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[4])
			{
				capteur_resultat[4] = 0;
			}
			if(SIU.GPDI[36].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[5])
			{
				capteur_resultat[5] = 0;
			}
		
			if(SIU.GPDI[37].B.PDI != 1 && EMIOS_0.CH[16].CCNTR.B.CCNTR< seuil_capteur[6])//686
			{
				capteur_resultat[6] = 0;
			}
			
			Counter_value_tmp = EMIOS_0.CH[16].CCNTR.B.CCNTR;
					
		}
	
		// Affichage du résultat du capteur courant i
		if(capteur_resultat[i])
		{
			TransmitData(" Blanc");
		}
		else
	    	TransmitData(" Noir");
	
		SIU.PCR[32].R = 0x0200;        // Pin set as an  OUTPUT  // PC[0]
		SIU.PCR[33].R = 0x0200;        // Pin set as an  OUTPUT  // PC[1]
		SIU.PCR[34].R = 0x0200;        // Pin set as an  OUTPUT  // PC[2]
		SIU.PCR[35].R = 0x0200;        // Pin set as an  OUTPUT  // PC[3]
		SIU.PCR[36].R = 0x0200;        // Pin set as an  OUTPUT  // PC[4]
		SIU.PCR[37].R = 0x0200;        // Pin set as an  OUTPUT  // PC[5]
		SIU.PCR[38].R = 0x0200;        // Pin set as an  OUTPUT  // PC[6]
	
		if(SIU.PGPDI[2].R & 0x40000000) // SI le bouton 2 est appuyé on passe au capteur suivant
		{
			i++;
			delay(1000000);
		}
	
	}
	
}
