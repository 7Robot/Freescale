#include "MPC5604B_M27V.h"

// Fait tourner le servo sur toute l'amplitude possible
	void SERVO(void); 

// Permet de r�gler les roues avec le potentiom�tre 
	void reglerPotentio();
	
void MOTOR_LEFT(void);
void MOTOR_RIGHT(void);
void RIGHT_MOTOR_CURRENT(void);
void LEFT_MOTOR_CURRENT(void);

void boutonLed(); //Allume une Leds en fonction du bouton appuy�

