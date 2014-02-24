#ifndef __MILIEU_LIGNE_H__
#define __MILIEU_LIGNE_H__

#include "MPC5604B_M07N.h"
#include "extern_globals.h"

// renvoie un tableau avec la dérivée des valeurs
int16_t* deriver(uint16_t* valeurs);

// appliquer un hysteresis a 3 niveaux resultants : -1, 0 et 1
// on a besoin de 2 seuils (bas et haut) et d'une tolérance pour l'hysteresis
uint8_t* filtre_hys_3(int16_t* valeurs, int seuil_bas, int seuil_haut, int tol);

// calcul la position des min sur un hysteresis a 3 niveaux
// renvoie un tableau d'indices
// Attention, la longueur du tableau est dans mini[0]
uint8_t* min_hys(uint8_t* hys);

uint8_t milieu_ligne(uint16_t* valeurs_filtrees);

#endif
