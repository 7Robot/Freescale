#ifndef __MILIEU_LIGNE_H__
#define __MILIEU_LIGNE_H__

#include "MPC5604B_M27V.h";
#include "extern_globals.h";

void moyenne_glissante(int8_t* valeurs);
void milieu_ligne(uint8_t* milieu, uint8_t* incertitude);

#endif
