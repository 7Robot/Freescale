#ifndef __CAMERA_H__
#define __CAMERA_H__

#define BITS_SI  0x00003000
#define BITS_CLK 0x00000C00
#define DELAY_CLK_CAM 40

uint16_t Acquisitions_Cameras(void);

void coeffs_moy_cam (uint8_t do_coeffs);

void delay(uint32_t nb_tours);

void asserv_leds(uint16_t max_lum);
void Set_PWM_Leds(float consigne);

#endif
