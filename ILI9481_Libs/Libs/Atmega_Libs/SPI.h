/*
 * SPI.h
 *
 * Created: 5/24/2013 2:49:47 PM
 *  Author: mfolz
 */ 


#ifndef SPI_H_
#define SPI_H_


#include <stdio.h>
#include <avr/io.h>

#define DDR_SPI DDRB
#define SPI_MOSI PORTB2
#define SPI_MISO PORTB3
#define SPI_CLK  PINB1
#define SPI_SS  PORTB0
#define SPI_PORT PORTB


#define  SPI_CS_Low()		CLEARBIT(SPI_PORT , SPI_SS) // pull data low
#define  SPI_CS_High()		SETBIT(SPI_PORT , SPI_SS) // pull data high



/// SWSPI
#define  SW_SPI_DDR  DDRC // Data I/O + 
#define  SW_SPI_PORT  PORTC
#define  SW_SPI_CLK PINC1 //digital pin 11
#define  SW_SPI_Data_Out PINC2 //digital pin 11
#define  SW_SPI_Data_In PINC0 // digital pin 10
// 
#define SW_SPI_CS_DDR DDRD 
#define SW_SPI_CS_PORT PORTD
#define SW_SPI_CS PIND7


void SPI_MasterInit(void);
char SPI_Master_Transmit_Recieve(char cData);
void SPI_Master_Transmit(char cData);
char SPI_Master_Recieve(void);


void SPI_Master_Transmit_Bytes(char *data, int length);
void SPI_Master_Recieve_Bytes(char *data, int length);
void SW_SPI_Transmit(char byte, char datalength);


#define	 SW_SPI_CLK_Low()	CLEARBIT(SW_SPI_PORT, SW_SPI_CLK) // pull clock low
#define  SW_SPI_CLK_High()	SETBIT(SW_SPI_PORT, SW_SPI_CLK) // pull clock high
#define  SW_SPI_Data_Low()		CLEARBIT(SW_SPI_PORT, SW_SPI_Data_Out) // pull data low
#define  SW_SPI_Data_High()		SETBIT(SW_SPI_PORT, SW_SPI_Data_Out) // pull data high
#define  SW_SPI_Data_In_Check() (SW_SPI_PORT & (1 << SW_SPI_Data_In)) 


#define  SW_SPI_CS_Low()		CLEARBIT(SW_SPI_CS_PORT , SW_SPI_CS) // pull data low
#define  SW_SPI_CS_High()		SETBIT(SW_SPI_CS_PORT, SW_SPI_CS) // pull data high




#endif /* SPI_H_ */