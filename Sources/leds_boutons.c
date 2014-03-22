#include "leds_boutons.h";

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