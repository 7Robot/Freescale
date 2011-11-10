#include "base.h"

void SERVO(void)
{
//	TransmitData("****Steering Servo Test****\n\r");
	EMIOS_0.CH[4].CBDR.R = 1300;      	/* 1500 Middle */
//	TransmitData("Middle\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1550;        /* 1750 Right Max,*/
//	TransmitData("Right\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1050;        /* 1250 Left Max*/
//	TransmitData("Left\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1300;      	/* 1500 Middle */
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