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
	uint16_t valeurs[126];
	uint8_t pos_max = 0;
	uint8_t pos_min = 0;
	uint8_t max_hors_ligne = 0;
	uint8_t i;



	// calcul de la d�riv�e
    for(i = 0; i < 126; i++)
   	{
        valeurs[i] = camera_val[i + 2] - camera_val[i];
    }
    
    valeurs[0] = 0; // hack parce que le premier pixel fait de la merde
	
	
	
		// calcul de la d�riv�e de la deuxi�me camera

    

	
	// recherche du min / du max
	for(i = 0; i < 126; i++)
	{
		if(valeurs[i] > valeurs[pos_max])
			pos_max = i;
		if(valeurs[i] < valeurs[pos_min])
			pos_min = i;
	}
    
    
    	
	*milieu =(pos_min + pos_max) / 2;
	
	// recherche du plus gros pic en dehors de la ligne (incertitude)
	for(i = 0; i < 126; i++)
	{
		if(abs((int8_t)(*milieu) - (int8_t)i) > LARGEUR_LIGNE + DELTA_LARGEUR_LIGNE && abs(valeurs[i]) > max_hors_ligne)
			max_hors_ligne = abs(valeurs[i]);
	}
	
	if(abs((int8_t)pos_min - (int8_t)pos_max - LARGEUR_LIGNE) < DELTA_LARGEUR_LIGNE)
		*incertitude = 100 * max_hors_ligne / min(valeurs[pos_max], -valeurs[pos_min]);
	else
		*incertitude = 250;
	
	
	//ancien_milieu = *milieu
}









