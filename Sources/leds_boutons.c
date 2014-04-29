#include "extern_globals.h"
#include "leds_boutons.h"

uint8_t get_bouton(uint8_t but)
{
	but &= 3;	// modulo 4
	return SIU.GPDI[64+but].B.PDI;
}

uint8_t get_led(uint8_t led)
{
	led &= 3;	// modulo 4 (NTUI)
	return (SIU.GPDO[68+led].B.PDO == 0);
}

void set_led(uint8_t led, uint8_t val)
{
	led &= 3;	// modulo 4
	SIU.GPDO[68+led].B.PDO = (val == 0);
}

void blink_led(uint8_t led)
{
	led &= 3;
	SIU.GPDO[68+led].B.PDO = !SIU.GPDO[68+led].B.PDO;
}

uint16_t get_potar(void)
{
	uint16_t toto;
	ADC.JCMR[0].R = 0x10;		// définie l'acquisition voulue sur l'AN4 (PD0)
	ADC.MCR.B.JSTART = 1;			// déclenche la conversion
	while (ADC.MSR.B.JSTART);		// boucle tant qu'elle est pas finie
	
	toto = ADC.CDR[4].B.CDATA;	// premiere aquiz poubelle => préchargement du condo
	
	ADC.JCMR[0].R = 0x10;		// définie l'acquisition voulue sur l'AN4 (PD0)
	ADC.MCR.B.JSTART = 1;			// déclenche la conversion
	while (ADC.MSR.B.JSTART);		// boucle tant qu'elle est pas finie
	
	return ADC.CDR[4].B.CDATA;
}


uint16_t get_buttons (void)
{
	uint16_t btn[3] = {0};
	uint16_t j;
	uint16_t val_btn = 5;
	
	for (j=0;j<4;j++)
	{
		btn[j] = get_bouton(j);
		if ((btn[j] == 1) && (old_btn[j] != 1))
		{
			val_btn = j;
		}
		old_btn[j] = btn[j];
	}
	return val_btn;
}