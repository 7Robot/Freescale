#ifndef __CAMERA_H__
#define __CAMERA_H__

#define BITS_SI  0x00003000
#define BITS_CLK 0x00000C00
#define DELAY_CLK_CAM 40

uint16_t Acquisitions_Cameras(void);

void moy_cam (uint8_t do_moy);

void calcul_courbe(void);
void retranche_courbe(void);

//uint8_t analyse_cam(void);
void analyse_cam_bis(void);
void centre_et_arrivee(void);

void delay(uint32_t nb_tours);

void asserv_leds(uint16_t max_cam);
void Set_PWM_Leds(float consigne);


#endif
