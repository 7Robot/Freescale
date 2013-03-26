#include "controle.h"
#include "constantes.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"
#include "extern_globals.h"

#define max(x,y) (x<y?y:x)
 
void Controle_Direction(void)
{
    uint8_t pos_milieu;
    uint8_t incertitude;
	int8_t erreur;
	int8_t derivee;
	int16_t commande;
	int16_t commande_bornee;
    

    objectif_vitesse = 6;
    ; 
    milieu_ligne(&pos_milieu, &incertitude);

    if(incertitude < CONTROLE_INCERTITUDE_PALIER)
    {
        controle_derniere_position = pos_milieu;
        compteur_acquisitions_invalides = 0;
    }
    else
    {
    	pos_milieu = controle_derniere_position;
    	compteur_acquisitions_invalides++;
    }

    /*objectif_vitesse = max(6-abs((int16_t)(pos_milieu)-64)/10, 2);
    
    commande = (64 - (int16_t)pos_milieu) / (objectif_vitesse);
    */
    
    // PID pour la direction
    // zieger-nicols
    erreur = 62 - ((int16_t)pos_milieu);

	derivee = erreur - controle_derniere_erreur;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    
    commande = CONTROLE_MILIEU_SERVO + CONTROLE_KP * erreur + CONTROLE_KI*derivee + CONTROLE_KI*controle_integrale;
    
    if(commande < pos_min_servo) commande_bornee = pos_min_servo;
    else if (commande > pos_max_servo) commande_bornee = pos_max_servo;
    else commande_bornee = commande; 
    
    EMIOS_0.CH[4].CBDR.R = commande;

	
	//EMIOS_0.CH[4].CBDR.R = pos_milieu_servo + commande * AMPLITUDE_SERVO;
	
}
