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
    
    

void milieu_ligne(uint8_t* milieu, uint8_t* incertitude, uint16_t camera_val[], uint8_t print)
{
	int16_t valeurs_deriv[128];
	int16_t valeurs_moy [128]; 
	uint8_t pos_max = 0;
	uint8_t pos_min = 0;
	uint8_t max_hors_ligne = 0;
	uint8_t i, j=0, k=0, l=0, j2=0;
	int16_t seuil;
	uint8_t max_possibles[16] = {0};
	uint8_t min_possibles[16] = {0};
	uint8_t milieu_possible = 0;
	uint8_t proba_milieu;
	uint8_t proba_milieu_mini = 128;
	uint8_t ecart_reel = 0;

	
	uint32_t somme_ligne_moy = 0;
	uint16_t max_moy = 0;
	
	uint16_t moy;
	float a;
	
	
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
	
	
	//On cherche a corriger l'erreur par une courbe ax^2+c
	
	/*a = (somme_ligne_moy-128*max_moy)/DENOMINATEUR_A;
	
	for (i = 0; i<128; i++)
	{
		valeurs_moy[i] = valeurs_moy[i]-(a*((i-64)^2)+max_moy);
	}*/
	
	
	
	
	

	
	
	

	// calcul de la dérivée
    for(i = 0; i < 126; i++)
   	{
        valeurs_deriv[i+1] = valeurs_moy[i + 2] - valeurs_moy[i];
    }
    valeurs_deriv[0] = 0;
    valeurs_deriv[127] = 0;
    
	
	// recherche du min et du max
	for(i = 0; i < 128; i++)
	{
		if(valeurs_deriv[i] > valeurs_deriv[pos_max])
			pos_max = i;
		if(valeurs_deriv[i] < valeurs_deriv[pos_min])
			pos_min = i;
	}
	
	
	seuil = /*max(*/valeurs_deriv[pos_max]/*,abs(valeurs_deriv[pos_min]))*//2;
	
	
	i=0;
    while (i < 128)
    {
    	//on a atteint un pic de valeurs
    	if(valeurs_deriv[i] > seuil)
    	{
    		max_possibles[j] = i;
    		//on cherche le maximum du pic de valeurs
    		while ((valeurs_deriv[i+k] > seuil)&&(i+k<128))
    		{
    			max_possibles[j] = max(valeurs_deriv[i], valeurs_deriv[i+k]);
    			k++;
    		}
    		j++;
    		i+=k-1;
    		k=1;
    	}
    	
    	
    	if(valeurs_deriv[i] < -seuil)
    	{
    		min_possibles[k] = i;
    		
    		while ((valeurs_deriv[i+k] > seuil)&&(i+k<128))
    		{
    			min_possibles[j2] = min(valeurs_deriv[i], valeurs_deriv[i+k]);
    			k++;
    		}
    		j2++;
    		i+=k-1;
    		k=1;
    	}
    	
    	i++;
    }
    
   
    
    for(i = 0; i<j; i++)
    {
    	for(l = 0; l<j2; l++)
    	{    		
    		milieu_possible=(min_possibles[i]+max_possibles[l])/2;
    		ecart_reel = abs(i-l);
    		proba_milieu=abs(milieu_possible-ancien_milieu) + abs(ecart_normal - ecart_reel);
    		if (proba_milieu < proba_milieu_mini)
    			proba_milieu_mini = proba_milieu;
    	}
    }
    
    
    
    //proba_milieu = (pos_max + pos_min)/2;
    ancien_milieu = proba_milieu;
    	
    if (abs(proba_milieu - ancien_milieu) > 5) 
    {
    	*milieu = ancien_milieu;
    }
    else 
    {
    	*milieu = proba_milieu;
    }
	
		if (print)
	{
		/*for (i = 0; i< 128; i++)
		{
			printserialsigned(valeurs_deriv[i]);
			TransmitCharacter('\n');
		}*/
		printserialsigned(proba_milieu);			
		TransmitCharacter('\n');
		TransmitCharacter('\n');
		TransmitCharacter('\n');
	}
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






