/*
 * _SSD1289 
 *
 * Created: 3/24/2013 6:23:55 PM
 *  Author: Michael
 This SSD1289 Driver library is meant to be used with the LCD_Draw.h library:
        - Always initalize the LCD before you call any functions from the LCD_Draw Library
		- The library uses parallel programming, not spi. 
		- 18 bit color (262,144 colors) colors are defined 0xRRGGBB  
		- Use the LCD_Geometry structure to define the variables before you pass them


 ---------------------------------------------------------------------------
 I have made the 
 
 
 
 */ 

#ifndef _SSD1289_H_INCLUDED_
#define _SSD1289_H_INCLUDED_
/// Configuration Drivers 

#include "LCD_Draw.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/delay.h>




/// Registry commands -> see data sheet for proper configuration. 
#define Oscillator 	0x0000  // write 1 to enable
#define Driver_Output_Control  	0x0001 // pg 27
#define LCD_Waveform_Control 	0x0002 //pg 31
#define Power_Control_1	  0x0003 // pg 34
#define Compare_Register	0x0005
#define Compare_Register_2	0x0006 // pg 35
#define Display_Control 	0x0007 // IMPORTATNT PAge 36
#define Frame_Cycle_Control	0x0008 // Page 36
#define Power_Control_2	   0x000C // pg 38 0x0008/ max voltage
#define Power_Control_3	  0x000D // pg 39 0x0009/ max voltage
#define Power_Control_4	  0x000E // pg 39 0x0008/ max voltage
#define Gate_Scan_Position			0x000F // pg 40 // set the starting position of the gate driver 0-319
#define Sleep_Mode					0x0010 // sleep mode, internal operations are halted to reduce current consumption
#define Entry_Mode					0x0011 // Pg 40 IMPORTANT
#define Generic_Interface_Control	0x0015 // pg 43
#define Horizontal_Porch			0x0016 // pg 44  //
#define Vertical_Porch			    0x0017	// pg 45  0xFE00; useful for vsync only
#define Power_Control_5				0x001E // pg 46 0x0029;
#define Write_GRAM					0x0022 	// 18 bit data stores pixle data -needs address doe
#define Frame_Frequency_Control		0x0025 // internal frequency
#define Vertical_Scroll_Control		0x0041
#define Vertical_Scroll_Control_2	0x0042 // pg 49  ?/?
#define Horizontal_Ram_Address_Pos	0x0044 /// write here to specify the start and end points of the window address
// First 8 bits represent the ending address, last 8bits represent the starting address
#define Vertical_Ram_Address_Pos	0x0045  // Starting Address  9bits long
#define Vertical_Ram_Address_Pos_2	0x0046  //Ending address   9 bits long
#define Horizontal_Ram_Address_X_Initial		0x004E // Y initial
#define Vertical_Ram_Address_Y_Initial		0x004F // Y initial
#define Screen_driving_position		0x0048
#define Screen_driving_position_2	0x0049
#define Second_screen_driving_position 0x004A
#define Second_screen_driving_position_2 0x004B
#define Ram_Address_Set_X			0x004E // 8 
#define Ram_Address_Set_Y			0x004F // 9 bits of data for the y position of the pixel


#define	   Gamma_Control_1  0x0030
#define	   Gamma_Control_2  0x0031
#define	   Gamma_Control_3 0x0032
#define	   Gamma_Control_4 0x0033
#define	   Gamma_Control_5 0x0034
#define	   Gamma_Control_6  0x0035
#define	   Gamma_Control_7  0x0036
#define	   Gamma_Control_8 0x0037
#define	   Gamma_Control_9  0x003A
#define	   Gamma_Control_10  0x003B



///// Mode and Addressing Pins
// This pinout is ment for the TFT LCD mega shield 2.0
// see the pinout for the lcd and compare it to the atmega2560
#define SSD_1289_RS	PIND7     
#define SSD_1289_WR	PING2
#define SSD_1289_CS	PING1
#define SSD_1289_RESET PING0

#define SSD_1289_RCW_DDR DDRG
#define SSD_1289_RCW_PORT PORTG
#define SSD_1289_DA_DDR	DDRD
#define SSD_1289_DA_PORT PORTD


#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))


/// Data transmission pins
#define SSD_1289_LB_DDR DDRA
#define SSD_1289_LB_PORT PORTA

#define SSD_1289_HB_DDR DDRC
#define SSD_1289_HB_PORT PORTC
//LCD Libraries 
void SSD1289_Initalize();
void SSD1289_SendCommand(int command_address, int command_data);
void SSD1289_WriteAddress(int data_address, char r, char g, char b);
void SSD1289_SetRotation(void);

long int color_return(long int color); 
long int color16(long int color);


//////////////////////////////////////////////////////////////////////////
void Swapvalues();

#endif /* _SSD1289_H_ */