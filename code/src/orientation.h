#include "MPC5604B_M07N.h"

float get_commande_orientation(uint8_t milieu_camera,uint8_t milieu_camera_old);
float combiner(float base, float orientation, uint8_t milieu_camera_loin);