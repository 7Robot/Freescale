#ifndef __MOTEUR__H__
#define __MOTEUR__H__

#include "MPC5604B_M27V.h"

void Asserv_Vitesse(void);

void Commande_Moteur(float C_Moteur_D, float C_Moteur_G);

float get_moteur_compteur(void);

#endif