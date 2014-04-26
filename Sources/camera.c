#include "extern_globals.h"
#include "MPC5604B_M27V.h"
#include "camera.h"
#include "leds_boutons.h"
//#include "coeff_cam.h"
#include "un_sur.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)


uint16_t Acquisitions_Cameras()
{
    uint8_t i;
    uint16_t max_cam1 = 0;
    
    /* rappel des pattes conectées
    SIU.PCR[82].R = 0x0200;				// PF[2]  = SI  cam 1 
	SIU.PCR[83].R = 0x0200;				// PF[3]  = SI  cam 2
	SIU.PCR[84].R = 0x0200;				// PF[4]  = CLK cam 1 
	SIU.PCR[85].R = 0x0200;				// PF[5]  = CLK cam 2 */
	
	
	//SIU.PGPD[2].R = PORT[E] & PORT[F];// mappage du port pour visualisation
	
	
	// début de la séquence : coup de clock avec bit de start (SI)
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~(BITS_SI | BITS_CLK);	// mise à 0 de tous les bits
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_SI;					// start = 1
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;					// CLK = 1
	delay(DELAY_CLK_CAM/2);
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_SI;					// start = 0
	delay(DELAY_CLK_CAM/2);
	
	// fist acquiz pour précharger le condo dans l'ADC...
	ADC.MCR.B.NSTART=1; 
	while (ADC.MCR.B.NSTART);
	i = (ADC.CDR[40].B.CDATA == 0);
	i = (ADC.CDR[41].B.CDATA == 0);
	
	// séquence de récupération des bits
	for (i = 0; i < 128; i++)
	{
		SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
		ADC.MCR.B.NSTART=1;                     				// démarre la série de conversions (cam1 et cam2)
		delay(DELAY_CLK_CAM);
		while (ADC.MCR.B.NSTART);								// vérifie que les 2 conversion sont finies
		SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;				// CLK = 1

		camera1_valeurs[i] = ADC.CDR[41].B.CDATA;					// on récupère les données converties par l'ADC
		max_cam1 = max(max_cam1, camera1_valeurs[i]);					// on prends en meme temps le max de luminosité de la caméra pour lasserv des leds
		camera2_valeurs[127-i] = ADC.CDR[40].B.CDATA;						// la première CAM est montée en inverse de la première => il faudra inverser les indices
		
		delay(DELAY_CLK_CAM);		
	}
	
	// remise de la clock à 0 à la fin de la séquence
	SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
	//delay(DELAY_CLK_CAM);
	//SIU.PGPDO[2].R = SIU.PGPDO[2].R | BITS_CLK;				// CLK = 1
	//delay(DELAY_CLK_CAM);
	//SIU.PGPDO[2].R = SIU.PGPDO[2].R & ~BITS_CLK;			// CLK = 0
    // old version avec 1 seule caméra

 	return max_cam1;    
}

void asserv_leds(uint16_t max_cam)
{
	// mix entre un intégrale et un proportionnel
	consigne_led += (consigne_lum - max_cam) * k_lum;
	
	if ( consigne_led>= 100)
		 consigne_led= 100;
	else if (consigne_led < 0)
		 consigne_led= 0;
	
	if (!mode_led)
	{
		Set_PWM_Leds (led_power);
	}		
	else
	{
		Set_PWM_Leds (consigne_led);
	}
		
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

void moy_cam (uint8_t do_moy)
{
	uint8_t i;
	uint16_t moy;
	
	if (do_moy)
	{
		// passage pour la caméra 1
		// moyenne par circulairesur 5 pixels
		moy = 3*camera1_valeurs[0] + camera1_valeurs[1] + camera1_valeurs[2];

		camera1_valeurs_m[0] = moy;
		max_moy1 = moy;
		min_moy1 = moy;
		
		moy = moy - camera1_valeurs[0] + camera1_valeurs[3];

		camera1_valeurs_m[1] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		moy = moy - camera1_valeurs[0] + camera1_valeurs[4];

		camera1_valeurs_m[2] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		for (i = 3; i <= 125; i ++)
		{
			moy = moy - camera1_valeurs[i-3] + camera1_valeurs[i+2];

			camera1_valeurs_m[i] = moy;
			max_moy1 = max(max_moy1, moy);
			min_moy1 = min(min_moy1, moy);
		}
		
		moy = moy - camera1_valeurs[123] + camera1_valeurs[127];

		camera1_valeurs_m[126] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		moy = moy - camera1_valeurs[124] + camera1_valeurs[127];

		camera1_valeurs_m[127] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		
		



		// camera 2
		moy = 3*camera2_valeurs[0] + camera2_valeurs[1] + camera2_valeurs[2];
		
		camera2_valeurs_m[0] = moy;
		max_moy2 = moy;
		min_moy2 = moy;
		
		moy = moy - camera2_valeurs[0] + camera2_valeurs[3];
		
		camera2_valeurs_m[1] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		moy = moy - camera2_valeurs[0] + camera2_valeurs[4];
		camera2_valeurs_m[2] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		for (i = 3; i <= 125; i ++)
		{
			moy = moy - camera2_valeurs[i-3] + camera2_valeurs[i+2];
			camera2_valeurs_m[i] = moy;
			max_moy2 = max(max_moy2, moy);
			min_moy2 = min(min_moy2, moy);
		}
		
		moy = moy - camera2_valeurs[123] + camera2_valeurs[127];
		camera2_valeurs_m[126] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		moy = moy - camera2_valeurs[124] + camera2_valeurs[127];
		camera2_valeurs_m[127] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
	}
	else
	{
		max_moy1 = 0;
		max_moy2 = 0;
		min_moy1 = 6000;	// max en sortie de l'ADC : 1023 => * 5 donne 5k et des poussieres => on met 6k
		min_moy2 = 6000;
		// fonction qui ne fait pas la moyenne
		// recopie en mutlipliant par 5 (pour garder une cohérence avec la moyenne)
		for (i = 0; i < 128; i++)
		{
			moy = 5 * camera1_valeurs[i];
			camera1_valeurs_m[127] = moy;
			max_moy1 = max(max_moy1, moy);
			min_moy1 = min(min_moy1, moy);
			
			moy = 5 * camera2_valeurs[i];
			camera2_valeurs_m[127] = moy;
			max_moy2 = max(max_moy2, moy);
			min_moy2 = min(min_moy2, moy);
		}
	}
}

void delay(uint32_t nb_tours)
{
    uint32_t i;
    for(i = 0; i < nb_tours; i++) {};
}


void calcul_courbe(void)
{
	
	uint16_t courbe[128][2];
	float pente_courbe[127];		// pente_courbe(0) = pente entre 0 et 1
	uint8_t index = 0, index_max;
	uint8_t i;
	float pente;
	uint8_t xa, xb, xc;
	uint16_t ya, yb, yc;
	int32_t p1,p2;
	
	
	courbe[0][0] = 0;
	courbe[0][1] = camera1_valeurs_m[0];
	
	courbe[1][0] = 1;
	courbe[1][1] = camera1_valeurs_m[1];
	
	index = 1;
	i = 2;
	
	while(i < 128)
	{
		if (index != 0)
		{
			ya = courbe[index-1][1];
			yb = courbe[index][1];
			yc = camera1_valeurs_m[i];
			xa = courbe[index-1][0];
			xb = courbe[index][0];
			xc = i;
			p2 = (yc - yb) * (xb - xa);		//	pente entre B et C
			p1 = (yb - ya) * (xc - xb);		//	pente entre A et B
			if(p2 < p1)
			{
				index++;
				courbe[index][0] = i;
				courbe[index][1] = yc;
				i++;
			}
			else
			{
				index--;
			}
		}
		else
		{
			index++;
			courbe[1][0] = i;
			courbe[1][1] = camera1_valeurs_m[i];
			i++;
		}
		
	}
	
	i = 0;
	index_max = index;
	index = 0;
	
	while (index < index_max)
	{
		xa = courbe[index][0];
		xb = courbe[index+1][0];
		ya = courbe[index][1];
		yb = courbe[index+1][1];
		
		while (i < xb)
		{
			camera1_courbe[i] = ya + (((yb - ya)*(i - xa)) / (xb - xa));
			i++;
		}
		index++;
	}
	camera1_courbe[127] = yb;
	
	
	
	/**************************************/
	//cam2
	
	courbe[0][0] = 0;
	courbe[0][1] = camera2_valeurs_m[0];
	
	courbe[1][0] = 1;
	courbe[1][1] = camera2_valeurs_m[1];
	
	index = 1;
	i = 2;
	
	while(i < 128)
	{
		if (index != 0)
		{
			ya = courbe[index-1][1];
			yb = courbe[index][1];
			yc = camera2_valeurs_m[i];
			xa = courbe[index-1][0];
			xb = courbe[index][0];
			xc = i;
			p2 = (yc - yb) * (xb - xa);		//	pente entre B et C
			p1 = (yb - ya) * (xc - xb);		//	pente entre A et B
			if(p2 < p1)
			{
				index++;
				courbe[index][0] = i;
				courbe[index][1] = yc;
				i++;
			}
			else
			{
				index--;
			}
		}
		else
		{
			index++;
			courbe[1][0] = i;
			courbe[1][1] = camera2_valeurs_m[i];
			i++;
		}
		
	}
	
	i = 0;
	index_max = index;
	index = 0;
	
	while (index < index_max)
	{
		xa = courbe[index][0];
		xb = courbe[index+1][0];
		ya = courbe[index][1];
		yb = courbe[index+1][1];
		
		while (i < xb)
		{
			camera2_courbe[i] = ya + (((yb - ya)*(i - xa)) / (xb - xa));
			i++;
		}
		index++;
	}
	camera2_courbe[127] = yb;
	
}


void retranche_courbe(void)
{
	uint8_t i;
	uint16_t a,b;
	max_p1 =0;
	max_p2 =0;
	for (i = 0; i < 128; i++)
	{
		a = camera1_courbe[i] - camera1_valeurs_m[i];
		b = camera2_courbe[i] - camera2_valeurs_m[i];
		max_p1 = max(max_p1, a);
		max_p2 = max(max_p2, b);
		camera1_p[i] = a;
		camera2_p[i] = b;
	}
}

void analyse_cam(void)
{
	if (max_p1 > (max_moy1 / 5))
	{
		
	}
	if (max_p2 > (max_moy2 / 5))
	{
		
	}
}