/*
 * SPI.c
 *
 * Created: 5/24/2013 2:49:34 PM
 *  Author: mfolz
 */ 

#include <stdio.h>
#include <avr/io.h>
#include "SPI.h"

void SPI_MasterInit(void){
	/* set MOSI and SCK out put and all others input */
	//DDR_SPI |= (1 << DD_MOSI) | (1<< DD_SCK) | (1 << SPI_SS);
	DDR_SPI |= (1 << SPI_SS) | (1<< SPI_CLK) | (1 << SPI_MOSI);
	DDR_SPI &= ~(1 << SPI_MISO);
	//	DDR_SPI = 0b11101100;
	//SPCR = 0x50;
	/*enable SPI, Master, set clock rate fck/16 */
	SPSR = 0x01;   // double speed this tends to be troublesome it works most of the time but....
	SPCR = (1 << SPE)| (1<< MSTR) | (0<< SPR0) | (0 << SPR1) ; // pg 173 of data sheet
	// SPE = SPI enable, when high spi is enabled.
	// MSTR = Selects spi master mode when high.
	// SPR0 = SCK frequency = Oscillator Clock Freq/ 16  (see data sheet for more config)
}


char SPI_Master_Transmit_Recieve(char cData)
{
	/* Start Transmission */
	SPDR = cData;    // memory location 0x2E & 0x4E
	// read and write data register used for data transfer. 1 byte max
	/* wait for transmission to complete when transfer is complete flag is set while loop terminates*/
	while (!( SPSR & (1 << SPIF)));
	// SPSR = Spi status register
	/// SPIF when serial transfer is complete the spif flag is set
	
	cData = SPDR;
	return cData;
}

void SPI_Master_Transmit(char cData)
{
	/* Start Transmission */
	SPDR = cData;    // memory location 0x2E & 0x4E
	// read and write data register used for data transfer. 1 byte max
	/* wait for transmission to complete when transfer is complete flag is set while loop terminates*/
	while (!( SPSR & (1 << SPIF)));
	// SPSR = Spi status register
	/// SPIF when serial transfer is complete the spif flag is set
	
}

char SPI_Master_Recieve(void)
{
	/* Start Transmission */
	char cData = 0x00;
	SPDR = 0xFF;    // send dummy to generate clock
	// read and write data register used for data transfer. 1 byte max
	/* wait for transmission to complete when transfer is complete flag is set while loop terminates*/
	while (!( SPSR & (1 << SPIF)));
	// SPSR = Spi status register
	/// SPIF when serial transfer is complete the spif flag is set
	
	cData = SPDR;
	return cData;
	
}

void SPI_Master_Transmit_Bytes(char *data, int length){
	for(int x=0; x < length; x++)
		SPI_Master_Transmit(data[x]);
}

void SPI_Master_Recieve_Bytes(char *data, int length){
	for(int x=0; x< length; x++)
		data[x]= SPI_Master_Recieve();
}


void SW_SPI_Init(void){

	SW_SPI_DDR |= (1 << SW_SPI_CLK) | (1 << SW_SPI_Data_Out);
	SW_SPI_DDR &= ~(1 << SW_SPI_Data_In);
	SW_SPI_CS_DDR |= (1 << SW_SPI_CS_PORT);
}

void SW_SPI_Transmit(char data, char datalength){
	
	SW_SPI_CS_Low(); 
	for (char datacount=datalength-1; datacount >=0 ; datacount--){
		SW_SPI_CLK_Low();
		
		if (data & _BV(datacount)){
			SW_SPI_Data_High();
		}
	
		else {
			SW_SPI_Data_Low();
		}
		 SW_SPI_CLK_High();
	}
	SW_SPI_CS_High();
	
}

char SW_SPI_Recieve(char datalength){
	char byte;
	byte = 0x00;
	SW_SPI_CS_Low();
	for (char position = 0x00; position < 0x08; position++){
		byte += (SW_SPI_Data_In_Check() << position); 
	}
	return byte;
	SW_SPI_CS_High();
}
	
