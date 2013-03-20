#include "MPC5604B_M27V.h"

extern int8_t camera_valeurs[128];
extern uint16_t objectif_vitesse;
extern int32_t moteur_derniere_erreur;
extern int32_t moteur_integrale;
extern uint16_t moteur_compteur;
extern int8_t controle_derniere_erreur;
extern int16_t controle_integrale;
extern uint8_t controle_derniere_position;
extern uint8_t  interrupteur_balance_des_blancs;
extern int8_t main_fin_boucle;
extern float main_timer_period;
extern uint8_t	Capteurs_resultat;
extern uint32_t POS_MILIEU_SERVO;