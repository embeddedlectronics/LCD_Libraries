/*
 * uSD_Card.h
 *
 * Created: 5/24/2013 2:38:22 PM
 *  Author: mfolz
 */ 


#ifndef USD_CARD_H_INCLUDED_
#define USD_CARD_H_INCLUDED_

#include "../Atmega_Libs/AtmegaPins.h"
#include <stdio.h>

#define MMC_CMD_SIZE 6
#define MMC_TIMEOUT  0xFF
#define MMC_BLOCK_SIZE 512

#define DDR_CS  DDRB
#define PORT_CS PORTB
#define SW_SPI_CS PINB0
#define DD_CS	PORTB0

#define SET_CS()   PORT_CS |= _BV(DD_CS);
#define CLEAR_CS() PORT_CS &= ~_BV(DD_CS);

#define  SW_SPI_CS_Low()		CLEARBIT(PORT_CS, SW_SPI_CS) // pull data low
#define  SW_SPI_CS_High()		SETBIT(PORT_CS, SW_SPI_CS) // pull data high






/* Definitions for MMC/SDC command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define CMD13	(13)		/* SD_STATUS (SDC) */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* SET ERASE START BLOCK */
#define CMD33	(33)		/* SET ERASE FINISH BLOCK */
#define CMD38	(38)		/* EXECUTE ERASE BLOCK ON CMD32 & CMD33 BLOCKS */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */




uint8_t mmc_cmd[MMC_CMD_SIZE];


uint8_t mmc_response(uint8_t response);
void mmc_init(void);
void mmc_read_block(uint16_t block_number, uint8_t* block_address);



uint8_t SDcart_disk_read(uint8_t *buffer_array, uint64_t block_number);
uint8_t SDSend_CMD(char command,  uint32_t argument);
uint8_t SDCard_Read(uint8_t *buffer_array, uint32_t length);
uint8_t SDCard_Disk_Write(uint8_t *buffer_array, uint64_t block_number);
uint8_t SDCard_Write(uint8_t *buffer_array, uint16_t length);
uint8_t SDRead_Response_R1(char response);

#endif /* USD_CARD_H_ */