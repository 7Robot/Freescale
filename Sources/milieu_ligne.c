#include "milieu_ligne.h"
#include "liaison_serie.h"
#include "constantes.h"
#include "math.h"

#include "vector_X1.h"
#include "vector_X2.h"
#include "vector_X4.h"
#include "vector_X6.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)

#define MODE_PROJ


//*************************************************  moyenne_glissante **************************************************************



void moyenne_glissante(int8_t* valeurs)
{
    uint8_t i;
    uint16_t previous = *valeurs;
    uint16_t temp;

    *valeurs = (previous*2 + *(valeurs + 1))/3;

	for(i = 1; i < 125; i++)
    {
        temp = *(valeurs + i);
        *(valeurs + i) = (temp + previous + *(valeurs + i + 1)) / 3;
        previous = temp;
    }

    *(valeurs + 125) = ((*(valeurs + 125))*2 + previous)/3;
}
    
    

//************************************************* abs **************************************************************



 uint16_t abs(uint16_t n)
 {
 	if (n>0)
 		return n;
 	else
 		return -n;
 }
    
//************************************************* milieu_ligne **************************************************************
    
    

void milieu_ligne(uint8_t* milieu, uint8_t* incertitude, uint16_t camera_val[], uint8_t print)
{
	int16_t valeurs_deriv[128];
	uint16_t valeurs_moy [128]; 
	uint8_t pos_max = 0;
	uint8_t pos_min = 0;
	uint8_t max_hors_ligne = 0;
	uint8_t i;
	
	uint32_t somme_ligne_moy = 0;
	uint16_t max_moy = 0;
	
	uint16_t moy;
	float a;
	#ifdef MODE_PROJ
		float coef_0 = 0.0883883476483184;
		float proj_0 = 0.0;
		float proj_1 = 0.0;
		float proj_2 = 0.0;
		float proj_4 = 0.0;
		float proj_6 = 0.0;
		float projection[128], moy_proj[128];
		float min_moy = 10000000;
		uint8_t pos_min_moy = 0;
		
		float max_1_proj = 0.0,max_2_proj = 0.0;
		float proj_prev = 0.0, proj_actu = 0.0, proj_next = 0.0;
		uint8_t pos_max_1, pos_max_2, max_en_cours = 1;
		float offset, pente;
	#endif
	
	// fait la somme des 5 proches : 
	// moy [i] = val[i-2] + val[i-1] + val[i] + val[i+1] + val[i+2]
	// moy [i+1] = moy[i] - val[i-3] + val[i+2]
	
	// regarde en meme temps le max de ces valeurs et la somme globale de toutes 
	
	moy = 3*camera_val[0] + camera_val[1] + camera_val[2];
	valeurs_moy[0] = moy;
	max_moy = moy;
	somme_ligne_moy += moy;
	
	moy = moy - camera_val[0] + camera_val[3];
	valeurs_moy[1] = moy;
	max_moy = max(max_moy, moy);
	somme_ligne_moy += moy;
	
	moy = moy - camera_val[0] + camera_val[4];
	valeurs_moy[2] = moy;
	max_moy = max(max_moy, moy);
	somme_ligne_moy += moy;
	
	for (i = 3; i <= 125; i ++)
	{
		moy = moy - camera_val[i-3] + camera_val[i+2];
		valeurs_moy[i] = moy;
		max_moy = max(max_moy, moy);
		somme_ligne_moy += moy;
	}
	
	moy = moy - camera_val[123] + camera_val[127];
	valeurs_moy[126] = moy;
	max_moy = max(max_moy, moy);
	somme_ligne_moy += moy;
	
	moy = moy - camera_val[124] + camera_val[127];
	valeurs_moy[127] = moy;
	max_moy = max(max_moy, moy);
	somme_ligne_moy += moy;
	
		
	#ifdef MODE_PROJ
		for (i = 0; i < 128; i ++)
		{
			proj_0 += coef_0*valeurs_moy[i];
			proj_1 += vector_X1[i]*valeurs_moy[i];
			proj_2 += vector_X2[i]*valeurs_moy[i];
			proj_4 += vector_X4[i]*valeurs_moy[i];
			proj_6 += vector_X6[i]*valeurs_moy[i];	
		}
		
		for (i = 0; i < 128; i++)
		{
			projection[i] = coef_0*proj_0 + proj_1*vector_X1[i] + proj_2*vector_X2[i] + proj_4*vector_X4[i] + proj_6*vector_X6[i];
		}
		
		proj_prev = projection[0];
		proj_actu = projection[1];
		proj_next = projection[2];
		i = 1;
		while (max_en_cours == 1 && i < 64)
		{
			if (proj_actu > proj_prev && proj_actu >= proj_next)
			{
				max_en_cours = 2;
				max_1_proj = proj_actu;
				pos_max_1 = i;
			}
			i++;
			proj_prev = proj_actu;
			proj_actu = proj_next;
			proj_next = projection[i+1];
		}
		
		while (max_en_cours == 2 && i < 126)
		{
			i++;
			proj_prev = proj_actu;
			proj_actu = proj_next;
			proj_next = projection[i+1];
			
			if (proj_actu >= proj_prev && proj_actu > proj_next)
			{
				max_en_cours = 3;
				max_2_proj = proj_actu;
				pos_max_2 = i;
			}
		}
		
		if (max_en_cours == 3)
		{
			pente = (max_2_proj - max_1_proj)/(pos_max_2 - pos_max_1);
			offset = max_1_proj - pente * pos_max_1;
			for (i = pos_max_1; i <= pos_max_2; i++)
			{
				projection[i] = offset + i * pente;
			}
			
			
			for (i = 0; i < 127; i++)
			{
				moy_proj[i] = valeurs_moy[i] - projection[i];
				if (moy_proj[i] < min_moy)
				{
					min_moy = moy_proj[i];
					pos_min_moy = i;
				}
			}
		}
		//else
		// PB !
		
		
	#endif
	
	
	//On cherche a corriger l'erreur par une courbe ax^2+c
	
	a = (((float)somme_ligne_moy - 128.0*(float)(max_moy)) * DENOMINAT_A_DIVIDE);
	
	for (i = 0; i<128; i++)
	{
		valeurs_moy[i] = (int16_t)(valeurs_moy[i] - (a*( (i-64)^2) + max_moy) );
	}
	
	
	
	
	
	if (print)
	{
		for (i = 0; i< 128; i++)
		{
			printhex16(valeurs_moy[i]);
			TransmitCharacter('\n');
		}				
		TransmitData("somme:\n");
		printhex32(somme_ligne_moy);
		TransmitData("\nmax:\n");
		printhex16(max_moy);
		TransmitCharacter('\n');
		printfloat(a);
		TransmitCharacter('\n');
		printfloat(DENOMINAT_A_DIVIDE);
		TransmitCharacter('\n');
		printfloat(DENOMINATEUR_A);
		TransmitCharacter('\n');
	}
	
	
	

	// calcul de la dérivée
    for(i = 0; i < 126; i++)
   	{
        valeurs_deriv[i+1] = valeurs_moy[i + 2] - valeurs_moy[i];
    }
    valeurs_deriv[0] = 0;
    valeurs_deriv[127] = 0;
    
	
	// recherche du min / du max
	for(i = 0; i < 128; i++)
	{
		if(valeurs_deriv[i] > valeurs_deriv[pos_max])
			pos_max = i;
		if(valeurs_deriv[i] < valeurs_deriv[pos_min])
			pos_min = i;
	}
    
    	
	*milieu =(pos_min + pos_max) / 2;
	
	// recherche du plus gros pic en dehors de la ligne (incertitude)
	for(i = 0; i < 126; i++)
	{
		if(abs((int8_t)(*milieu) - (int8_t)i) > LARGEUR_LIGNE + DELTA_LARGEUR_LIGNE && abs(valeurs_deriv[i]) > max_hors_ligne)
			max_hors_ligne = abs(valeurs_deriv[i]);
	}
	
	if(abs((int8_t)pos_min - (int8_t)pos_max - LARGEUR_LIGNE) < DELTA_LARGEUR_LIGNE)
		*incertitude = 100 * max_hors_ligne / min(valeurs_deriv[pos_max], -valeurs_deriv[pos_min]);
	else
		*incertitude = 250;
	
	
	//ancien_milieu = *milieu
}






