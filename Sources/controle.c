#include "MPC5604B_M27V.h"
#include "controle.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"
#include "extern_globals.h"

#define max(x,y) (x<y?y:x)
#define abs(i) (i >= 0 ? i : (-i))
 
//***************************************************  Controle_Direction  ****************************************************************
 
void Controle_Direction(uint8_t print)
{
    uint8_t pos_milieu = 0;
    uint8_t incertitude;
    uint8_t pos_milieu_loin;
    uint8_t incertitude_loin;
	int8_t erreur;
	int8_t derivee;
	int16_t commande;
	float commande_finale;
	int16_t commande_bornee;


    if (mode_controle)
    

    
    // PID pour la direction
    // zieger-nicols
    erreur = 64 - ((int16_t)pos_milieu);

	derivee = erreur - controle_derniere_erreur;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    
    commande = -controle_kp * erreur + controle_kd*derivee + controle_ki*controle_integrale;

    commande_finale = modif_camera2((float)commande);
    
    
	Set_Dir_Servo(commande_finale * autor_controle);
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

//*************************************************	Modification de la commande par la camera 2  ***************************

float modif_camera2(float commande)
{
	uint16_t ecart_milieu = abs(64-milieu2);

	if (!pb_aquiz2)
		return commande*coeff_modif_commande*(ecart_milieu+1);
	else
		return commande;
}
