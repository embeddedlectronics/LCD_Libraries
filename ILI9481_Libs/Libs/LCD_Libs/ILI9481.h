/*
 * ILI9481.h
 *
 * Created: 25/11/2013 8:43:42 AM
 *  Author: Michael
 */ 


#ifndef ILI9481_H_INCLUDED
#define ILI9481_H_INCLUDED

#include <avr/io.h>
#include <avr/delay.h>
#include "LCD_Draw.h"
#include "LCD_Config.h"

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))

#define ILI9481_UPPER_ADDRESS_LOW		0x00
#define ILI9481_UPPER_ADDRESS_HIGH		0xFF

#define ILI9481_LB_PORT PORTC
#define ILI9481_HB_PORT PORTA

#define ILI9481_LB_DDR DDRC
#define ILI9481_HB_DDR DDRA

// DB0 - DB7 = PC0 - PC7 (perfect)
// DB8 - DB15 = PA0- PA7 (lines up perfectly)
#define ILI9481_RS	PORTD7 // PIN 38
#define ILI9481_WR	PORTG2 // PIN 39
#define ILI9481_CS	PORTG1 // PIN 40 
#define ILI9481_RESET	PORTG0 // PIN 41

#define ILI9481_CS_WR_RESET_PORT PORTG
#define ILI9481_RS_PORT	PORTD

#define ILI9481_CS_WR_RESET_DDR DDRG
#define ILI9481_RS_DDR	DDRD

/// H_RS		=   PD7
/// H_CS		=   PG1
/// H_WR  -		=   PG2
/// H_RESET - RSET = PG0

// port b for serial communication shit

// iMPORTANT COMMANDS 
#define SET_PIXLE_FORMAT		0x3A  // 87 
#define SET_ADDRESS_MODE		0x36
#define SET_COLUMN_ADDRESS		0x2A // XStart and end address 
#define SET_ROW_ADDRESS		0x2B // YStart and end address 

#define	PANEL_DRIVING_SETTING	0xC0 

/* Command List*/ 
#define NOP						0x00
#define SOFT_RESET				0x01

// READ COMMANDS 
#define GET_RED_CHANNEL			0x06
#define GET_GREEN_CHANNEL		0x07
#define GET_BLUE_CHANNEL		0x08
#define GET_POWER_MODE			0x0A
#define GET_ADDRESS_MODE		0x0B // Sets column address adn page order, rgb order /page 43 
#define GET_PIXEL_FORMAT		0x0C
#define GET_DISPLAY_MODE		0x0D
#define GET_SIGNAL_MODE			0x0E
#define GET_DIAGNOSTIC_RESULT	0x0F

// POWER EFF COMMANDS 
#define ENTER_SLEEP_MODE		0x10
#define EXIT_SLEEP_MODE			0x11
#define ENTER_PARTIAL_MODE		0x12
#define ENTER_NORMAL_MODE		0x13

#define EXIT_INVERT_MODE		0x20
#define ENTER_INVERT_MODE		0x21

#define SET_GAMMA_CURVE			0x26
#define SET_DISPLAY_OFF			0x28
#define SET_DISPLAY_ON			0x29
#define WRITE_MEMORY_START		0x2C
#define WRITE_LUT				0x2D
#define READ_MEMORY_START		0x2E

#define SET_PARTIAL_AREA		0x30
#define SET_SCROLL_AREA			0x33
#define SET_TEAR_OFF			0x34
#define SET_TEAR_ON				0x35
#define SET_SCROLL_START		0x37
#define EXIT_IDLE_MODE			0x38
#define ENTER_IDLE_MODE			0x39


#define WRITE_MEMORY_CONTINUE	0x3C
#define READ_MEMORY_CONTINUE	0x3E
#define SET_TEAR_SCANLINE		0x44
#define GET_SCANLINE			0x45 

#define READ_DDB_START			0xA1
#define COMMAND_ACCESS_PROTECT	0xB0
#define FRAME_ACCESS			0xB3
#define DISPLAY_MODE_FRAME_MEM	0xB4
#define DEVICE_CODE_READ		0xBF


// Power Settings 
#define POWER_SETTING			0xD0
#define VCOM_CONTROL			0xD1
#define POWER_SETTING_FOR_NORMAL_MODE	0xD2

#define FRAME_RATE_AND_INVERSION_CONTROL	0xC5
#define GAMMA_SETTING			0xC8



// Keep all  the same 
void ILI9481_SendCommand(char command_address, char *command_data, char length);
void ILI9481_SendPacket(char UpperByte,char LowerByte); 
void ILI9481_Initalize(void);
void ILI9481_Address_set(void);
void ILI9481_SendPixel(int x, int y, long int color);
void ILI9481_Fill_Rectangle(int x, int y, int width, int height, long int color);

void ILI9481_SendCommand_Single(char command_address, int command_data); 

////demo shit

void ILI9481_WriteData(char data);
void ILI9481_WriteCom(char command);


char Exit_Sleep_Mode_Settings;
char Gamma_Settings_array[12];

// Initalize resoultion arrays
char X_Max_Column_Settings[4]; // 320
char Y_Max_Row_Settings[4]; // 480

char X_Max[4]; // 320
char Y_Max[4]; // 480

// Pannel Driving arays
char Panel_Driving_Settings[5] ;

char COLOR_array[3];

char Set_Pixle_Format_Settings;
char Set_address_Mode_Settings;
char Frame_Rate_and_inversion_control;

/// Power settings Arrays
char Power_Settings_Array[3];
char Vcom_Control_Array[3];
char Power_Settings_For_Norm[2];

#endif /* ILI9481_H_ */