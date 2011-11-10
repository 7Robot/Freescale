#include "base.h"

/* Techniquement : la position min est limitée non pas par la butée,
					mais par le servo qui n'est pas trop sensé tourner
					
voiture 1 : positions (min, moy, max) : ( 810,  1070,  1330)
											 
voiture 2 : positions (min, moy, max) : ( 420,   660,   900)
											
voiture 3 : positions (min, moy, max) : (1050,  1300,  1550)
											

ATTENTION : La syntaxe est VOITURE_1, VOITURE_2, VOITURE_3 */
	
	
#define VOITURE_3

#ifdef VOITURE_1
	#define PAS_SYM 260
	#define POS_MIN 810
	#define POS_MOY 1070
	#define POS_MAX 1330
#endif

#ifdef VOITURE_2
	#define PAS_SYM 240
	#define POS_MIN 420
	#define POS_MOY 660
	#define POS_MAX 900
#endif
	
#ifdef VOITURE_3
	#define PAS_SYM 250
	#define POS_MIN 1050
	#define POS_MOY 1300
	#define POS_MAX 1550
#endif


void SERVO(void)
{
//	TransmitData("****Steering Servo Test****\n\r");
	EMIOS_0.CH[4].CBDR.R = POS_MOY;
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = POS_MAX;
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = POS_MOY;
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = POS_MIN;
	Delaywait();
}

void MOTOR_LEFT(void)
{
//	TransmitData("****Left Drive Motor Test****\n\r");
	SIU.PCR[16].R = 0x0200;				/* Program the drive enable pin of Left Motor as output*/
	SIU.PGPDO[0].R = 0x00008000;		/* Enable Left the motors */
	Delaywait();
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Left the motors */
}

void MOTOR_RIGHT(void)
{
//	TransmitData("****Right Drive Motor Test****\n\r");
	SIU.PCR[17].R = 0x0200;				/* Program the drive enable pin of Right Motor as output*/
	SIU.PGPDO[0].R = 0x00004000;		/* Enable Right the motors */
	Delaywait();
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Right the motors */
}



void boutonLed()
{
	SIU.PGPDO[2].R |= 0x0f000000;		/* Disable LEDs*/
	if((SIU.PGPDI[2].R & 0x80000000) == 0x00000000)
	{
		SIU.PGPDO[2].R &= 0x07000000;		/* Enable LED1*/
	}
	if((SIU.PGPDI[2].R & 0x40000000) == 0x00000000)
	{
		SIU.PGPDO[2].R &= 0x0b000000;		/* Enable LED2*/
	}
	if((SIU.PGPDI[2].R & 0x20000000) == 0x00000000)
	{
		SIU.PGPDO[2].R &= 0x0d000000;		/* Enable LED3*/
	}
	if((SIU.PGPDI[2].R & 0x10000000) == 0x00000000)
	{
		SIU.PGPDO[2].R &= 0x0e000000;		/* Enable LED4*/
	}
}