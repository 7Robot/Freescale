#include "codeArretC.h"
#include "extern_globals.h"

void codeArret_Cam(void)
{

	static uint8_t nbZero = 0;
	static uint8_t nbLigneArrivee = 0;
	
	int8_t valeursDerivee[126];		
	uint8_t i; 
	uint8_t posMax = 0;
	uint8_t moyenne = 0;
	
	// Calcul de la dérivée, du maximum et de la moyenne
    for (i=0; i<126; i++) {
		 valeursDerivee[i] = camera_valeurs[i+1] - camera_valeurs[i];
		 posMax = (valeursDerivee[i] > valeursDerivee[posMax] ? i : posMax);
		 moyenne += camera_valeurs[i];		 
    }
    
    moyenne /= 127;
    
    if (camera_valeurs[posMax] > 3/2*moyenne) {
		
	   // Detection des pic de la derivee
       for (i=0; i<126; i++) {
		  if (abs(valeursDerivee[i]) > 3/4*valeursDerivee[posMax])
			  nbZero++;			  
       }   
    
       // Detection des lignes d'arrivees intermediaires (au nombre de deux)
       if (nbZero > 4) {
		   nbLigneArrivee++;
		   nbZero = 5;
		   
		   // Provisoire clignotte 2 fois
		   SIU.GPDO[71].B.PDO = 0;
		   delay(5000000); // delai d'une demi seconde
		   SIU.GPDO[71].B.PDO = 1;
		   delay(5000000); 		   
		   SIU.GPDO[71].B.PDO = 0;
		   delay(5000000); 
		   SIU.GPDO[71].B.PDO = 1;		   
       }
       
       // Detection de LA ligne d'arrivee
       if (nbLigneArrivee >= 3) {
		   nbLigneArrivee = 3;	
		   	   
		   SIU.GPDO[71].B.PDO = 0;
		   delay(50000000); // delai de 5 secondes
		   SIU.GPDO[71].B.PDO = 1;
	    }
    delay(10000000); // delai d'une seconde	
    } 	
}
