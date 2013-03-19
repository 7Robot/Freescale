

void codeArret(uint8_t* milieu, uint16_t* incertitude)
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
       if (nbZero > 2) {
		   nbLigneArrivee++;
		   nbZero = 5;
		   SIU.GPDO[71].B.PDO = 0;
		   delay(10000000); // delai d'une seconde
		   SIU.GPDO[71].B.PDO = 1;
       }
       
       // Detection de LA ligne d'arrivee
       if (nbLigneArrivee >= 3) {
		   SIU.GPDO[71].B.PDO = 0;
		   delay(50000000); // delai de 5 secondes
		   SIU.GPDO[71].B.PDO = 1;
		   nbLigneArrivee = 3;
	}
    } 	
}
