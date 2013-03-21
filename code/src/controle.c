#include "controle.h"
#include "constantes.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"

void interruptionControle(void)
{
    uint8_t pos_milieu;
    uint8_t incertitude;
	int8_t erreur;
	int8_t derivee;
	int16_t commande;

    milieu_ligne(&pos_milieu, &incertitude);

    if(incertitude < CONTROLE_INCERTITUDE_PALIER)
    {
        controle_derniere_position = pos_milieu;
    }
    else
    {
    	pos_milieu = controle_derniere_position;
    }

	// vitesse    
    if(pos_milieu < 52 || pos_milieu > 76)
        objectif_vitesse = 1;
    else
       	objectif_vitesse = 1;
    
    commande = ((int8_t)pos_milieu - 64) / objectif_vitesse;
    /*
    // PID pour la direction
    // zieger-nicols
    
    erreur = ((int8_t)pos_milieu) - 0x64;

	derivee = erreur - controle_derniere_erreur;
	controle_integrale +=  erreur;
	commande = CONTROLE_KP * erreur + CONTROLE_KD * derivee + CONTROLE_KI * controle_integrale;
    controle_derniere_erreur = erreur;
    */
    
	/*printserialsigned(incertitude);
	TransmitCharacter(' ');
	printserialsigned(pos_milieu);
	TransmitCharacter('\n');*/
	
	EMIOS_0.CH[4].CBDR.R = POS_MILIEU_SERVO + commande * AMPLITUDE_SERVO / 64;  // Via la carte de puissance 
	//EMIOS_0.CH[3].CBDR.R = POS_MILIEU_SERVO + commande * AMPLITUDE_SERVO / 64;    // Via PA[3]
}
