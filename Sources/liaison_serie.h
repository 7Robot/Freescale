#ifndef __LIAISON_SERIE__
#define __LIAISON_SERIE__

#include "MPC5604B_M27V.h";

//Autres fonctions UART
void TransmitCharacter(uint8_t ch);
void Rst_Buffer(void);
//void Set_Autor(uint8_t autor);
void TransmitData (char TransData[]);
void TransmitCharacterIfInf(uint8_t ch, uint16_t lim);
void TransmitDataIfInf (char TransData[], uint16_t lim);
//uint8_t ReadData (void);
void printhex8(uint8_t innum);
void printhex16(uint16_t innum);
void printhex32(uint32_t innum);
void printserialsigned(int16_t innum);
 
void Flush_UART_Try(void);

void printfloat (float data);

uint16_t get_i_buffer_t(void);

#endif
