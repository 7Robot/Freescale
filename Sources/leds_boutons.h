#ifndef __LEDS_BOUTONS__H__
#define __LEDS_BOUTONS__H__


#include "MPC5604B_M27V.h"

uint8_t get_bouton(uint8_t but);
uint8_t get_led(uint8_t led);
void set_led(uint8_t led, uint8_t val);
void blink_led(uint8_t led);

uint16_t get_potar(void);

#endif