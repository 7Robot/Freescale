#include "codeArretC.h"
#include "extern_globals.h"

int code_arret_cam(void)
{

	static uint8_t nbZero = 0;
	static uint8_t nbLigneArrivee = 0;
	
	int8_t valeursDerivee[126];		
	uint8_t i; 
	uint8_t posMax = 0;
	uint8_t moyenne = 0;
	
	// Calcul de la dérivée, du maximum et de la moyenne
    for (i=0; i<126; i++) {
		 valeursDerivee[i] = camera_valeurs[i+2] - camera_valeurs[i];
		 posMax = (abs(valeursDerivee[i]) > abs(valeursDerivee[posMax]) ? i : posMax);
		 moyenne += camera_valeurs[i];		 
    }
    
    moyenne /= 127;
    
    // Seuil de considération (algorithme) = k1 * moyenne => incertitude
//    if (camera_valeurs[posMax] > 3/2 * moyenne) { 		
       // Detection des pics de la derivee
       for (i=0; i<126; i++) {
		  
		  // Seuil de considération (pic) = k2 * max => incertitude
		  if (abs(valeursDerivee[i]) > (6/10)*abs(valeursDerivee[posMax])) {
			  nbZero++;	
              i += 5;	
		  }	  
       }   
    
       // Detection des lignes d'arrivees intermediaires (au nombre de deux)
       if (nbZero >= 6) {
	   // nbLigneArrivee++;
           nbZero = 0;
           return 1; // provisoire   
       }
       else return 0;
       /*
       // Detection de LA ligne d'arrivee
       if (nbLigneArrivee >= 3) {
	   nbLigneArrivee = 0;
           return 1;		   	   
       }
       */
        	
}
