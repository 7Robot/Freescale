#include "codeArretC.h"
#include "extern_globals.h"

void code_arret_cam(void)
{
	static uint8_t nbLigneArrivee = 0;
	static uint8_t pallier_pic = 65;
	uint8_t nbZero = 0;
		
	int8_t valeursDerivee[126];		
	uint8_t i; 
	uint8_t pos_max = 0;

	
    // recherche du max
	for(i = 1; i < 126; i++)
	{
	    valeursDerivee[i] = camera_valeurs[i+2] - camera_valeurs[i];
		if(valeursDerivee[i] > valeursDerivee[pos_max])
			pos_max = i;
	}

        
    // Seuil de considération (algorithme) = k1 * moyenne => incertitude
    if (valeursDerivee[pos_max] > pallier_pic) { 		
       // Detection des pics de la derivee
       i = 0;
       do 
       {		  
		  // Seuil de considération (pic) = k2 * max => incertitude
		  if (abs(valeursDerivee[i]) >= (5/10)*abs(valeursDerivee[pos_max])) {
			  nbZero++;	
              i += 3;                     	
		  }
		  else i++;
       }  while (nbZero < 5 && i <= 125);  
    
       // Detection des lignes d'arrivees intermediaires (au nombre de deux)
       if (nbZero == 5) {
	       nbLigneArrivee++;
           nbZero = 0;
           SIU.GPDO[68].B.PDO = 0; // LED1 ON : a enlever
       }

       
       // Detection de LA ligne d'arrivee
       if (nbLigneArrivee == 3) {
	       nbLigneArrivee = 0;
           SIU.GPDO[69].B.PDO = 0; // LED2 ON : a remplacer par la commade arreter moteur
       }
    }
        	
}
