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
    uint8_t pos_milieu2;
    uint8_t pos_abs;
    uint8_t incertitude2;
    int8_t tolerance; 
    int8_t difference_cams;
	int8_t erreur;
	int8_t derivee;
	int16_t commande;
	int16_t commande_bornee;
	static uint32_t compteur_ligne_arrivee = 0; // On regarde combien de fois on croise la ligne d'arrivé
    

   

    milieu_ligne(&pos_milieu, &incertitude);
    
    /////////////////////////// AJOUT CODE POUR DEUXIEME CAMERA /////////////////////////////////////
    
   
    pos_abs = pos_milieu2-64;
    if (pos_abs<0)
    pos_abs = - pos_abs;
    else {}
    
    if (pos_abs<32)
    {
    CONTROLE_KP = 1+(1/8)*pos_abs;
    objectif_vitesse = 5-(1/16)*pos_abs;
    }
    else
    {
    CONTROLE_KP = 5;
    objectif_vitesse = 3;
    }
    
    
    
    /////////////////////////// FIN AJOUT CODE POUR DEUXIEME CAMERA /////////////////////////////////////

	





    compteur_ligne_arrivee++;

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
    if(code_arret_cam() == 1)
    {
        if (compteur_ligne_arrivee >= COMPTEUR_AVANT_ARRIVEE) // 30 secondes après le démarage de la voiture => compteur d'arrêt ligne actif
    	{
    		compteur_acquisitions_invalides = 1000;
    	    compteur_ligne_arrivee = 0;   	    
    	} 
    	SIU.GPDO[71].B.PDO = 0;   	    	
    }
    
    SIU.GPDO[71].B.PDO = compteur_ligne_arrivee >= COMPTEUR_AVANT_ARRIVEE;



    /*objectif_vitesse = max(6-abs((int16_t)(pos_milieu)-64)/10, 2);
    
    commande = (64 - (int16_t)pos_milieu) / (objectif_vitesse);
    */
    
    // PID pour la direction
    // zieger-nicols
    erreur = 64 - ((int16_t)pos_milieu);//difference entre le milieu de la camera et le milieu de la ligne

	derivee = erreur - controle_derniere_erreur;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    
    commande = CONTROLE_MILIEU_SERVO + CONTROLE_KP * erreur + CONTROLE_KD * derivee + CONTROLE_KI*controle_integrale;
    //uniquement Proportionnel actuellement
    
    //if(commande < pos_min_servo) commande_bornee = pos_min_servo;
    //else if (commande > pos_max_servo) commande_bornee = pos_max_servo;
    //else commande_bornee = commande; 
    
    EMIOS_0.CH[4].CBDR.R = commande;

	
	//EMIOS_0.CH[4].CBDR.R = pos_milieu_servo + commande * AMPLITUDE_SERVO;
	
}
