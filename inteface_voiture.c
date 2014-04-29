#define FALSE 0
#define TRUE 1

// A mettre hors de la fonction main
uint16_t get_button();

// Mettre en global
uint16_t old_btn[3] = {0};

/*
Interface pour modifier certains paramètres de la voiture (jusqu'a 16)
On a un tableau de 16 entiers, chacun pouvant prendre une valeur entre 1 et 16.
Les boutons 1 et 2 permettent respectivement d'avancer et de reculer dans le tableau si on est en mode R,
et d'incrementer ou decrementer la valeur du paramètre selectionné si on est en mode !R.
Le bouton 0 permet de changer le mode de R a !R ou inversement.

La rampe de leds indique le mode (R = allumé, !R = éteint).
Les leds affichent en binaire soit la valeur du paramètre soit l'indice du tableau (en fonction du mode).
*/ 

uint16_t menu[16] = {0};
uint16_t parametreARegler=0, valeurDuParam=0;
uint16_t R = TRUE; //La rampe de leds
uint16_t ld[4]={0};
uint16_t nb_btn = 5; 
uint16_t i;
uint16_t raccourciAffichage;

//a mettre dans une boucle d'aquisition avant le démarrage de la voiture
while( //JUSQU'A CE QU'ON DEMARRE LA VOITURE ) 
{


	nb_btn = get_button(); 
	
	if ((nb_btn == 3)&&(!R))
		menu[valeurDuParam] = get_potard();
	
	else
	{
		//traitement des modifs
		if (R) // On est dans le menu de choix des paramètres
		{
			switch (nb_btn)
			{
				case 0 : 	R = !R;
						valeurDuParam = 0;
					break;
				case 1 : 	parametreARegler = (parametreARegler+1) % 16; 
					break;
				case 2 : 	parametreARegler = (parametreARegler+15) % 16;
					break;
			}
		}
		else // On est dans le menu de modif du param actuel
		{
			switch (nb_btn)
			{
			case 0 : 	R = !R;
					parametreARegler = 0; 
				break;
			case 1 : 	valeurDuParam = (valeurDuParam+1) % 16;
					menu[parametreARegler] = valeurDuParam;
				break;
			case 2 : 	valeurDuParam = (valeurDuParam+15) % 16;
					menu[parametreARegler] = valeurDuParam;
				break;
			}
		}
	}
	

	//*******************************************  affichage leds  ***********************************************//

	if (R) // on doit afficher le numero de paramètre
	{
		ld[0] = parametreARegler % 2;
		ld[1] = (parametreARegler % 4)>1;
		ld[2] = (parametreARegler % 8)>3;
		ld[3] = parametreARegler >= 8;	
	}
	else
	{
		if (nb_btn == 3)
			raccourciAffichage = menu[parametreARegler] >> 6;
		else
			raccourciAffichage = menu[parametreARegler];
		ld[0] = raccourciAffichage % 2;
		ld[1] = (raccourciAffichage % 4)>1;
		ld[2] = (raccourciAffichage % 8)>3;
		ld[3] = raccourciAffichage >= 8;	

	}
	
}


uint16_t get_button ()
{
	uint16_t btn[3] = {0};
	uint16_t j;
	uint16_t val_btn = 5;
	
	for (j=0;j<4;j++)
	{
		btn[j] = get_bouton(j);
		if ((btn[j] == 1)&&(old_btn[j] != 1))
		{
			val_btn = j;
		}
		old_btn[j] = btn[j];
	}
	return val_btn;
}


