#include "moteur.h"
#include "liaison_serie.h"
#include "extern_globals.h"

int32_t moteur_compteur = 0;
float moteur_integrale = 0;
float moteur_derniere_erreur = 0;
float moteur_derniere_derivee = 0;


//************************************************* Capteur1_Roue_ISR **************************************************************


void Capteur1_Roue_ISR(void)
{
	uint32_t Port;
	
	Port = SIU.PGPDI[3].R;
/*	SIU.GPDO[70].B.PDO = ((Port & 0x02000000) == 0);	//capteur 2
	SIU.GPDO[71].B.PDO = ((Port & 0x08000000) == 0);	//capteur 1
	moteur_compteur = 0;*/
	if ((Port & 0x08000000) == 0)	// si on a declench� l'interruption et que �a donne 0, �a veut dire front descendant
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
	if ((Port & 0x02000000) == 0)	// si on a declench� l'interruption et que �a donne 0, �a veut dire front descendant
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




void Asserv_Vitesse(void)
{
	float erreur;
	float derivee;
	float commande;
	int32_t moteur_compteur_temp;
   float liss_err = 0.8;
   float liss_der = 0.8;
    
    // calcul erreur
    moteur_compteur_temp = moteur_compteur;
    moteur_compteur = 0;
    erreur = liss_err*moteur_derniere_erreur + (1-liss_err)*(objectif_vitesse - moteur_compteur_temp);
    
/*	TransmitData("mot_c: ");
	printfloat((float)moteur_compteur_temp);
	TransmitCharacter('\n');*/
	// partie d�riv�e
	derivee = liss_der*moteur_derniere_derivee + (1-liss_der)*(erreur - moteur_derniere_erreur);
	moteur_derniere_erreur = erreur;
	
	// partie int�grale
	moteur_integrale +=  erreur;
	
	// calcul global
	commande = 
	moteur_kp * erreur + 
	moteur_kd * derivee + 
	moteur_ki * moteur_integrale;
	
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
