/*
 * UTouch.h
 * PORTED  
 * Created: 4/5/2013 3:54:10 PM
 *  Author: mfolz
 */ 


#ifndef UTOUCH_H_INCLUDED_
#define UTOUCH_H_INCLUDED_
#include <util/delay.h>
#include <avr/io.h>
#include "../Atmega_Libs/Macros_Functions.h"
#include "../Atmega_Libs/AtmegaPins.h"
/* 
ATMEGA 2560 Pins & Touchscreen   XPT2046 
The wireing of this touch screen is questionable at best using 3 ports when only one is necessary is baffeling 
-----------------
Touch screen |  ATMEGA 2560
CLOCK - 	 |	PORTH3 - PIN6 
DIN  -		 |  PINE5 - PIN3
DOUT   -	 |  PORTG5  - PIN4
CS  	 	 |  PORTE3 - PIN5
IRQ			 |  PORTE4 - PIN2
*/
#define PORTRAIT 0x00
#define orient  0x00
#define SPI_EX_Clock_Busy_DDR	DDRH 
#define SPI_EX_DOUT_DDR			DDRG
#define SPI_EX_DIN_CS_DDR		DDRE
#define SPI_EX_Clock_Busy_PORT	PORTH 
#define SPI_EX_DOUT_PORT		PORTG
#define SPI_EX_DIN_CS_PORT		PORTE
#define SPI_EX_BUSY_Flag		PORTH4  // when high impedance when cs is high
#define SPI_EX_DClock			PORTH3 // Clock is connected to PIN6   
#define SPI_EX_DOut				PORTG5  // Digital pin4
#define SPI_EX_DIN				PINE5  
#define SPI_EX_IRQ				PORTE4  // interrupt.
#define SPI_EX_CS				PORTE3  // PIN5 active low

#define  SPI_EX_CLK_Low()				CLEARBIT(SPI_EX_Clock_Busy_PORT,SPI_EX_DClock) // pull clock low
#define  SPI_EX_CLK_High()				SETBIT(SPI_EX_Clock_Busy_PORT, SPI_EX_DClock) // pull clock high
#define  SPI_EX_Data_Out_Low()			CLEARBIT(SPI_EX_DOUT_PORT, SPI_EX_DOut) // pull data low
#define  SPI_EX_Data_Out_High()			SETBIT(SPI_EX_DOUT_PORT, SPI_EX_DOut) // pull data high
#define  SPI_EX_CS_Low()				CLEARBIT(SPI_EX_DIN_CS_PORT,SPI_EX_CS) // pull cs low
#define  SPI_EX_CS_High()				SETBIT(SPI_EX_DIN_CS_PORT,SPI_EX_CS) // pull cs high
#define  SPI_EX_IRQ_DDR_Low()				CLEARBIT(SPI_EX_DIN_CS_DDR,SPI_EX_IRQ) // pull cs low
#define  SPI_EX_IRQ_DDR_High()				SETBIT(SPI_EX_DIN_CS_DDR,SPI_EX_IRQ) // pull cs high
#define  SPI_EX_Data_In_Check() 		CHECKBIT(PINE, SPI_EX_DIN)
#define  SPI_EX_IRQ_In_Check()			CHECKBIT(PINE, SPI_EX_IRQ)


struct XPT2046_Variables{
	float xValue; 
	float yValue;
	int tempterature;
	int orientation;
	char precision;
	int xPositionRes;
	int yPositionRes;
}Touch;

void UTouch_InitTouch(char orientation);
void UTouch_WriteData(char data);
int UTouch_ReadData();
void UTouch_Temperature_Read();
void UTouch_read();
char UTouch_dataAvailable(void);

#endif /* UTOUCH_H_ */



