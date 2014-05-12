/*
 * µSD_Card.c
 *
 * Created: 5/24/2013 2:37:21 PM
 *  Author: mfolz
 
 
 http://mbed.org/users/mbed_official/code/SDFileSystem/file/c8f66dc765d4/SDFileSystem.cpp
 http://mac6.ma.psu.edu/space2008/RockSat/microController/SDcard_Simplified_Physical_Layer_Spec.pdf
 
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uSD_Card.h"
#include "../Atmega_Libs/SPI.h"
#include "../Atmega_Libs/Serial_Atmel328p.h"

char SD_CMD0[6]= {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};  // Initialize
char SD_CMD1[6]= {0x41, 0x00, 0x00, 0x00, 0x00, 0x01};  // Idle State
char SD_CMD55[6]= {0x77, 0x00, 0x00, 0x00, 0x00, 0x01}; // Idle State
char SD_CMD41[6]= {0x69, 0x00, 0x00, 0x00, 0x00, 0x01}; // Idle State
char SD_CMD16[6]= {0x50, 0x00, 0x00, 0x02, 0x00, 0x01}; // Set block size (1-512 in arguement)

char SD_CMD17[6]= {0x51, 0x00, 0x0F, 0x40, 0x00, 0x01}; // Read single block
char SD_CMD24[6]= {0x58, 0x00, 0x0F, 0x40, 0x00, 0x01}; // Write single block

#define R1_IDLE_STATE           (1 << 0)
#define R1_ERASE_RESET          (1 << 1)
#define R1_ILLEGAL_COMMAND      (1 << 2)
#define R1_COM_CRC_ERROR        (1 << 3)
#define R1_ERASE_SEQUENCE_ERROR (1 << 4)
#define R1_ADDRESS_ERROR        (1 << 5)
#define R1_PARAMETER_ERROR      (1 << 6)

	
/* 
- SPI pins 
/////////////////////
   DOUT -  MISO - PB2
	SCK	-  PB1
	DIN	- PB3
	CS	- PB0 
*/
/// Standard output == UART
static FILE uart_stdout = FDEV_SETUP_STREAM(USART_putstring, NULL, _FDEV_SETUP_WRITE);

uint8_t SDcart_disk_read(uint8_t *buffer_array, uint64_t block_number) {
	// set read address for single block (CMD17)
	
	SPI_Master_Transmit_Bytes(SD_CMD17, MMC_CMD_SIZE); 
	if (SDSend_CMD(CMD17, block_number * MMC_BLOCK_SIZE) != 0) return 1; // return 1 on failure
	
	// receive the data
	SDCard_Read(buffer_array, 512);
	return 0;
}

uint8_t SDSend_CMD(char command,  uint32_t argument){
	int response; 

	SPI_CS_Low();
	/// Send Command and argument
	SPI_Master_Transmit(0x40 | command);
	SPI_Master_Transmit(argument >> 24);
	SPI_Master_Transmit(argument >> 16);
	SPI_Master_Transmit(argument >> 8);
	SPI_Master_Transmit(argument);
	SPI_Master_Transmit(0x00); // CRC 
	
	for (int count=0; count < MMC_TIMEOUT; count++){
		response = SPI_Master_Transmit_Recieve(0xFF); // send dummy to read response
		if (!(response & 0x80)){
			SPI_CS_High();
			SPI_Master_Transmit(0xFF);
			return 0; // success 
		}
	}
	SPI_CS_High();
	SPI_Master_Transmit(0xFF);
	return 1; // time out error 	
}

uint8_t SDCard_Read(uint8_t *buffer_array, uint32_t length) {
	SPI_CS_Low();
	
	// read until start byte (0xFF)
	while (SPI_Master_Recieve()  != 0xFE);
	
	// read data
	for (int i = 0; i < length; i++) {
		buffer_array[i] = SPI_Master_Recieve(); 
	}
	SPI_Master_Transmit(0xFF); // checksum
	SPI_Master_Transmit(0xFF); // checksum
	
	SPI_CS_High();
	SPI_Master_Transmit(0xFF); 
	
	return 0;
}

uint8_t SDCard_Disk_Write(uint8_t *buffer_array, uint64_t block_number) {
	// set write address for single block (CMD24)
	if (SDSend_CMD(CMD24, block_number * MMC_BLOCK_SIZE) != 0) return 1; // return 1 on failure
	
	// send the data block
	SDCard_Write(buffer_array, 512);
	return 0;
}

uint8_t SDCard_Write(uint8_t *buffer_array, uint16_t length) {
	SPI_CS_Low();
	
	// indicate start of block
	SPI_Master_Transmit(0xFE); // checksum
	
	// write the data
	for (int i = 0; i < length; i++)  SPI_Master_Transmit(0xFF); // SPI_Master_Transmit(buffer_array[i]);
	
	SPI_Master_Transmit(0xFF); // checksum
	SPI_Master_Transmit(0xFF); // checksum
	
	// check the response token
	if ((SPI_Master_Transmit_Recieve(0xFF) & 0x1F) != 0x05) {
		SPI_CS_High();
		SPI_Master_Transmit(0xFF);
		return 1; // success
	}
	
	// wait for write to finish
	while (SPI_Master_Transmit_Recieve(0xFF) == 0x00);
	
	SPI_CS_High();
	SPI_Master_Transmit(0xFF);
	return 0; // failure 
}

uint8_t SDRead_Response_R1(char response){
	char result;
		
	for (int count=0; count < MMC_TIMEOUT; count++){
		result = SPI_Master_Transmit_Recieve(0xFF); // send dummy to read response
		if (result == response){
			return 0; // success
		}
	}
	return 1; // time out error
}

void mmc_init(void)
{
	char byte;
	char i; 
	/* Init the CS output */
	printf("Turning on Chip \r");
	SPI_CS_High();
	
	/* Init the card in SPI mode by sending at least 72 clocks Data and CS lines must be set to 1*/
	for(i = 0; i < 10; i++) SPI_Master_Transmit(0xFF); /// send FF pull the data line high
	SPI_CS_Low();
	
	printf("Successfully Sent Dummy Clocks \r");

	/* Send CMD0 GO_IDLE_STATE - Resets the card puts it in idle state if CS is pulled low and CMD0 is sent the card is put in 
	spi mode*/
	SPI_Master_Transmit_Bytes(SD_CMD0, MMC_CMD_SIZE); 

	if (SDRead_Response_R1(0x01)) // MMC responds with a R1 response 
	{
		printf("CMD0 timeout Exiting!\r"); 
		return;
	}
	printf("Successfully sent CMD0- device is in idle mode\r");

	/* Send some dummy clocks after GO_IDLE_STATE */
	SPI_CS_High();
	SPI_Master_Recieve(); 
	SPI_CS_Low();

	printf("send CMD1\r");

	/* Send CMD1 SEND_OP_COND until response is 0 */
	i = 0xFF; 
	while((SDRead_Response_R1(0x00) != 0) && (i > 0))
	{
		SPI_Master_Transmit_Bytes(SD_CMD1, MMC_CMD_SIZE);
		i--;
	}
	
	if (i == 0){
		printf("CMD1 timeout\r");
		return;
	}

	printf("send dummy clocks\r");

	/* Send some dummy clocks after GO_IDLE_STATE */
	SPI_CS_High();
	SPI_Master_Recieve();
	SPI_CS_Low();

	
	SPI_Master_Transmit_Bytes(SD_CMD16, MMC_CMD_SIZE);

	if ((SDRead_Response_R1(0x00)) == 1)printf("CMD16 timeout\r");
	else printf("CMD16 Sent 512 bytes \r");

	SPI_CS_High();
	SPI_Master_Recieve();

	printf("mmc_init end\r");

}
