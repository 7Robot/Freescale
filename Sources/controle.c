#include "MPC5604B_M27V.h"
#include "controle.h"
#include "milieu_ligne.h"
#include "liaison_serie.h"
#include "extern_globals.h"

#define max(x,y) (x<y?y:x)
 
 
//***************************************************  Controle_Direction  ****************************************************************
 
void Controle_Direction(uint8_t print)
{
    uint8_t pos_milieu = 0;
    uint8_t incertitude;
    uint8_t pos_milieu_loin;
    uint8_t incertitude_loin;
	int8_t erreur;
	int8_t derivee;
	float commande;



    

    
    // PID pour la direction
    // zieger-nicols
    erreur = 64 - (milieu1);

	derivee = erreur - controle_derniere_erreur;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    
    commande = - controle_kp * erreur + controle_kd*derivee + controle_ki*controle_integrale;
    
    
	Set_Dir_Servo(commande * autor_controle);
}



//************************************************* Set_Dir_Servo **************************************************************



// cette fonction veut des valeurs en %
void Set_Dir_Servo(float consigne)
{
	if (consigne > 100.0)
		consigne = 100;
	else if (consigne < -100.0)
		consigne = -100;
	
	EMIOS_0.CH[4].CBDR.R = pos_milieu_servo + (int16_t)((amplitude_servo * consigne)/100.0);
}