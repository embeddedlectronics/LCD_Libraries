

#ifndef ST7735_SPI_h_INCLUDED_
#define ST7735_SPI_h_INCLUDED_
#include <avr/pgmspace.h>
#include <util/delay.h>


/// Declaire the LCD properties here ///
void ST7735_Properties(int xResolution, int yResolution, int Colorbytes, char Rotation);
//////////////////////////////////////////////////////////////////////////
// This library uses 16 bit colors, anything higher slows the display down by 1/2
// it takes the display clocked at 4mHz approx 500ms to draw every pixel on the screen.





void ST7735_SPI_Send(char data);  // send data
void ST7735_Set_Address(); //

void ST7735_Geometry2(int xPosition, int yPosition, int size, long int color );

void ST7735_Initalize(); /// initialize
void ST7735_Write_Data(char data); /// data
void ST7735_Write_Command(char command); // command
void ST7735_Send_Pixel(void);

void ST7735_DrawPixel(int x, int y, long int color);

// Fast Libs
void ST7735_Draw_FastRect(int x, int y, int width, int height, long int color);
void color_return(long int color);


#define ST7735_NOP 0x0
#define ST7735_SWRESET 0x01  // Software reset applied to display during sleep out, or display on mode, 120ms wait before next command (page 83)
#define ST7735_RDDID 0x04  // Read Display id (page 84)
#define ST7735_RDDST 0x09  // Read display status, (useful for debugging )
// * 0x0A - 0x0F there are many read commands, since i was going for a simple write only i did not use any of these commands.
// as most people wont.
#define ST7735_SLPIN  0x10  // Enter sleep mode, LCD enters min power mode 120ms must be waited for next command (page 93)
#define ST7735_SLPOUT  0x11 // Wake from sleep mode, again 120ms (page 94)
#define ST7735_PTLON  0x12  // Partial display mode on, designate the partial display area with command 0x30 (pg 95)
#define ST7735_NORON  0x13  // Normal display mode on (pg 96)

#define ST7735_INVOFF 0x20 // invert the display off
#define ST7735_INVON 0x21   // invert the display on
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RGBSET 0x2D
#define ST7735_RAMRD 0x2E

#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36


#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#endif


