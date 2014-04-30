#include "MPC5604B_M27V.h"
#include "constantes.h";

uint16_t camera1_valeurs[128];
uint16_t camera2_valeurs[128];

uint16_t camera1_valeurs_m[120];
uint16_t camera2_valeurs_m[120];

uint16_t camera1_courbe[120];
uint16_t camera2_courbe[120];

uint16_t camera1_p[120];
uint16_t camera2_p[120];

uint16_t max_moy1;
uint16_t min_moy1;
uint16_t max_moy2;
uint16_t min_moy2;

uint16_t max_p1;
uint16_t max_p2;
uint8_t pos_max_p1;
uint8_t pos_max_p2;

uint8_t ancien_milieu=64;//ancien milieu

uint16_t pos_min_servo = POS_MILIEU_SERVO - AMPLITUDE_SERVO;
uint16_t pos_max_servo = POS_MILIEU_SERVO + AMPLITUDE_SERVO;
uint16_t pos_milieu_servo = POS_MILIEU_SERVO;
uint16_t amplitude_servo = AMPLITUDE_SERVO;

float objectif_vitesse = 0;

uint8_t mode_asserv_vitesse = 1;

int8_t controle_derniere_erreur;
int16_t controle_integrale;
uint8_t controle_derniere_position;

int8_t main_fin_boucle;
float main_timer_period;

uint16_t compteur_acquisitions_invalides;
uint8_t	Capteurs_resultat;

uint8_t nb_receive = 0;
vuint8_t autorisation_aquiz = 0;

int16_t mode_spam = 0;

uint16_t controle_kp = CONTROLE_KP;
uint16_t controle_kd = CONTROLE_KD;
uint16_t controle_ki = CONTROLE_KI;
float coeff_modif_commande = 1/64;

uint8_t autor_controle = 0;
uint8_t mode_controle = 0;


float moteur_kp_vit = MOTEUR_KP_VIT;
float moteur_ki_vit = MOTEUR_KI_VIT;
float moteur_kd_vit = MOTEUR_KD_VIT;

float moteur_kp_pos = MOTEUR_KP_POS;
float moteur_ki_pos = MOTEUR_KI_POS;
float moteur_kd_pos = MOTEUR_KD_POS;

float consigne_vitesse_min = 0.05;
float consigne_vitesse_max = 1.0;
float pente_consigne_vitesse = 0.05;
uint8_t autor_vitesse = 0;


uint8_t ecart_normal = 15;//valeur incertaine, a recalculer
	
float led_power = 0;

uint8_t mode_led = 1;
uint16_t consigne_lum = 0;
uint16_t max_lum = 0;
float k_lum = 0.05;

float consigne_led = 0;

uint8_t milieu1, milieu2, milieu1_ok, milieu2_ok, old_milieu1, old_milieu2;
uint8_t pb_aquiz1 = 0, pb_aquiz2 = 0;
	
uint16_t old_btn[3] = {0};

// camera analyse_2
uint8_t bandes_1[120][2]={0}; // bandes (0,1 et leur largeur)
uint8_t bandes_2[120][2]={0}; // bandes (0,1 et leur largeur)
uint8_t nb_bandes_1=0;
uint8_t nb_bandes_2=0;
uint8_t ligne_arrivee=0;
