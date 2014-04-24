#include "extern_globals.h"
#include "MPC5604B_M27V.h"
#include "camera.h"
#include "coeff_cam.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)

uint16_t Acquisitions_Cameras()
{
    uint8_t i;
    uint16_t max_cam1 = 0;
    
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
	
	// fist acquiz pour pr�charger le condo dans l'ADC...
	ADC.MCR.B.NSTART=1; 
	while (ADC.MCR.B.NSTART);
	i = (ADC.CDR[40].B.CDATA == 0);
	i = (ADC.CDR[41].B.CDATA == 0);
	
	// s�quence de r�cup�ration des bits
	for (i = 0; i < 128; i++)
	{
		SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
		ADC.MCR.B.NSTART=1;                     				// d�marre la s�rie de conversions (cam1 et cam2)
		delay(DELAY_CLK_CAM);
		while (ADC.MCR.B.NSTART);								// v�rifie que les 2 conversion sont finies
		SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;				// CLK = 1

		camera1_valeurs[i] = ADC.CDR[40].B.CDATA;					// on r�cup�re les donn�es converties par l'ADC
		max_cam1 = max(max_cam1, camera1_valeurs[i]);					// on prends en meme temps le max de luminosit� de la cam�ra pour lasserv des leds
		camera2_valeurs[i] = ADC.CDR[41].B.CDATA;						// la premi�re CAM est mont�e en inverse de la premi�re => il faudra inverser les indices
		
		delay(DELAY_CLK_CAM);		
	}
	
	// remise de la clock � 0 � la fin de la s�quence
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
	//delay(DELAY_CLK_CAM);
	//SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;				// CLK = 1
	//delay(DELAY_CLK_CAM);
	//SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
    // old version avec 1 seule cam�ra

 	return max_cam1;    
}

void asserv_leds(uint16_t max_lum)
{
	static float consigne_led = 0;
	
	// mix entre un int�grale et un proportionnel
	consigne_led += (consigne_lum - max_lum) * k_lum;
	
	if (consigne_led >= 95)
		consigne_led = 95;
	else if (consigne_led < 0)
		consigne_led = 0;
	
	if (!mode_led)
		Set_PWM_Leds (led_power);
	else
		Set_PWM_Leds (consigne_led);
		
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

void coeffs_moy_cam (uint8_t do_coeffs)
{
	uint8_t i;
	uint16_t moy;
	uint16_t moy_c;
	
	// passage pour la cam�ra 1
	// moyenne par circulairesur 5 pixels
	moy = 3*camera1_valeurs[0] + camera1_valeurs[1] + camera1_valeurs[2];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[0])) >> 7);
	else
		moy_c = moy;
	camera1_valeurs_t[0] = moy_c;
	max_moy1 = moy_c;
	min_moy1 = moy_c;
	
	moy = moy - camera1_valeurs[0] + camera1_valeurs[3];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[1])) >> 7);
	else
		moy_c = moy;
	camera1_valeurs_t[1] = moy_c;
	max_moy1 = max(max_moy1, moy_c);
	min_moy1 = min(min_moy1, moy_c);
	
	moy = moy - camera1_valeurs[0] + camera1_valeurs[4];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[2])) >> 7);
	else
		moy_c = moy;
	camera1_valeurs_t[2] = moy_c;
	max_moy1 = max(max_moy1, moy_c);
	min_moy1 = min(min_moy1, moy_c);
	
	for (i = 3; i <= 125; i ++)
	{
		moy = moy - camera1_valeurs[i-3] + camera1_valeurs[i+2];
		if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[i])) >> 7);
		else
			moy_c = moy;
		camera1_valeurs_t[i] = moy_c;
		max_moy1 = max(max_moy1, moy_c);
		min_moy1 = min(min_moy1, moy_c);
	}
	
	moy = moy - camera1_valeurs[123] + camera1_valeurs[127];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[126])) >> 7);
	else
		moy_c = moy;
	camera1_valeurs_t[126] = moy_c;
	max_moy1 = max(max_moy1, moy_c);
	min_moy1 = min(min_moy1, moy_c);
	
	moy = moy - camera1_valeurs[124] + camera1_valeurs[127];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[127])) >> 7);
	else
		moy_c = moy;
	camera1_valeurs_t[127] = moy_c;
	max_moy1 = max(max_moy1, moy_c);
	min_moy1 = min(min_moy1, moy_c);
	
	
	



	// camera 2
	moy = 3*camera2_valeurs[0] + camera2_valeurs[1] + camera2_valeurs[2];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[0])) >> 7);
	else
		moy_c = moy;
	camera2_valeurs_t[0] = moy_c;
	max_moy2 = moy_c;
	min_moy2 = moy_c;
	
	moy = moy - camera2_valeurs[0] + camera2_valeurs[3];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[1])) >> 7);
	else
		moy_c = moy;
	camera2_valeurs_t[1] = moy_c;
	max_moy2 = max(max_moy2, moy_c);
	min_moy2 = min(min_moy2, moy_c);
	
	moy = moy - camera2_valeurs[0] + camera2_valeurs[4];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[2])) >> 7);
	else
		moy_c = moy;
	camera2_valeurs_t[2] = moy_c;
	max_moy2 = max(max_moy2, moy_c);
	min_moy2 = min(min_moy2, moy_c);
	
	for (i = 3; i <= 125; i ++)
	{
		moy = moy - camera2_valeurs[i-3] + camera2_valeurs[i+2];
		if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[i])) >> 7);
		else
			moy_c = moy;
		camera2_valeurs_t[i] = moy_c;
		max_moy2 = max(max_moy2, moy_c);
		min_moy2 = min(min_moy2, moy_c);
	}
	
	moy = moy - camera2_valeurs[123] + camera2_valeurs[127];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[126])) >> 7);
	else
		moy_c = moy;
	camera2_valeurs_t[126] = moy_c;
	max_moy2 = max(max_moy2, moy_c);
	min_moy2 = min(min_moy2, moy_c);
	
	moy = moy - camera2_valeurs[124] + camera2_valeurs[127];
	if (do_coeffs)
		moy_c = (uint16_t)(((uint32_t)(moy * coeffs_cam[127])) >> 7);
	else
		moy_c = moy;
	camera2_valeurs_t[127] = moy_c;
	max_moy2 = max(max_moy2, moy_c);
	min_moy2 = min(min_moy2, moy_c);
}

void delay(uint32_t nb_tours)
{
    uint32_t i;
    for(i = 0; i < nb_tours; i++) {};
}

