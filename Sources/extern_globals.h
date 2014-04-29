#include "MPC5604B_M27V.h"
#include "constantes.h"

extern uint16_t camera1_valeurs[128];
extern uint16_t camera2_valeurs[128];

extern uint16_t camera1_valeurs_m[128];
extern uint16_t camera2_valeurs_m[128];

extern uint16_t camera1_courbe[128];
extern uint16_t camera2_courbe[128];

extern uint16_t camera1_p[128];
extern uint16_t camera2_p[128];

extern uint16_t max_moy1;
extern uint16_t min_moy1;
extern uint16_t max_moy2;
extern uint16_t min_moy2;

extern uint16_t max_p1;
extern uint16_t max_p2;

extern uint8_t pos_max_p1;
extern uint8_t pos_max_p2;

extern uint8_t ancien_milieu;//ancien milieu

extern uint16_t pos_min_servo; 
extern uint16_t pos_max_servo; 
extern uint16_t pos_milieu_servo;
extern uint16_t amplitude_servo;

extern float objectif_vitesse;
extern uint8_t mode_asserv_vitesse;

extern int8_t controle_derniere_erreur;
extern int16_t controle_integrale;
extern uint8_t controle_derniere_position;

extern int8_t main_fin_boucle;
extern float main_timer_period;

extern uint16_t compteur_acquisitions_invalides;
extern uint8_t Capteurs_resultat;

extern uint8_t nb_receive;
extern 	vuint8_t autorisation_aquiz;
extern int16_t mode_spam;

extern uint16_t controle_kp ;
extern uint16_t controle_kd ;
extern uint16_t controle_ki ;

extern uint8_t autor_controle;
extern uint8_t mode_controle;

extern float moteur_kp_vit;
extern float moteur_ki_vit;
extern float moteur_kd_vit;

extern float moteur_kp_pos;
extern float moteur_ki_pos;
extern float moteur_kd_pos;

extern float consigne_vitesse_min;
extern float consigne_vitesse_max;
extern float pente_consigne_vitesse;
extern uint8_t autor_vitesse;

extern uint8_t ecart_normal;

extern float led_power;

extern uint8_t mode_led;
extern uint16_t consigne_lum;
extern float k_lum;
extern uint16_t max_lum;
extern float consigne_led;

extern uint8_t milieu1, milieu2, milieu1_ok, milieu2_ok, old_milieu1, old_milieu2;
extern uint8_t pb_aquiz1, pb_aquiz2;

extern uint16_t old_btn[3];