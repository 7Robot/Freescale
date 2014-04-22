#include "MPC5604B_M27V.h"
#include "constantes.h";

uint16_t camera1_valeurs[128];
uint16_t camera2_valeurs[128];
uint8_t ancien_milieu=64;//ancien milieu

uint16_t pos_min_servo = POS_MILIEU_SERVO - AMPLITUDE_SERVO;
uint16_t pos_max_servo = POS_MILIEU_SERVO + AMPLITUDE_SERVO;
uint16_t pos_milieu_servo = POS_MILIEU_SERVO;
uint16_t amplitude_servo = AMPLITUDE_SERVO;

float objectif_vitesse = 0;


int8_t controle_derniere_erreur;
int16_t controle_integrale;
uint8_t controle_derniere_position;

int8_t main_fin_boucle;
float main_timer_period;

uint16_t compteur_acquisitions_invalides;
uint8_t	Capteurs_resultat;

uint8_t nb_receive = 0;
vuint8_t autorisation_aquiz = 0;

uint16_t controle_kp = CONTROLE_KP;
uint16_t controle_kd = CONTROLE_KD;
uint16_t controle_ki = CONTROLE_KI;


float moteur_kp_vit = MOTEUR_KP;
float moteur_ki_vit = MOTEUR_KI;
float moteur_kd_vit = MOTEUR_KD;

float moteur_kp_pos = MOTEUR_KP;
float moteur_ki_pos = MOTEUR_KI;
float moteur_kd_pos = MOTEUR_KD;

float led_power = 0;

uint8_t ecart_normal = 15;//valeur incertaine, a recalculer
	
	
	
float projection[128];