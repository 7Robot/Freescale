#include "MPC5604B_M27V.h"

extern int8_t camera_valeurs[128];

extern pos_min_servo; 
extern pos_max_servo; 
extern pos_milieu_servo;

extern float objectif_vitesse;

extern float moteur_derniere_erreur;
extern float moteur_integrale;
extern uint8_t moteur_compteur;
extern int8_t controle_derniere_erreur;
extern int16_t controle_integrale;
extern uint8_t controle_derniere_position;
extern int8_t main_fin_boucle;
extern float main_timer_period;
extern uint16_t compteur_acquisitions_invalides;

extern uint8_t Capteurs_resultat;