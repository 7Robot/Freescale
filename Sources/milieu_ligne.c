#include "milieu_ligne.h"
#include "constantes.h"
#include "math.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)



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
    
    

void milieu_ligne(uint8_t* milieu, uint8_t* incertitude, uint16_t camera_val[])
{
	uint16_t valeurs_deriv[128];
	uint16_t valeurs_moy [128];
	uint8_t pos_max = 0;
	uint8_t pos_min = 0;
	uint8_t max_hors_ligne = 0;
	uint8_t i;
	
	uint16_t moy;
	
	// fait la somme des 5 proches : 
	// moy [i] = val[i-2] + [i-1] + [i] + [i+1] + [i+2]
	moy = 3*camera_val[0] + camera_val[1] + camera_val[2];
	valeurs_moy[0] = moy;
	moy = moy - camera_val[0] + camera_val[3];
	valeurs_moy[1] = moy;
	moy = moy - camera_val[0] + camera_val[4];
	valeurs_moy[2] = moy;
	
	for (i = 3; i <= 125; i ++)
	{
		moy = moy - camera_val[i-3] + camera_val[i+2];
		valeurs_moy[i] = moy;
	}
	moy = moy - camera_val[123] + camera_val[127];
	valeurs_moy[126] = moy;
	moy = moy - camera_val[124] + camera_val[127];
	valeurs_moy[127] = moy;
	

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









