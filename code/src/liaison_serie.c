#include "liaison_serie.h"
#include "extern_globals.h" 
#include "MPC5604B_M07N.h"
#include <math.h>

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
	uint16_t	j,k;                                 /* Dummy variable */
	k =(uint16_t) strlen (TransData);
	//SIU.GPDO[69].B.PDO = 0;     // LED 2 ON
	for (j=0; j< k; j++) 
	{  /* Loop for character string */

		TransmitCharacter(TransData[j]);  		/* Transmit a byte */		

	}
	//SIU.GPDO[69].B.PDO = 1;     // LED 2 OFF
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
//transmet en hexa la valeur sur 8 bit transmise
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

//transmet en hexa la valeur sur 16 bit transmise
void printhex16(uint16_t innum) {
	printhex8(innum >> 8);
	printhex8(innum);
}

//transmet en hexa la valeur sur 32 bit transmise
void printhex32(uint32_t innum) {
	printhex16(innum >> 16);
	printhex16(innum);
}

// transmet un float
void printfloat (float data)
{	
	// gestion de la précision
	int8_t power = 0;
	uint8_t m = 0, c = 0, d = 0, u = 0 , dm = 0, cm = 0, mm = 0;
	//uint16_t toto;
	//gestion du signe
	
	if (data < 0)
	{	TransmitCharacter('-');
		data = -data;	}
	else 
		TransmitCharacter (' ');
	if (data != 0)
	{
		// si c'est trop grand on diminue
		while (data >= 10000.0)
		{	data = data * 0.001;
			power += 3;	
		}
		// si c'est trop petit on augmente
		while (data < 1.0)
		{	data = data*1000;
			power -=3;
		}
	}


	
	data = data + 0.00001;
	while (m * 1000.0 < data) m++;
	m--;
	TransmitCharacter (48+m);
	data -= m * 1000.0;
	
	while (c * 100.0 < data) c++;
	c--;
	TransmitCharacter (48+c);
	data -= c*100.0;
	
	while (d * 10.0 < data) d++;
	d--;
	TransmitCharacter (48+d);
	data -= d*10.0;
	
	while (u * 1.0 < data) u++;
	u--;
	TransmitCharacter (48+u);
	data -= u*1.0;
	
	TransmitCharacter(',');
	
	while (dm * 0.1 < data) dm++;
	dm--;
	TransmitCharacter (48+dm);
	data -= dm*0.1;
	
	while (cm * 0.01 < data) cm++;
	cm--;
	TransmitCharacter (48+cm);
	data -= cm*0.01;
	
	while (mm*0.001 < data) mm++;
	mm--;
	TransmitCharacter (48+mm);
	data -= mm*0.001;
	
	
	
		
	
	TransmitCharacter('e');
	if (power < 0)
	{	TransmitCharacter('-');
		TransmitCharacter(48-power);  	}
	else
	{	TransmitCharacter(48+power);
		TransmitCharacter(' '); 	}
		
	TransmitCharacter(' ');
}

/*
// fonction qui envoie la partie entière du float par TX
// et qui renvoie le meme float 
double printunit(double data)
{
	uint8_t i_i = 0;
	double 	i_f = 0;
	do
	{
		i_f += 1.0;
		if (data < i_f) TransmitCharacter(48+i_i);
		i_i ++;
	}while (i_f <= data && i_i < 10);
	
	return (data - i_f + 1.0)*10.0;
}
*/
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

void Data_uart(void)
{
    static uint8_t select_coef;
    if(select_coef == 0)
    {
        //controle_kp = ReadData();
        TransmitData("\n KP modified \n");
        select_coef = 1;
    }
    else
    {
        //controle_kd = ReadData();
        TransmitData("\n KD modified \n");
        select_coef = 0;
    }

}

