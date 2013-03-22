#include "controle.h"
#include "constantes.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"

#define max(x,y) (x<y?y:x)
 
void Controle_Direction(void)
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
    	pos_milieu = controle_derniere_position < 64 ? 0 : 127;
    }

	// vitesse
    objectif_vitesse = max(6-abs((int16_t)(pos_milieu)-64)/10, 2);
    
    commande = (64 - (int16_t)pos_milieu) / (objectif_vitesse);

    /*
    // PID pour la direction
    // zieger-nicols
    
    erreur = ((int8_t)pos_milieu) - 0x64;

	derivee = erreur - controle_derniere_erreur;
	controle_integrale +=  erreur;
	commande = CONTROLE_KP * erreur + CONTROLE_KD * derivee + CONTROLE_KI * controle_integrale;
    controle_derniere_erreur = erreur;
    */
    
	EMIOS_0.CH[4].CBDR.R = POS_MILIEU_SERVO + commande * AMPLITUDE_SERVO;
	
}
