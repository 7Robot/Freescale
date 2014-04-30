#ifndef __MOTEUR__H__
#define __MOTEUR__H__

#include "MPC5604B_M27V.h"

void Asserv_Vitesse(float);

void Commande_Moteur(float C_Moteur_D, float C_Moteur_G);

float calcul_consigne_vitesse(uint8_t);

float get_moteur_compteur(void);
float get_distance_parcourue(void);


void reset_asserv_motor_state(void);
void send_asserv_motor_status(void);

#endif