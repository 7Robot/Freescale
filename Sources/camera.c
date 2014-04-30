#include "extern_globals.h"
#include "MPC5604B_M27V.h"
#include "camera.h"
#include "leds_boutons.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)
#define abs(i) (i >= 0 ? i : (-i))


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
	
	// first acquiz pour précharger le condo dans l'ADC...
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
		// moyenne par circulaire sur 5 pixels
		moy = 3*camera1_valeurs[8] + camera1_valeurs[9] + camera1_valeurs[10];

		camera1_valeurs_m[0] = moy;
		max_moy1 = moy;
		min_moy1 = moy;
		
		moy = moy - camera1_valeurs[8] + camera1_valeurs[11];

		camera1_valeurs_m[1] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		moy = moy - camera1_valeurs[8] + camera1_valeurs[12];

		camera1_valeurs_m[2] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		for (i = 3+8; i <= 125; i ++)
		{
			moy = moy - camera1_valeurs[i-3] + camera1_valeurs[i+2];

			camera1_valeurs_m[i-8] = moy;
			max_moy1 = max(max_moy1, moy);
			min_moy1 = min(min_moy1, moy);
		}
		
		moy = moy - camera1_valeurs[123] + camera1_valeurs[127];

		camera1_valeurs_m[118] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		moy = moy - camera1_valeurs[124] + camera1_valeurs[127];

		camera1_valeurs_m[119] = moy;
		max_moy1 = max(max_moy1, moy);
		min_moy1 = min(min_moy1, moy);
		
		
		



		// camera 2
		moy = 3*camera2_valeurs[8] + camera2_valeurs[9] + camera2_valeurs[10];
		
		camera2_valeurs_m[0] = moy;
		max_moy2 = moy;
		min_moy2 = moy;
		
		moy = moy - camera2_valeurs[8] + camera2_valeurs[11];
		
		camera2_valeurs_m[1] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		moy = moy - camera2_valeurs[8] + camera2_valeurs[12];
		camera2_valeurs_m[2] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		for (i = 3+8; i <= 125; i ++)
		{
			moy = moy - camera2_valeurs[i-3] + camera2_valeurs[i+2];
			camera2_valeurs_m[i-8] = moy;
			max_moy2 = max(max_moy2, moy);
			min_moy2 = min(min_moy2, moy);
		}
		
		moy = moy - camera2_valeurs[123] + camera2_valeurs[127];
		camera2_valeurs_m[118] = moy;
		max_moy2 = max(max_moy2, moy);
		min_moy2 = min(min_moy2, moy);
		
		moy = moy - camera2_valeurs[124] + camera2_valeurs[127];
		camera2_valeurs_m[119] = moy;
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
		for (i = 0; i < 120; i++)
		{
			moy = 5 * camera1_valeurs[i+8];
			camera1_valeurs_m[i] = moy;
			max_moy1 = max(max_moy1, moy);
			min_moy1 = min(min_moy1, moy);
			
			moy = 5 * camera2_valeurs[i];
			camera2_valeurs_m[i] = moy;
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
	
	uint16_t courbe[120][2];
	uint8_t index = 0, index_max;
	uint8_t i;
	uint8_t xa, xb, xc;
	uint16_t ya, yb, yc;
	int32_t p1,p2;
	
	
	courbe[0][0] = 0;
	courbe[0][1] = camera1_valeurs_m[0];
	
	courbe[1][0] = 1;
	courbe[1][1] = camera1_valeurs_m[1];
	
	index = 1;
	i = 2;
	
	while(i < 120)
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
	camera1_courbe[119] = yb;
	
	
	
	/**************************************/
	//cam2
	
	courbe[0][0] = 0;
	courbe[0][1] = camera2_valeurs_m[0];
	
	courbe[1][0] = 1;
	courbe[1][1] = camera2_valeurs_m[1];
	
	index = 1;
	i = 2;
	
	while(i < 120)
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
	camera2_courbe[119] = yb;
	
}


void retranche_courbe(void)
{
	uint8_t i;
	uint16_t a,b;
	max_p1 =0;
	max_p2 =0;
	pos_max_p1 = 0;
	pos_max_p2 = 0;
	
	for (i = 0; i < 120; i++)
	{
		a = camera1_courbe[i] - camera1_valeurs_m[i];
		b = camera2_courbe[i] - camera2_valeurs_m[i];
		
		if (a > max_p1)
		{	max_p1 = a;
			pos_max_p1 = i; 	}
		if (b > max_p2)
		{	max_p2 = b;
			pos_max_p2 = i; 	}
			
		camera1_p[i] = a;
		camera2_p[i] = b;
	}
}

/*
uint8_t analyse_cam(void)
{
	uint8_t i;
	uint8_t pos1A, pos1B, pos2A, pos2B;
	uint16_t seuil1, seuil2;
	uint8_t ligne_stop = 0;
	
	old_milieu1 = milieu1;
	old_milieu2 = milieu2;
	// conditions pour estimer que l'acquisition est bonne 
	// contraste >= 800
	// cad max_moy - min_moy 
	// moy veut dire que c'est la version *5  (d'ou le *5)
	// la profondeur du trou > contraste / 2
	if ((max_p1 > ((max_moy1 - min_moy1) >> 1 )) && (max_moy1 - min_moy1) >= 800 )
	{
		seuil1 = max_p1 >> 1;
		pos1A = pos_max_p1;
		pos1B = pos_max_p1;
		while (camera1_p[pos1A] > seuil1 && pos1A >= 0)
			pos1A --;
		while (camera1_p[pos1B] > seuil1 && pos1B <= 127)
			pos1B ++;
		
	
		milieu1 = (pos1B + pos1A) >> 1;
		
		// detection ligne arret
		seuil1 = max_p1 / 3;
		if (milieu1 > 40 && milieu1 < 87)		// si à peu pres au milieu, on vérifie la présence de la ligne de stop
		{
			if (camera1_p[milieu1 - 40] > seuil1)
				ligne_stop ++;
			if (camera1_p[milieu1 - 35] > seuil1)
				ligne_stop ++;
			if (camera1_p[milieu1 - 30] > seuil1)
				ligne_stop ++;

			if (camera1_p[milieu1 + 40] > seuil1)
				ligne_stop ++;
			if (camera1_p[milieu1 + 35] > seuil1)
				ligne_stop ++;
			if (camera1_p[milieu1 + 30] > seuil1)
				ligne_stop ++;
			
			if (ligne_stop != 6)	// faut absolument que les 6 conditions soeint remplies pour prendre en compte la ligne d'arret => revoir peut être par ici 
				ligne_stop = 0;
			
			pb_aquiz1 = 0;
		}
		
	}
	else
	{
		// milieu1 = milieu1;	// pour l'idée, si on touche pas au milieu, il se garde sa valeur d'avant
		pb_aquiz1 = 1;
	}
	
	if ((max_p2 > ((max_moy2 - min_moy2) >> 1)) && (max_moy2 - min_moy2) >= 800 )
	{
		seuil2 = max_p2 >> 1;
		pos2A = pos_max_p2;
		pos2B = pos_max_p2;
		while (camera2_p[pos2A] > seuil2 && pos2A >= 0)
			pos2A --;
		while (camera2_p[pos2B] > seuil2 && pos2B <= 127)
			pos2B ++;
		
	
		milieu2 = (pos2B + pos2A) >> 1;
	}
	else
	{
		// milieu2 = milieu2;	// pour l'idée, si on touche pas au milieu, il se garde sa valeur d'avant
		pb_aquiz2 = 1;
	}
	
	
	return ligne_stop;
}
*/





void analyse_cam_bis(void)
{
	uint16_t seuil1 = max_p1/3;
	uint16_t seuil2 = max_p2/3;
	int16_t etat_bande = 0;
	int16_t etat_bande_old = 0;
	int16_t i = 0;
	old_milieu1 = milieu1;
	old_milieu2 = milieu2;

	if ((max_p1 > ((max_moy1 - min_moy1) >> 1 )) && (max_moy1 - min_moy1) >= 300 )
	{
		// contsruction des bandes claires sombres
		etat_bande = (camera1_p[0]<seuil1);
		nb_bandes_1 = 1;
		bandes_1[0][0] = etat_bande;
		bandes_1[0][1] = 1;
		for (i=1; i<120; i++)
		{
			etat_bande_old = etat_bande;
			if (etat_bande_old==1)
				etat_bande = (camera1_p[i]<(1.05*seuil1));
			else
				etat_bande = (camera1_p[i]<(0.95*seuil1));
			
			//etat_bande = (camera1_p[i] < seuil1); 

			if (etat_bande != etat_bande_old)
			{
				bandes_1[nb_bandes_1][0] = etat_bande;
				bandes_1[nb_bandes_1][1] = 1;
				nb_bandes_1++;
			} 
			else 
				bandes_1[nb_bandes_1-1][1]++;
		}
		pb_aquiz1 = 0;
	}
	else
		pb_aquiz1 = 1;
   	
	if ((max_p2 > ((max_moy2 - min_moy2) >> 1)) && (max_moy2 - min_moy2) >= 300 )
	{
		// contsruction des bandes claires sombres
		etat_bande = camera2_p[0]<seuil2;
		nb_bandes_2 = 1;
		bandes_2[0][0] = etat_bande;
		bandes_2[0][1] = 1;
		for (i=1; i<120; i++)
		{
			etat_bande_old = etat_bande;

			if (etat_bande_old==1)
				etat_bande = (camera2_p[i]<(1.05*seuil2));
			else 
				etat_bande = (camera2_p[i]<(0.95*seuil2));
			

			//etat_bande = (camera2_p[i] < seuil2); 

			if (etat_bande != etat_bande_old)
			{
				bandes_2[nb_bandes_2][0] = etat_bande;
				bandes_2[nb_bandes_2][1] = 1;
				nb_bandes_2++;
			}
			else
				bandes_2[nb_bandes_2-1][1]++;
		}
		pb_aquiz2 = 0;
	}
	else 
		pb_aquiz2 = 1;


	// calcul du centre de la ligne et detection ligne d'arrivee
	centre_et_arrivee();
   
}




void centre_et_arrivee(void){
   int16_t i = 0;
   int16_t pos = 0;
   int16_t ecart = 128;
   int16_t ecart_old = 128;
   int16_t seuil1 = 20, seuil2 = 30;
   int16_t milieu1_temp = milieu1;
   int16_t milieu2_temp = milieu2;
   int16_t bande_milieu1_indice=-1;
   // si on a moins de 5 bandes ca ne peut pas etre la ligne d'arrivee
   // on se contente de regarder les bandes sombres
   // on garde celle qui a le plus de probabilites d'etre la bonne
   

   // camera 1 milieu
   ecart = 128;
   ecart_old = 128;
   for (i=0;i<nb_bandes_1;i++){
      if (bandes_1[i][0]==0){
         ecart_old = ecart;
         ecart = abs(pos+bandes_1[i][1]/2 - milieu1);
         // si on a ecart < ecart_old et ecart < seuil1
         if (ecart<ecart_old && ecart<seuil1 && bandes_1[i][1]>15){
            milieu1_temp = pos+bandes_1[i][1]/2;
            bande_milieu1_indice = i;
         }
      }
      // maj de pos : ajout de la largeur de la bande actuelle
      pos += bandes_1[i][1];
   }
   milieu1 = milieu1_temp;


   // camera 2 milieu
   ecart = 128;
   ecart_old = 128;
   pos = 0;
   for (i=0;i<nb_bandes_2;i++){
      if (bandes_2[i][0]==0){
         ecart_old = ecart;
         ecart = abs(pos+bandes_2[i][1]/2 - milieu2);
         // si on a ecart < ecart_old et ecart < seuil2
         if (ecart<ecart_old && ecart<seuil2 && bandes_2[i][1]>8){
            milieu2_temp = pos+bandes_2[i][1]/2;
         }
      }
      // maj de pos : ajout de la largeur de la bande actuelle
      pos += bandes_2[i][1];
   }
   milieu2 = milieu2_temp;


   // detection ligne arrivée camera 1
   ligne_arrivee = 0;
   i = bande_milieu1_indice;
   if (nb_bandes_1>=5 && i-2 >= 0 && i+2<nb_bandes_1){
      ligne_arrivee = (
            bandes_1[i-2][0] == 0 &&
            bandes_1[i-1][1] <= 25 &&
            bandes_1[i+1][0] <= 25 &&
            bandes_1[i+2][0] == 0 );
   }
}


