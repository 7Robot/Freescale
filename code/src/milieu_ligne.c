
#include "milieu_ligne.h"
#include "constantes.h"





// renvoie un tableau avec la dérivée des valeurs
int16_t* deriver(uint16_t* valeurs)
{
	int i; // iterateur
	int16_t* derivee = (int16_t*) malloc( 128 * sizeof(*derivee) ); // tableau résultant
	
	derivee[0] = 0; //on se fiche de la première valeur
	for (i=1; i<128; i++)
	{
		derivee[i] = valeurs[i] - valeurs[i-1];
	}
	return derivee;
}





// appliquer un hysteresis a 3 niveaux resultants : -1, 0 et 1
// on a besoin de 2 seuils (bas et haut) et d'une tolérance pour l'hysteresis
uint8_t* filtre_hys_3(int16_t* valeurs, int seuil_bas, int seuil_haut, int tol)
{
	int i;						// iterateur
	int16_t v;					// temporaire sur tableau
	uint8_t* signal_hys = (uint8_t*) malloc( 128 * sizeof(*signal_hys) );	// tableau filtré
	
	// initialisation du tableau résultant
	v = valeurs[0];
	if (v>seuil_haut) {signal_hys[0] = 1;}
	else if (v>seuil_bas) {signal_hys[0] = 0;}
	else {signal_hys[0] = -1;}
	
	// calcul du reste du tableau
	for (i=1; i<128; i++)
	{
		v = valeurs[i];
		if (v>seuil_haut+tol) {signal_hys[i] = 1;}
		else if (v<seuil_bas-tol) {signal_hys[i] = -1;}
		else if (v>seuil_haut-tol)
		{
			if (signal_hys[i-1] = 1) signal_hys[i] = 1;
			else signal_hys[i] = 0;
		}
		else if (v<seuil_bas+tol)
		{
			if (signal_hys[i-1] = -1) signal_hys[i] = -1;
			else signal_hys[i] = 0;
		}
		else {signal_hys[i] = 0;}
	}
	return signal_hys;
}




// calcul la position des min et max sur un hysteresis a 3 niveaux
// renvoie un tableau de variations var
// le nombre de variations est renvoyé dans nb_var (limité à 20)
void min_max_hys(uint8_t* hys, Variation* var, int* nb_var)
{
	Variation v;
	
	// premiere variation (test)
	v.sens = 0;
	v.centre = 115;
	var[0] = v;
	
	// seconde variation (test)
	v.sens = 1;
	v.centre = 125;
	var[1] = v;
	
	// nb_var vaut 2 pour les tests
	*nb_var = 2;
}




// renvoie la position de centre de la bande noire de la ligne
uint8_t milieu_ligne(uint16_t* valeurs_filtrees)
{
	uint8_t centre_ligne = 0;		// valeur à renvoyer
	int16_t* signal_derive;			// dérivée des valeurs
	uint8_t* signal_hys;			// apres application hysteresis
	int seuil_bas = -4;				// seuil bas de l'hysteresis
	int seuil_haut = 4;				// seuil haut de l'hysteresis
	int tol = 1;					// tolérance de l'hysteresis
	Variation* var = (Variation*) malloc(20*sizeof(var));// min et max de l'hysteresis
	int* nb_var;					// nb de variations dans var[]
	
	signal_derive = deriver(valeurs_filtrees);
	signal_hys = filtre_hys_3(signal_derive, seuil_bas, seuil_haut, tol);
	min_max_hys(signal_hys, var, nb_var);
	
	return centre_ligne;
}