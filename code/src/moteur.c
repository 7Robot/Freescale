#include "moteur.h"
#include "constantes.h"
#include "liaison_serie.h"
#include "extern_globals.h"
#include "Config_PIT.h"

void Compteur_Moteur(void)
{
	moteur_compteur++;
	EMIOS_0.CH[11].CSR.B.FLAG = 0x1;
}

void Controle_Vitesse(void)
{
	int32_t erreur;
	int32_t derivee;
	int32_t commande;
	
	
	// On regarde si on appuie sur un bouton on/off
	//if(! (SIU.PGPDI[2].R & 0x40000000))
	//	SIU.PGPDO[0].R = 0x0000C000;		// Active les 2 moteurs
	// FIXME : Passe toujours dans le if
	if(! (SIU.PGPDI[2].R & 0x20000000))
		SIU.PGPDO[0].R = 0x00000000;		// Éteint les 2 moteurs 
	
	erreur = objectif_vitesse - moteur_compteur;
		
	derivee = erreur - moteur_derniere_erreur;
	moteur_integrale +=  erreur;
	commande = MOTEUR_KP * erreur + MOTEUR_KD * derivee + MOTEUR_KI * moteur_integrale;

    moteur_derniere_erreur = erreur;

    // objectif_vitesse
    commande = commande < 0 ? 0 : commande;
    //Moteurs en série:
    //EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + 800;// Moteurs en serie
    //Moteurs en parallèle : 
    EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + commande;//HBridge gauche
    EMIOS_0.CH[7].CBDR.R = EMIOS_0.CH[7].CADR.R + commande;//HBridge Droit

	

	// port série
	/*printhex32(commande);
	TransmitCharacter('\n');
	printhex32(erreur);
	TransmitCharacter('\n');
	printhex32(moteur_integrale);
	TransmitCharacter('\n');
	printhex32(derivee);
	TransmitCharacter('\n');
	printhex32(moteur_compteur);
	TransmitCharacter('\n');
	TransmitCharacter('\n');*/

	moteur_compteur = 0;

	// on indique que l'interruption est finie
	PIT_ClearFlag(1); 

}