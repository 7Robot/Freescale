#include "codeArretC.h"
#include "extern_globals.h"
#define min(i, j) (i < j ? i : j) 
#define max(i, j) (i > j ? i : j)
#define abs(i) (i < 0 ? -1 : i)

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
		inf = max(*pos_max -2, 0);
		sup = min(*pos_max +2, 126);
		for (i = inf; i < sup; i++)  valeurs_derivee[1][i] = 0;
}	 

int code_arret_cam(void)
{
	static uint8_t nb_pic = 5;
	static uint8_t seuil = 50;
	
	int8_t valeurs_derivee[2][126];
	uint8_t j; 
	uint8_t pos_max_prec, pos_max;
	
    // Initialisation de la dérivée
    for (j=0; j<126; j++) {
		 valeurs_derivee[0][j] = camera_valeurs[j+2] - camera_valeurs[j];
		 valeurs_derivee[1][j] = 1; 
    }
    
    // Recherche des 8 pics de la derivee (= caracteristique de la ligne d'arrivee)
    for (j=0; j<nb_pic; j++) {
	    recherche_pic(valeurs_derivee, &pos_max);	
	}
	    
	j = 0;
	do 
	{
	    j++;
	    pos_max_prec = pos_max;
	    recherche_pic(valeurs_derivee, &pos_max);	    
	} 
	while (j< (8 -nb_pic +1) && (valeurs_derivee[0][pos_max_prec] - valeurs_derivee[1][pos_max]) < seuil); 
    
    if ((valeurs_derivee[0][pos_max_prec] - valeurs_derivee[1][pos_max]) < seuil)
        return 1;
    else
        return 0;   
}
