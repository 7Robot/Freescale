#ifndef __MILIEU_LIGNE_H__
#define __MILIEU_LIGNE_H__

#include "MPC5604B_M07N.h"
#include "extern_globals.h"

// structure des variations repérées à partir de l'hyteresis
typedef struct Variation Variation;
struct Variation
{
    int sens; // 0 pour negatif, 1 pour positif
    float centre; // indice du pixel du centre de la variation
};

// renvoie un tableau avec la dérivée des valeurs
int16_t* deriver(uint16_t* valeurs);

// appliquer un hysteresis a 3 niveaux resultants : -1, 0 et 1
// on a besoin de 2 seuils (bas et haut) et d'une tolérance pour l'hysteresis
uint8_t* filtre_hys_3(int16_t* valeurs, int seuil_bas, int seuil_haut, int tol);

// calcul la position des min et max sur un hysteresis a 3 niveaux
// renvoie un tableau de variations var
// le nombre de variations est renvoyé dans nb_var (limité à 20)
void min_max_hys(uint8_t* hys, Variation* var, int* nb_var);

uint8_t milieu_ligne(uint16_t* valeurs_filtrees);

#endif
