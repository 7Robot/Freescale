#include "MPC5604B_M07N.h"


//Envoyer un tableau de données en UART
void printlistall(uint8_t tab128[]);
//Autres fonctions UART
void TransmitCharacter(uint8_t ch);
void TransmitData (char TransData[]);
uint8_t ReadData (void);
void printserialhex(uint16_t innum);
void printserialsigned(uint16_t innum);