#include "MPC5604B_M27V.h"
#include "constantes.h"

extern uint16_t camera1_valeurs[128];
extern uint16_t camera2_valeurs[128];

extern uint16_t camera1_valeurs_m[128];
extern uint16_t camera2_valeurs_m[128];

extern uint16_t camera1_courbe[128];
extern uint16_t camera2_courbe[128];

extern uint16_t max_moy1;
extern uint16_t min_moy1;
extern uint16_t max_moy2;
extern uint16_t min_moy2;

extern uint8_t ancien_milieu;//ancien milieu

extern uint16_t pos_min_servo; 
extern uint16_t pos_max_servo; 
extern uint16_t pos_milieu_servo;
extern uint16_t amplitude_servo;

extern float objectif_vitesse;


extern int8_t controle_derniere_erreur;
extern int16_t controle_integrale;
extern uint8_t controle_derniere_position;

extern int8_t main_fin_boucle;
extern float main_timer_period;

extern uint16_t compteur_acquisitions_invalides;
extern uint8_t Capteurs_resultat;

extern uint8_t nb_receive;
extern 	vuint8_t autorisation_aquiz;
extern uint8_t mode_spam;

extern uint16_t controle_kp ;
extern uint16_t controle_kd ;
extern uint16_t controle_ki ;

extern float moteur_kp_vit;
extern float moteur_ki_vit;
extern float moteur_kd_vit;

extern float moteur_kp_pos;
extern float moteur_ki_pos;
extern float moteur_kd_pos;

extern float led_power;

extern uint8_t ecart_normal;

extern uint8_t mode_led;
extern uint16_t consigne_lum;
extern float k_lum;
extern uint16_t max_lum;
extern float consigne_led;

extern float projection[128];