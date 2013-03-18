#include "liaison_serie.h"

/*************************  Fonctions de transmission de données ******************************/

void printlistall(uint8_t tab128[]) 
{
	uint8_t pt=0;	
	TransmitCharacter(0x0a);//line feed   
    TransmitCharacter(0x0d);//retour à la ligne  
    for(pt=0;pt<128;pt++){
       printserialhex(tab128[pt]);
       TransmitCharacter(0x20);//espace
    }
    TransmitCharacter(0x0a);//line feed   
    TransmitCharacter(0x0d);//retour à la ligne   
}

void TransmitCharacter(uint8_t ch)
{
	LINFLEX_0.BDRL.B.DATA0 = ch;  			/* write character to transmit buffer */
	while (1 != LINFLEX_0.UARTSR.B.DTF) {}  /* Wait for data transmission completed flag */
	LINFLEX_0.UARTSR.R = 0x0002; 			/* clear the DTF flag and not the other flags */	
}

void TransmitData (char TransData[]) 
{
	uint8_t	j,k;                                 /* Dummy variable */
	k =(uint8_t) strlen (TransData);
	for (j=0; j< k; j++) 
	{  /* Loop for character string */

		TransmitCharacter(TransData[j]);  		/* Transmit a byte */		

	}
}

/* This functions polls UART receive buffer. when it is full, it moves received data from the buffer to the memory */
uint8_t ReadData (void)
{
	uint8_t ch;
	/* wait for DRF */
	while (1 != LINFLEX_0.UARTSR.B.DRF) {}  /* Wait for data reception completed flag */
		
	/* wait for RMB */
	while (1 != LINFLEX_0.UARTSR.B.RMB) {}  /* Wait for Release Message Buffer */
	
	/* get the data */
	ch = (uint8_t)LINFLEX_0.BDRM.B.DATA4;
		
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFLEX_0.UARTSR.R = 0x0204;
	
	return ch;
	
}

void printhex8(uint8_t innum) {
  uint8_t j1,in;
  uint8_t p1,p2;
  in = innum;
   
  j1 = (in & 0x0f);
  if (j1 > 9) p1 = (uint8_t)(j1 + 0x41 - 10);
  else p1 = (uint8_t)(j1 +0x30);
  j1 = (in & 0xf0) >> 4;
  if (j1 > 9) p2 = (uint8_t)(j1 +0x41 - 10);
  else p2 = (uint8_t)(j1 +0x30);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
}

void printhex16(uint16_t innum) {
	printhex8(innum >> 8);
	printhex8(innum);
}

void printhex32(uint32_t innum) {
	printhex16(innum >> 16);
	printhex16(innum);
}

void printserialsigned(uint16_t innum) {
  uint16_t j1,k1,l1,m1,in;
  uint8_t p1,p2,p3,p4,p5;
 
  if(innum < 0x8000) {
    in = innum;
  	TransmitCharacter('+');    
  } 
  else {
    in = (uint16_t)(~innum);
    in = 0x7fff - in;
    TransmitCharacter('-');     
  }
  
  j1 = (in / 10);
  p1 = (uint8_t)(in - j1*10 +0x30);
  k1 = (j1 / 10);
  p2 = (uint8_t)(j1 - k1*10 +0x30);
  l1 = (k1 / 10);
  p3 = (uint8_t)(k1 - l1*10 +0x30);
  m1 = (l1 / 10);
  p4 = (uint8_t)(l1 - m1*10 +0x30);
  p5 = (uint8_t)m1 +0x30;
  TransmitCharacter(p5);
  TransmitCharacter(p4);
  TransmitCharacter(p3);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
  TransmitCharacter(0x09);
}
