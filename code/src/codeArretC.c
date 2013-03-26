#include "codeArretC.h"
#include "extern_globals.h"
#include "constantes.h"
#define min(i, j) (i < j ? i : j) 
#define max(i, j) (i > j ? i : j)


void recherche_pic (int8_t valeurs_derivee[2][126], uint8_t* pos_max) {
	uint8_t i, k, sup, inf ;
		// Recherche de la premiere valeur de la derivee non encore assimilee a un pic
		k = 0;		
		while (! valeurs_derivee[1][k]) k++;				
		*pos_max = k;
		
		// Recherche du j-ieme max
		for (i=k+1; i<126; i++) {
			if (valeurs_derivee[1][i] && abs(valeurs_derivee[0][i]) > abs(valeurs_derivee[0][*pos_max])) 
			    *pos_max = i;
		}
		
		// Consideration du -ieme max et de ses points voisins
		inf = max(*pos_max -4, 0);
		sup = min(*pos_max +4, 126);
		for (i = inf; i <= sup; i++)  valeurs_derivee[1][i] = 0;
}

int code_arret_cam(void)
{	
	int8_t valeurs_derivee[2][126];
	uint8_t j; 
	uint8_t pos_max_prec, pos_max;
	static uint8_t compteur_arret = 0;
	
	#ifdef DEBUG_ARRET
	TransmitCharacter(0x42);
	#endif
	
    // Initialisation de la dérivée
    for (j=0; j<126; j++) {
		 valeurs_derivee[0][j] = camera_valeurs[j+2] - camera_valeurs[j];
		 valeurs_derivee[1][j] = 1; 
    }

	j = 0;
	do 
	{
	    j++;
	    pos_max_prec = pos_max;
	    recherche_pic(valeurs_derivee, &pos_max);
	   	if(j == 1)
	    	pos_max_prec = pos_max;
	   	#ifdef DEBUG_ARRET
	   	TransmitCharacter(abs(valeurs_derivee[0][pos_max]));
	   	TransmitCharacter(pos_max);
	   	#endif

	}
	while (j< 9 && (abs(valeurs_derivee[0][pos_max]) * 100 / abs(valeurs_derivee[0][pos_max_prec])) > ARRET_SEUIL); 
    j--;
    
   	#ifdef DEBUG_ARRET
	TransmitCharacter(j);
	#endif
    
    if (j >= ARRET_NB_PICS && j < 8 && abs(valeurs_derivee[0][pos_max_prec]) > ARRET_MIN_AMPLITUDE)
    {
    	if(compteur_arret > ARRET_NB_CONSECUTIFS)
    		return 1;
    	compteur_arret++;
    	return 0;
    }
    else
    {
    	compteur_arret = 0;
    	return 0;
    }
}
