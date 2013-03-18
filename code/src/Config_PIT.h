#include "MPC5604B_M27V.h"

void enableIrq(void);

void Init_PIT(int8_t ,uint32_t ,float );

void PIT_EnableINTC(int8_t );
									
void PIT_DisableINTC(int8_t );								

void PIT_Enable_Channel(int8_t );		

void PIT_Disable_Channel(int8_t );			

int8_t PIT_GetFlag(int8_t );

void PIT_ClearFlag(int8_t Channel);

void Boucle_principale(void);