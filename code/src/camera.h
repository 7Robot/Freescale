#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "MPC5604B_M07N.h"

void Acquisition_Camera(void);
uint16_t* Filtrer_Valeurs_Camera(int camera);

#endif
