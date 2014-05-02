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
	//int8_t erreur;
	
	float commande;
	float commande_finale;

	static float derivee = 0.0;
	static float erreur = 0.0;
	float aerreur;
    
    static float erreur2 = 0.0;

    
    // PID pour la direction
    // zieger-nicols
    //erreur = 0.5 * (64 - (milieu1)) + 0.5 * erreur;
	
	erreur = 64-milieu1;
	
	derivee = 0.2*(erreur - controle_derniere_erreur) + 0.8 * derivee;
	controle_integrale += erreur;

    controle_derniere_erreur = erreur;
    aerreur = abs(erreur);
    commande = -controle_kp * erreur * aerreur /40.0 - controle_kd*derivee + controle_ki*controle_integrale;
    
   // erreur2 = 0.25 * (64-milieu2) + 0.75 * erreur2;
   // erreur2 = abs(erreur2);
    
    //erreur2 = max(erreur2, 1);
    //if(!pb_aquiz2)
   	// 	commande = commande * (erreur2/3.0);
    

    //commande_finale = modif_camera2((float)commande);
    commande_finale = commande;
    
	Set_Dir_Servo(commande_finale * autor_controle);
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

//*************************************************	Modification de la commande par la camera 2  ***************************

float modif_camera2(float commande)
{
	int16_t ecart_milieu = 64-milieu2;
	ecart_milieu = abs(ecart_milieu);
	
//	if (!pb_aquiz2)
//		return commande*coeff_modif_commande*(ecart_milieu+1);
//	else
		return commande;
}
