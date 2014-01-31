// Fonction d'acquisition du potentiomètre

#include "Potentiometre.h"

uint32_t potentiometer_PD0(void)
{
	uint32_t resultat=0;
	ADC.MCR.B.NSTART=1;
	while (ADC.MCR.B.NSTART==1){}
	resultat = ADC.CDR[13].B.CDATA;
	return resultat;
}

uint32_t potent_entre(uint32_t a, uint32_t b)
{
	uint32_t val=0;
	val = (b-a)*potentiometer_PD0();
	val = val/1024;
	val = val + a;
	return val;
}