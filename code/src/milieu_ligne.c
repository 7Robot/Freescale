#include "milieu_ligne.h"
#include "constantes.h"

#define max(i, j) (i > j ? i : j)
#define min(i,j) (i > j ? j : i)

void moyenne_glissante(int8_t* valeurs)
{
    uint8_t i;
    int8_t previous = *valeurs;
    int8_t temp;

    *valeurs = (previous*2 + *(valeurs + 1))/3;

	for(i = 1; i < 125; i++)
    {
        temp = *(valeurs + i);
        *(valeurs + i) = (temp + previous + *(valeurs + i + 1)) / 3;
        previous = temp;
    }

    *(valeurs + 125) = ((*(valeurs + 125))*2 + previous)/3;
}
    

void milieu_ligne(uint8_t* milieu, uint8_t* incertitude)
{
    int8_t valeurs[126];
	uint8_t pos_max = 0;
	uint8_t pos_min = 0;
	uint8_t max_hors_ligne = 0;
	uint8_t i;
	
	#ifdef DEBUG_LIGNE
	TransmitCharacter(0x42); // Délimiteur pour l'affichage en python
	#endif

	// calcul de la dérivée
    for(i = 0; i < 126; i++)
   	{
        valeurs[i] = (int8_t)camera_valeurs[i + 2] - (int8_t)camera_valeurs[i];
    }
    
    valeurs[0] = 0; // hack parce que le premier pixel fait de la merde
	

	// recherche du min / du max
	for(i = 0; i < 126; i++)
	{
		if(valeurs[i] > valeurs[pos_max])
			pos_max = i;
		if(valeurs[i] < valeurs[pos_min])
			pos_min = i;
		
		#ifdef DEBUG_LIGNE
		TransmitCharacter(valeurs[i]);
		#endif
		
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
	
	#ifdef DEBUG_LIGNE
	TransmitCharacter(pos_min);
	TransmitCharacter(pos_max);
	TransmitCharacter(*incertitude);
	TransmitCharacter(*milieu);
	#endif

}