#include "orientation.h"
#include "extern_globals.h"
#include "math.h"

// nom explicite : restreindre valeur entre limite_inf et limite_sup
float limiter(float valeur, float limite_inf, float limite_sup)
{
	return fmax(limite_inf, fmin(valeur, limite_sup));
}

float get_commande_orientation(uint8_t milieu_camera,uint8_t milieu_camera_old)
{
	// variables
	uint8_t centre_camera = 64;
	static float erreur = 0;
	static float derivee = 0;
	float commande_orientation;
	// termes du PD
	float terme_proportionnel;
	float terme_derivee;
	float limite = 1300;
	// coefs du PD
	float Kp = 10;
	float Kd = 0;
	
	
	// Calcul erreur et derivee en filtre passe bas	
	erreur = (0.8*erreur) + (0.2*(centre_camera - milieu_camera));
	derivee = (0.9*derivee) + (0.1*(erreur - controle_derniere_erreur));
	controle_derniere_erreur = erreur;
	
	// Calcul des termes du PD
	terme_proportionnel = Kp*erreur;
	terme_derivee = Kd*derivee;
	
	// limitation des termes du PD
	terme_proportionnel = limiter(terme_proportionnel, -limite, limite);
	terme_derivee = limiter(terme_derivee, -limite, limite);
	
	// calcul de la commande
	commande_orientation = terme_proportionnel + terme_derivee;
	return commande_orientation;
}

float combiner(float base, float orientation, uint8_t milieu_camera_loin)
{
	// variables
	float erreur_camera_loin = fabs(centre_piste_loin - milieu_camera_loin);
	
	coeff_camera_loin = 1/64*erreur_camera_loin;
	return limiter(base + coeff_camera_loin*orientation,(float)pos_min_servo, (float)pos_max_servo);
}