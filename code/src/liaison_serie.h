#ifndef __LIAISON_SERIE__
#define __LIAISON_SERIE__

#include "MPC5604B_M27V.h"

//Envoyer un tableau de données en UART
void printlistall(uint8_t tab128[]);
//Autres fonctions UART
void TransmitCharacter(uint8_t ch);
void TransmitData (char TransData[]);
uint8_t ReadData (void);
void printhex8(uint8_t innum);
void printhex16(uint16_t innum);
void printhex32(uint32_t innum);
void printserialsigned(uint16_t innum);

#endif
