


#include "ST7735_SPI.h"
#include "../AtmegaLibs/SPI.h"  // SPI library 
#include "../AtmegaLibs/Serial_Atmel328p.h" // Serial used for debugging 
#include "../AtmegaLibs/AtmegaPins.h"
#include "LCD_Draw.h"

#include <avr/pgmspace.h>  // this is needed for the program memory
#include <util/delay.h>  // Delay
#include <math.h>

#define F_CPU 16000000UL  //16Mhz clock 


#define SPI_CS PORTD3 // Pin10 Chip Select 
#define SPI_RS PORTD5   // PIN9  RS PIN used for chosing data or command  
#define SPI_RST PORTD4 // PIN8
// ARDUINO PINOUT// Make sure you match your pins with the following. feel free to change RS and Reset
// RST-RESET	 -- Pin 5   // Can be changed 
// RS or DC      -- Pin 6   // can be changed 
// Chip Select   -- Pin 7   
// MOSI          -- Pin 11
// Clock         -- Pin 13
#define ST7735_PINS_DDR DDRD   // data direction port for the pins 
#define ST7735_PINS PORTD
// Please note this library uses 16 bit color and not 18, i found the display was far too slow 
// when using 18 bit colors as i have an extra byte to transmit for 1 more bit on red and blue. 


void ST7735_Draw_FastRect(int x, int y, int width, int height, long int color){
	struct LCD_Geometry *pointCurrent = &Current;
	int drawcolumn =0, drawrow=0;
	pointCurrent->mem_xPosition= x; pointCurrent->mem_x1Position = ((x+width)-1);
	pointCurrent->mem_yPosition= y; pointCurrent->mem_y1Position = ((y+height)-1);
	
	ST7735_Set_Address();  /// Send start and end addresses
	pointCurrent->color = color;
	
	for (drawcolumn= 0; drawcolumn <= height; drawcolumn++)
	{
		for (int drawrow =0 ; drawrow <= width; drawrow++)
		{
			ST7735_Send_Pixel();
		}
		drawrow = 0;
	}
}



void ST7735_Properties(int xResolution, int yResolution,  int colorBytes, char Rotation)
{
	struct LCD_Properties *point = &Properties;
	point->xResolution = xResolution;
	point->yResolution = yResolution;
	
	switch (colorBytes){
		case 12:
		point->colorBytes= 0x03;   //12 bit
		break;
		case 16:
		point->colorBytes= 0x05;   //16 bit
		break;
		case 18:
		point->colorBytes= 0x06;   //18 bit
		break;
		default:
		point->colorBytes=0x05;     // default 16bit
		break;
	}
	
	if (Rotation == 'L' | Rotation == 'l'){
		point->Rotation = 0x60;
		point->xResolution+= point->yResolution;
		point->yResolution= point->xResolution - point->yResolution;
		point->xResolution= point->xResolution - point->yResolution;
	}   // left
	
	else if (Rotation == 'R' | Rotation == 'r'){  // right
		point->Rotation = 0xA0;
		point->xResolution+= point->yResolution;
		point->yResolution= point->xResolution - point->yResolution;
		point->xResolution= point->xResolution - point->yResolution;
	}   // left
	
	
	else if (Rotation == 'T' | Rotation == 't')point->Rotation = 0xC0;  // Upright
	else if (Rotation == 'B' | Rotation == 'b')point->Rotation = 0x00;   // Upside down
	else point->Rotation = 0xC0;
}


void ST7735_DrawPixel(int x, int y, long int color){
	struct LCD_Geometry *point = &Current; // point to the
	point->mem_xPosition = x;
	point->mem_x1Position = x+1;
	point->mem_yPosition = y;   // draw pixel by pixel
	point->mem_y1Position = y+1;
	point->color = color; 
	ST7735_Set_Address();
	ST7735_Send_Pixel();
} 


void ST7735_Set_Address()
{
	struct LCD_Geometry *pointCurrent = &Current;
	ST7735_Write_Command(ST7735_CASET); // Colunmn Address Set command
	ST7735_Write_Data(0x00);  // first bit is always dont care
	ST7735_Write_Data(pointCurrent->mem_xPosition);   // Starting Address X axis.
	ST7735_Write_Data(0x00);  // first bit is always dont care
	ST7735_Write_Data(pointCurrent->mem_x1Position);   // Ending Addresss X axis.
	
	ST7735_Write_Command(ST7735_RASET);
	ST7735_Write_Data(0x00);  // first bit is always dont care
	ST7735_Write_Data(pointCurrent->mem_yPosition);   // Starting Address Y axis.
	ST7735_Write_Data(0x00);  // first bit is always dont care
	ST7735_Write_Data(pointCurrent->mem_y1Position);   // Ending Addresss Y axis.
	
	ST7735_Write_Command(ST7735_RAMWR);  // what ever is sent next is written to the ram
}

void ST7735_Send_Pixel(void){
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	color_return(pointCurrent->color);
	
	if (pointProperties ->colorBytes == 0x03){
		// put logic here for shitty 12 bit color
		ST7735_Write_Data(pointCurrent->color>>4); // send first 8 bytes
		ST7735_Write_Data(pointCurrent->color); /// send last 8 bytes
	}
	
	else if (pointProperties ->colorBytes == 0x05){
		ST7735_Write_Data(pointCurrent->color>>8); // send first 8 bytes
		ST7735_Write_Data(pointCurrent->color); /// send last 8 bytes
	}
	
	else if (pointProperties ->colorBytes == 0x06){
		ST7735_Write_Data(pointCurrent->color>>16); // send first 8 bytes
		ST7735_Write_Data(pointCurrent->color>>8); // send next 8 bytes
		ST7735_Write_Data(pointCurrent->color); /// send last 8 bytes
	}

}

void ST7735_Fill_Rectangle(int x, int y, int width, int height, long int color){
	int xStart=0; 	int yStart=0;
	struct LCD_Geometry *pointCurrent = &Current;
	pointCurrent->color= color;

	for (xStart = 0; xStart < (width); xStart++){ /// draw rows second
		for(yStart = 0; yStart < (height); yStart++){ // draw columns first at height length
			pointCurrent->xPosition = (x+xStart);
			pointCurrent->yPosition = (y+yStart);
			pointCurrent->mem_xPosition= (pointCurrent->xPosition);
			pointCurrent->mem_x1Position= ((pointCurrent->xPosition)+1);
			pointCurrent->mem_yPosition= (pointCurrent->yPosition);
			pointCurrent->mem_y1Position= ((pointCurrent->yPosition)+1);
			
			ST7735_Set_Address();
			ST7735_Send_Pixel();  // Call the draw pixel function draw
		}
	}
}




////// WRITE DATA
/// to write data RS needs to be high, CS needs to be low and rst needs to stay high
void ST7735_Write_Data(char data){
	/// To write data we need to pull the rs-pin high
	CLEARBIT(ST7735_PINS,SPI_CS);
	//ST7735_PINS |= (1 << SPI_RS) (0 << SPI_CS) |(1 << SPI_RST);  /// pull RS, and RST high pull CS low
	SPI_Master_Transmit(data);  /// Transmit the 8 bit data
	SETBIT(ST7735_PINS,SPI_CS);
	//ST7735_PINS = (1 << SPI_RS) | (1 << SPI_CS) |(1 << SPI_RST);  // pull CS high
	return;
}

///// WRITE COMMAND
//// to write a command, rs needs to be low, cs low and rst must stay high
void ST7735_Write_Command(char command){
	/// to write a command we need to pull the rs-pin low
	CLEARBIT(ST7735_PINS,SPI_CS);
	CLEARBIT(ST7735_PINS,SPI_RS);
	//ST7735_PINS = (0 << SPI_RS) & (0 << SPI_CS) | (1 << SPI_RST);  /// Pull both RS and CS low, keep RST high
	SPI_Master_Transmit(command);  /// Send command
	//ST7735_PINS = (1 << SPI_RS) | (1 << SPI_CS) | (1 << SPI_RST);  /// Pull RS, CS and RST high
	SETBIT(ST7735_PINS,SPI_CS);
	SETBIT(ST7735_PINS,SPI_RS);
	return;
}

//// DRAW pixle,  make sure color is 16 bits   RGB
/// R - 5 bits,  G - 6 bits,  b- 5 bits,

void ST7735_Initalize(){
	SPI_MasterInit();
	struct LCD_Properties *pointProperties = &Properties;
	ST7735_PINS_DDR |= (1 << SPI_CS) | (1 << SPI_RS) | (1 << SPI_RST); // Set Cs, rs a
	SETBIT(ST7735_PINS,SPI_RST);   // pull RST HIGH

	ST7735_Write_Command(ST7735_SWRESET); // software reset
	_delay_ms(120); // after sw reset is initiated the device will need 120ms

	ST7735_Write_Command(ST7735_SLPOUT);  // out of sleep mode
	_delay_ms(120);

	ST7735_Write_Command(ST7735_FRMCTR1);  // frame rate control - normal mode  // frame rate = 120hz ish
	ST7735_Write_Data(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	ST7735_Write_Data(0x2C); //5082
	ST7735_Write_Data(0x2D);

	ST7735_Write_Command(ST7735_FRMCTR2);  // frame rate control - idle mode
	ST7735_Write_Data(0x01);  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D)
	ST7735_Write_Data(0x2C);
	ST7735_Write_Data(0x2D);

	ST7735_Write_Command(ST7735_FRMCTR3);  // frame rate control - partial mode
	ST7735_Write_Data(0x01); // dot inversion mode
	ST7735_Write_Data(0x2C);
	ST7735_Write_Data(0x2D);
	ST7735_Write_Data(0x01); // line inversion mode
	ST7735_Write_Data(0x2C);
	ST7735_Write_Data(0x2D);
	
	ST7735_Write_Command(ST7735_INVCTR);  // display inversion control
	ST7735_Write_Data(0x07);  // Line inversion for all three settings

	ST7735_Write_Command(ST7735_PWCTR1);  // power control PG 130
	ST7735_Write_Data(0xA2);      // 5v to 4.6V
	ST7735_Write_Data(0x02);      // -4.6V
	ST7735_Write_Data(0x84);      // AUTO mode

	ST7735_Write_Command(ST7735_PWCTR2);  // power control
	ST7735_Write_Data(0xC5);      // VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD     //25v  well within spec

	ST7735_Write_Command(ST7735_PWCTR3);  // power control  0xC2
	ST7735_Write_Data(0x1B);      // Opamp current medium
	ST7735_Write_Data(0x00);      // Boost frequency

	ST7735_Write_Command(ST7735_PWCTR4);  // power control 0xC3  in idle mode
	ST7735_Write_Data(0x8A);      // BCLK/2, Opamp current small & Medium low
	ST7735_Write_Data(0x2A);

	ST7735_Write_Command(ST7735_PWCTR5);  // power control
	ST7735_Write_Data(0x1B);
	ST7735_Write_Data(0x00);

	ST7735_Write_Command(ST7735_VMCTR1);  // power control
	ST7735_Write_Data(0x3F);  // Vcom = -1.2v

	ST7735_Write_Command(ST7735_INVOFF);    // don't invert display

	ST7735_Write_Command(ST7735_MADCTL);  // memory access control (directions) aka ROTATION
	ST7735_Write_Data(pointProperties->Rotation);  // row address/col address, bottom to top refresh   // needs to be 08 for the non R revision
	
	ST7735_Write_Command(ST7735_COLMOD);  // set color mode
	ST7735_Write_Data(pointProperties->colorBytes);        // 16-bit color

	ST7735_Write_Command(ST7735_CASET);  // column addr set
	ST7735_Write_Data(0x00);
	ST7735_Write_Data(0x00);   // XSTART = 0   (128x160 display)
	ST7735_Write_Data(0x00);
	ST7735_Write_Data(pointProperties->xResolution);   // XEND = 127

	ST7735_Write_Command(ST7735_RASET);  // row addr set
	ST7735_Write_Data(0x00);
	ST7735_Write_Data(0x00);    // XSTART = 0
	ST7735_Write_Data(0x00);
	ST7735_Write_Data(pointProperties->yResolution);    // XEND = 159 /.
	
	ST7735_Write_Command(0x26);
	ST7735_Write_Data(0x01); // gamma curve 1 //standard curve
	
	ST7735_Write_Command(ST7735_GMCTRP1);
	for(int x=0; x < 16; x++) {ST7735_Write_Data(0x00);} //set the gamma at its lowest, the display looks like shit otherwise

	ST7735_Write_Command(ST7735_GMCTRN1);
	for(int x=0; x < 16; x++) {ST7735_Write_Data(0x00);}  //set the gamma at its lowest, the display looks like shit otherwise
	
	ST7735_Write_Command(ST7735_DISPON);
	ST7735_Write_Command(ST7735_NORON);  // normal display mode
	_delay_ms(120);
	return;
}
////////////////////////////////////////////////////////////
////////////// Color Libraries  ////////////////////////////
////////////////////////////////////////////////////////////

// another method where you pass in the 24bit color int. this returns a long int 16bit number.
// this simple function made my life easier
void color_return(long int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;  /* To declare p as a pointer of type struct point */
	long int red=0, blue=0, green =0;
	long int redmask, greenmask, bluemask;
	int redshift, blueshift, greenshift;
	
	switch (pointProperties->colorBytes){
		case 0x03:
		redshift= 12; greenshift= 8; blueshift=4;
		redmask= 0xF00000; greenmask= 0x00F000; bluemask= 0x000F0;
		break;
		
		case 0x05:
		redshift= 8; greenshift= 5; blueshift=3;
		redmask= 0xF80000; greenmask= 0x00FC00; bluemask= 0x000F8;
		break;
		
		case 0x06:
		redshift= 0; greenshift= 0; blueshift=0;
		redmask= 0xFF0000; greenmask= 0x00FF00; bluemask= 0x000FF;
		break;
	}
	
	red = color;
	red &= redmask;
	red >>= redshift;
	
	green = color;
	green &= greenmask;
	green >>= greenshift;
	
	blue = color;
	blue &= bluemask;
	blue >>= blueshift;
	

	pointCurrent->color = (red+blue+green);
}
