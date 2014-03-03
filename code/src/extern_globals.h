#include "MPC5604B_M07N.h"

extern int8_t camera_valeurs[128];

extern pos_min_servo; 
extern pos_max_servo; 
extern pos_milieu_servo;

extern float objectif_vitesse;

extern float moteur_derniere_erreur;
extern float moteur_integrale;
extern uint16_t moteur_compteur;
extern int16_t controle_derniere_erreur;
extern float controle_integrale;
extern uint8_t controle_derniere_position;
extern int8_t main_fin_boucle;
extern float main_timer_period;
extern uint16_t compteur_acquisitions_invalides;

extern uint8_t Capteurs_resultat;

//pepino
extern uint8_t i_camera_interupt;
extern uint16_t camera_valeurs_brutes1[128];
extern uint16_t camera_valeurs_brutes2[128];


// coefs dependant de l'ecart à la ligne de la camera loin
extern float coeff_camera_loin;

// centre de la piste
extern uint8_t centre_piste_proche;
extern uint8_t centre_piste_loin;