#include "MPC5604B_M27V.h"
#include "constantes.h"

extern uint16_t camera1_valeurs[128];
extern uint16_t camera2_valeurs[128];

extern uint16_t pos_min_servo; 
extern uint16_t pos_max_servo; 
extern uint16_t pos_milieu_servo;
extern uint16_t amplitude_servo;

//extern float objectif_vitesse;


extern int8_t controle_derniere_erreur;
extern int16_t controle_integrale;
extern uint8_t controle_derniere_position;

extern int8_t main_fin_boucle;
extern float main_timer_period;

extern uint16_t compteur_acquisitions_invalides;
extern uint8_t Capteurs_resultat;

extern uint8_t nb_receive;
extern 	vuint8_t autorisation_aquiz;