#include "moteur.h"
#include "liaison_serie.h"
#include "extern_globals.h"

#define MODE_VITESSE
#define MODE_POSITION

int32_t moteur_compteur = 0;
float moteur_integrale_vit = 0;
float moteur_integrale_pos = 0;
float moteur_derniere_erreur = 0;

float distance_parcourue = 0;
float distance_consigne = 0;




//************************************************* Capteur1_Roue_ISR **************************************************************


void Capteur1_Roue_ISR(void)
{
	uint32_t Port;
	
	Port = SIU.PGPDI[3].R;
/*	SIU.GPDO[70].B.PDO = ((Port & 0x02000000) == 0);	//capteur 2
	SIU.GPDO[71].B.PDO = ((Port & 0x08000000) == 0);	//capteur 1
	moteur_compteur = 0;*/
	if ((Port & 0x08000000) == 0)	// si on a declenché l'interruption et que ça donne 0, ça veut dire front descendant
	{
		if ((Port & 0x02000000) == 0)
			moteur_compteur ++;
		else
			moteur_compteur --;
	}
	else	// si front montant
	{
		if ((Port & 0x02000000) == 0)
			moteur_compteur --;
		else
			moteur_compteur ++;
	}
	EMIOS_1.CH[13].CSR.B.FLAG = 0x1;
}



//************************************************* Capteur2_Roue_ISR **************************************************************



void Capteur2_Roue_ISR(void)
{
	
	uint32_t Port;
	//moteur_compteur = 0;
	Port = SIU.PGPDI[3].R;
	if ((Port & 0x02000000) == 0)	// si on a declenché l'interruption et que ça donne 0, ça veut dire front descendant
	{
		if ((Port & 0x08000000) == 0)
			moteur_compteur --;
		else
			moteur_compteur ++;
	}
	else	// si front montant
	{
		if ((Port & 0x08000000) == 0)
			moteur_compteur ++;
		else
			moteur_compteur --;
	}
	EMIOS_1.CH[15].CSR.B.FLAG = 0x1;
}



//************************************************* Asserv_Vitesse **************************************************************




void Asserv_Vitesse(float consigne)
{
	float erreur;
	float derivee;
	float commande = 0;
	int32_t moteur_compteur_temp;
	
	// récupération de l'avancement
	moteur_compteur_temp = moteur_compteur;
	moteur_compteur = 0;
	/*	TransmitData("mot_c: ");
	printfloat((float)moteur_compteur_temp);
	TransmitCharacter('\n');*/
    
    #ifdef MODE_VITESSE
    
	    // calcul erreur	    
	    erreur = consigne - moteur_compteur_temp;	    

		// partie dérivée
		derivee = erreur - moteur_derniere_erreur;
		moteur_derniere_erreur = erreur;
		
		// partie intégrale
		moteur_integrale_vit +=  erreur;
		
		// calcul global
		commande = 
		moteur_kp_vit * erreur + 
		moteur_kd_vit * derivee + 
		moteur_ki_vit * moteur_integrale_vit; 
	
	#endif
	
	#ifdef MODE_POSITION
		// intégration de la consigne (100 passages par seconde)
		distance_consigne += consigne * 0.01;
		
		// intégration du déplacement
		distance_parcourue += moteur_compteur_temp * METER_BY_TICS;
		
		// calcul erreur
		erreur = distance_consigne - distance_parcourue;
		
		// partie dérivée
		derivee = erreur - moteur_derniere_erreur;
		moteur_derniere_erreur = erreur;
		
		// partie intégrale
		moteur_integrale_pos +=  erreur;
		
		// calcul global
		commande = 
		moteur_kp_pos * erreur + 
		moteur_kd_pos * derivee + 
		moteur_ki_pos * moteur_integrale_pos; 
		
	#endif
/*	TransmitData("err : ");
	printfloat(erreur);
	TransmitCharacter('\n');
	
	TransmitData("int : ");
	printfloat(moteur_integrale);
	TransmitCharacter('\n');*/
	
/*	TransmitData("der : ");
	printfloat(derivee);
	TransmitCharacter('\n');
	
	TransmitData("comm : ");
	printfloat(commande);
	TransmitCharacter('\n');*/
	
	

	// envoie de la commande aux moteurs
	Commande_Moteur(commande, commande);
}




//************************************************* Commande_Moteur **************************************************************




// cette fonction attend des valeurs en %
void Commande_Moteur(float C_Moteur_D, float C_Moteur_G)
{
/*	static uint8_t i = 0;
	i++;
	if (i == 120)
	{
		TransmitData("D : \n");
		printfloat(C_Moteur_D);
		TransmitData("\nG : \n");
		printfloat(C_Moteur_G);
		TransmitCharacter('\n');
	}
	
*/	
	// protection anti over-shoot
	if (C_Moteur_D > MAX_MOTEUR)
		C_Moteur_D = MAX_MOTEUR;
	else if (C_Moteur_D < -MAX_MOTEUR)
		C_Moteur_D = -MAX_MOTEUR;
	
	if (C_Moteur_G > MAX_MOTEUR)
		C_Moteur_G = MAX_MOTEUR;
	else if (C_Moteur_G < -MAX_MOTEUR)
		C_Moteur_G = -MAX_MOTEUR;
	

	// les PWM attendent des valeurs en pour 1000 => multiplications par 10...
	if (C_Moteur_D >= 0.0) // 7 et 5
	{
		EMIOS_0.CH[7].CBDR.R = (uint16_t)(10*C_Moteur_D);
		EMIOS_0.CH[5].CBDR.R = 0;
	}
	else
	{
		EMIOS_0.CH[7].CBDR.R = 0;
		EMIOS_0.CH[5].CBDR.R = (uint16_t)(- 10*C_Moteur_D);
	}
	
	if (C_Moteur_G >= 0.0) // 6 et 3
	{
		EMIOS_0.CH[6].CBDR.R = (uint16_t)(10*C_Moteur_G);
		EMIOS_0.CH[3].CBDR.R = 0;
	}
	else
	{
		EMIOS_0.CH[6].CBDR.R = 0;
		EMIOS_0.CH[3].CBDR.R = (uint16_t)(- 10*C_Moteur_G);
	}	
}



float get_moteur_compteur(void)
{
	return (float)(moteur_compteur);
}
