#include "MPC5604B_M27V.h"
#include "controle.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"
#include "extern_globals.h"

#define max(x,y) (x<y?y:x)
 
 
//***************************************************  Controle_Direction  ****************************************************************
 
void Controle_Direction(uint8_t print)
{
    uint8_t pos_milieu;
    uint8_t incertitude;
    uint8_t pos_milieu_loin;
    uint8_t incertitude_loin;
	int8_t erreur;
	int8_t derivee;
	int16_t commande;
	int16_t commande_bornee;
	static uint32_t compteur_ligne_arrivee = 0; // On regarde combien de fois on croise la ligne d'arrivé
	uint16_t camera1_val[128] ;
	uint16_t camera2_val[128] ;
    int i;
    
    for (i=0; i<=128; i++)
    {
    	camera1_val[i] = camera1_valeurs[i];
    	camera2_val[i] = camera2_valeurs[i];
    }
	
	if (print != 0) 
		TransmitData("cam1:\n");
    milieu_ligne(&pos_milieu, &incertitude, camera1_val, print);
    
    /*if (print != 0) 
		TransmitData("cam2:\n");
    milieu_ligne(&pos_milieu_loin, &incertitude_loin, camera2_val, print);*/
    
/*    TransmitData("\npm :");
    printfloat(pos_milieu);
    TransmitData("\npml :");
    printfloat(pos_milieu_loin);
    TransmitCharacter('\n');*/
    
    /*
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
    

*/


    /*objectif_vitesse = max(6-abs((int16_t)(pos_milieu)-64)/10, 2);
    
    commande = (64 - (int16_t)pos_milieu) / (objectif_vitesse);
    */
    
    // PID pour la direction
    // zieger-nicols
    erreur = 60 - ((int16_t)pos_milieu);

	derivee = erreur - controle_derniere_erreur;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    
    commande = -controle_kp * erreur + controle_kd*derivee + controle_ki*controle_integrale;
    
    
	Set_Dir_Servo((float)commande);
	
}



//************************************************* Set_Dir_Servo **************************************************************



// cette fonction veut des valeurs en %
void Set_Dir_Servo(float consigne)
{
	if (consigne > 100.0)
		consigne = 100;
	else if (consigne < -100.0)
		consigne = 100;
	
	EMIOS_0.CH[4].CBDR.R = pos_milieu_servo + (int16_t)((amplitude_servo * consigne)/100.0);
}