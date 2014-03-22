#include "liaison_serie.h"
#include "extern_globals.h" 
#include "leds_boutons.h"


// *************************  Fonctions de transmission de données *****************************


    vuint8_t buffer_tx[256];
    vuint8_t buffer_rx[8];
    vuint8_t buffer_rx_lecture[8];
	vuint8_t i_buffer_r = 0;
	vuint8_t i_buffer_t = 0;
	
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

 

void UART_TXI_ISR(void)
{
/*	// V1
	LINFLEX_0.UARTSR.B.DTF = 1;					// clear le bit (oui, on clear en écrivant 1)
	LINFLEX_0.BDRL.R =							// ecriture du buffer avec 4 octets à transmettre
			((buffer_tx[(i_buffer_r+3) & 255] << 24) |
			( buffer_tx[(i_buffer_r+2) & 255] << 16)) 
			|
			((buffer_tx[(i_buffer_r+1) & 255] << 8)  |
		  	  buffer_tx[i_buffer_r]);
	i_buffer_r += 4;

	if (((uint16_t)(256 + i_buffer_t - i_buffer_r) & 255 ) <= 7)		// s'il n'y a plus assez de data à transmettre, on arrete 
		LINFLEX_0.LINIER.B.DTIE =0;
	SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
 */
	// V2
	uint8_t i;
	uint32_t buff_temp =
			((buffer_tx[3] << 24) |		// ecriture du buffer avec 4 octets à transmettre
			( buffer_tx[2] << 16)) 
			|
			((buffer_tx[1] << 8)  |
		  	  buffer_tx[0]);
		  	  
	LINFLEX_0.BDRL.R = buff_temp;
	LINFLEX_0.UARTSR.B.DTF = 1;					// clear le bit (oui, on clear en écrivant 1)
		  	  
	i_buffer_t -= 4;
	for (i = 0; i <= i_buffer_t; i++)
		buffer_tx[i] = buffer_tx[i+4];
	if (i_buffer_t <= 4)
		LINFLEX_0.LINIER.B.DTIE = 0;
	
	//SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
}

void Rst_Buffer(void)
{
	i_buffer_t = 0;
}


void TransmitCharacter(uint8_t ch)
{
/*	// V1
	while (((uint16_t)(i_buffer_t + 5) & 255) == i_buffer_r);
	i_buffer_t ++;
	buffer_tx[i_buffer_t] = ch;
	
	if ((uint16_t)(256 + i_buffer_t - i_buffer_r) & 255 > 3)
  		LINFLEX_0.LINIER.B.DTIE = 1;	// si on a suffisament de data à transmettre, on active les interruption
    */
  	// V2

	//while(i_buffer_t > 250);
	if (i_buffer_t < 250)
	{
		buffer_tx[i_buffer_t] = ch;
		i_buffer_t ++;
		if (i_buffer_t > 4)
			LINFLEX_0.LINIER.B.DTIE = 1;
	}
	

  	
}
uint8_t get_i_buffer_t(void)
{
	return i_buffer_t;
}

void TransmitCharacterIfInf(uint8_t ch, uint8_t lim)
{
	if (i_buffer_t <= lim)
	{
	  	i_buffer_t ++;
		buffer_tx[i_buffer_t] = ch;
		if (i_buffer_t > 4)
			LINFLEX_0.LINIER.B.DTIE = 1;
	}
}

/*  ancienne version, carractère par carractère
void TransmitCharacter(uint8_t ch)
{
	LINFLEX_0.BDRL.B.DATA0 = ch;  			// write character to transmit buffer 
	while (1 != LINFLEX_0.UARTSR.B.DTF) {}  // Wait for data transmission completed flag 
	LINFLEX_0.UARTSR.R = 0x0002; 			// clear the DTF flag and not the other flags 	
}*/

void TransmitData (char TransData[]) 
{
	uint16_t	j,k;                                 // Dummy variable 
	k =(uint16_t) strlen (TransData);
	for (j=0; j< k; j++) 
	{  // Loop for character string 

		TransmitCharacter(TransData[j]);  		// Transmit a byte 		

	}
}

void TransmitDataIfInf (char TransData[], uint8_t lim) 
{
	uint16_t	j,k;                                 // Dummy variable 
	k =(uint16_t) strlen (TransData);
	for (j=0; j< k; j++) 
	{  // Loop for character string 

		TransmitCharacterIfInf(TransData[j], lim);  		// Transmit a byte 		

	}
}
/*
// This functions polls UART receive buffer. when it is full, it moves received data from the buffer to the memory 
uint8_t ReadData (void)
{
	uint8_t ch;
	// wait for DRF 
	while (1 != LINFLEX_0.UARTSR.B.DRF) {}  // Wait for data reception completed flag 
		
	// wait for RMB 
	while (1 != LINFLEX_0.UARTSR.B.RMB) {}  // Wait for Release Message Buffer 
	
	// get the data 
	ch = (uint8_t)LINFLEX_0.BDRM.B.DATA4;
		
	// clear the DRF and RMB flags by writing 1 to them 
	LINFLEX_0.UARTSR.R = 0x0204;
	
	return ch;
	
}*/

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

void printserialsigned(int16_t innum) {
  uint16_t j1,k1,l1,m1;
  int16_t innbr;
  uint8_t p1,p2,p3,p4,p5;
 
  if(innum >= 0) {
    innbr = innum;
  	TransmitCharacter('+');    
  } 
  else {
    innbr = -innum;
    TransmitCharacter('-');     
  }
  
  j1 = (innbr / 10);
  p1 = (uint8_t)(innbr - j1*10 +0x30);
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


// interruption de reception
void UART_RXI_ISR(void)
{
	//uint8_t rx,i;
	
//	SIU.GPDO[71].B.PDO = !LINFLEX_0.UARTSR.B.DRF;
//	SIU.GPDO[70].B.PDO = !LINFLEX_0.UARTSR.B.RMB;
//	SIU.GPDO[69].B.PDO = !SIU.GPDO[69].B.PDO;
//	if (LINFLEX_0.UARTSR.B.DRF == 1)
		//LINFLEX_0.UARTSR.B.DRF =1;
//	if (LINFLEX_0.UARTSR.B.RMB == 1)
	//	LINFLEX_0.UARTSR.B.RMB =1;

	//for (i = 0; i< 20; i++);
	
//	SIU.GPDO[71].B.PDO = !LINFLEX_0.UARTSR.B.DRF;
//	SIU.GPDO[70].B.PDO = !LINFLEX_0.UARTSR.B.RMB;
//	rx = (uint8_t)LINFLEX_0.BDRM.R;				// récupération du mot reçu
/*	while (LINFLEX_0.UARTSR.B.RMB != 1);
	rx = LINFLEX_0.BDRM.B.DATA4;
	nb_receive ++;
	
			TransmitData("\nDRF ");
			printhex8(LINFLEX_0.UARTSR.B.DRF);
			TransmitData("\nRMB ");
			printhex8(LINFLEX_0.UARTSR.B.RMB);
			
//
	LINFLEX_0.UARTSR.B.DRF = 1;					// clear le bit (oui, on clear en écrivant 1)
	LINFLEX_0.UARTSR.B.RMB = 1;*/
//	printhex32(LINFLEX_0.UARTSR.R);
//	TransmitCharacter('\n');
//	TransmitCharacter(rx);
//	TransmitCharacter('\n');
//	printhex32(LINFLEX_0.BDRM.R);
//	TransmitCharacter('\n');

//	LINFLEX_0.UARTSR.B.DRF = 1;					// clear le bit (oui, on clear en écrivant 1)
	
//	INTC.PSR[79].R = 0;
	
	
	uint8_t rx, i;
	
	blink_led(1);
	if(LINFLEX_3.UARTSR.B.DRF == 1 || LINFLEX_3.UARTSR.B.RMB == 1)
	{

		rx = LINFLEX_3.BDRM.B.DATA4;
		LINFLEX_3.UARTSR.B.DRF = 1;
		LINFLEX_3.UARTSR.B.RMB = 1;
		//TransmitCharacterIfInf(rx,80);			// debug
		if (i_buffer_r >= 7)					// si le buffer est plein, on drop la première valeur reçue (fifo)
		{
			i_buffer_r --;
			for (i = 0; i <=6; i++)
				buffer_rx[i] = buffer_rx[i+1];
		}
		if (rx != 0x1b || rx != 0x7e || rx != 0x7f) // si le caractere reçu n'est ni echap, ni suppr, ni effacer....
		{	
			i_buffer_r ++;
			buffer_rx[i_buffer_r] = rx;				// ajoute le dernier mot reçu à la fin du buffer
			
			TransmitCharacter('\n');
			printhex8(rx);
			TransmitCharacter('\n');
			if (rx == '\n' || rx == '\r')						// si le dernier caractère reçu est \n ça veut dire que normalement une commade est prete
			{
				i_buffer_r = 0;
				for (i=0;i<=7;i++)
					buffer_rx_lecture[i] = buffer_rx[i];
				INTC.SSCIR[4].B.SET = 1;			// Declenche l'interruption software 4
			}
		}
		else
			i_buffer_r = 0;
	}
		
}


// interruption de lecture du buffer série
void SwIrq4ISR(void)
{
	uint16_t data =	10000*(buffer_rx_lecture[2] - 48) + 
					1000*(buffer_rx_lecture[3] - 48) +
					100*(buffer_rx_lecture[4] - 48) + 
					10*(buffer_rx_lecture[5] - 48) + 
					(buffer_rx_lecture[6] - 48);
	printfloat(data);
	
	
	switch (buffer_rx_lecture[0])
	{
		case 'c' : switch (buffer_rx_lecture[1])
				{

				case 'p' : controle_kp = data;
					break;
				case 'i' : controle_ki = data;
					break;
				case 'd' : controle_kd = data;
					break;
				}
			break;
	}
					
	/*if (buffer_rx_lecture[0] == 's')// parlons servo
	{
		if (buffer_rx_lecture[1] == 'm')	// parlons milieu
			pos_milieu_servo = data;
	}	*/
	
	INTC.SSCIR[4].B.CLR = 1;		// Clear channel's flag   
}
