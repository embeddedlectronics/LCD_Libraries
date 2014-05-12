/*
 * _SSD1289.c
 *
 * Created: 4/18/2013 3:56:29 PM
 *  Author: mfolz
 */ 


#include "_SSD1289.h"
// PAge 71 using 65k for the color
//0x60B0

// send the SPI commands to
void SSD1289_SendCommand(int command_address, int command_data){
	/// Send Command Address
	CLEARBIT(SSD_1289_DA_PORT, SSD_1289_RS); // pull RS low to send command
	SSD1289_SendPacket(command_address >> 8, command_address);
	//// Send Command Data
	SETBIT(SSD_1289_DA_PORT, SSD_1289_RS); // pull RS high to send data
	SSD1289_SendPacket(command_data >> 8, command_data);
}

void SSD1289_SendPacket(char LowerByte, char UpperByte){
	SETBIT(SSD_1289_RCW_PORT,SSD_1289_RS);
	SSD_1289_LB_PORT = LowerByte;
	SSD_1289_HB_PORT = UpperByte;
	CLEARBIT(SSD_1289_RCW_PORT, SSD_1289_WR); // pull WR low to send write data
	SETBIT(SSD_1289_RCW_PORT, SSD_1289_WR);
}

void SSD1289_Initalize(){
	struct LCD_Properties *pointProperties = &Properties;
	SSD_1289_LB_DDR = 0xFF;
	SSD_1289_HB_DDR = 0xFF;
	
	pointProperties->colorBytes = 16;
	pointProperties->xResolution = 240;
	pointProperties->yResolution = 320;

	SSD_1289_RCW_DDR = (1 << SSD_1289_CS) | (1 << SSD_1289_WR) | (1 << SSD_1289_RESET);
	SSD_1289_DA_DDR = (1 << SSD_1289_RS);
	
	SETBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	_delay_us(250);
	CLEARBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	_delay_us(250);
	SETBIT(SSD_1289_RCW_PORT, SSD_1289_RESET);
	_delay_us(250);
	
	SSD1289_SendCommand(Display_Control, 0x0021);
	SSD1289_SendCommand(Oscillator, 0x0001);
	SSD1289_SendCommand(Display_Control, 0x2B3F);
	SSD1289_SendCommand(Sleep_Mode, 0x0000);
	SSD1289_SendCommand(Display_Control, 0x0033);
	SSD1289_SendCommand(LCD_Waveform_Control, 0x0600);
	SSD1289_SendCommand(Driver_Output_Control ,0x2B3F); // check this
	SSD1289_SendCommand(Entry_Mode, 0x40B0);   //65k color  - type B color input // type c 8-R 8-G 8-B 8-blank


	SSD1289_SendCommand(Power_Control_1, 0x6664);
	SSD1289_SendCommand(Power_Control_2, 0x0000);
	SSD1289_SendCommand(Power_Control_3, 0x080C);
	SSD1289_SendCommand(Power_Control_4, 0x2B00);
	SSD1289_SendCommand(Power_Control_5, 0x0029);
	SSD1289_SendCommand(Compare_Register, 0x0000);
	SSD1289_SendCommand(Compare_Register_2, 0x0000);
	SSD1289_SendCommand(Horizontal_Porch, 0xEF1C);
	SSD1289_SendCommand(Frame_Frequency_Control, 0x6000);
	SSD1289_SendCommand(Vertical_Porch, 0x0003); // 0x0017 vertical porch
	SSD1289_SendCommand(Display_Control, 0x0233);  // can turn on and off the display; keeps image in gddr
	//SSD1289_SendCommand(0x000B, 0x0000);  // FRAMe cycle control

	SSD1289_SendCommand(Gate_Scan_Position, 0x0000); /// Starting position of the gate driver G0
	SSD1289_SendCommand(Gamma_Control_1,0x0707);
	SSD1289_SendCommand(Gamma_Control_2,0x0204);
	SSD1289_SendCommand(Gamma_Control_3,0x0204);
	SSD1289_SendCommand(Gamma_Control_4,0x0502);
	SSD1289_SendCommand(Gamma_Control_5,0x0507);
	SSD1289_SendCommand(Gamma_Control_6,0x0204);
	SSD1289_SendCommand(Gamma_Control_7,0x0204);
	SSD1289_SendCommand(Gamma_Control_8,0x0502);
	SSD1289_SendCommand(Gamma_Control_9,0x0302);
	SSD1289_SendCommand(Gamma_Control_10,0x0302);

	/// Scroll control and  screen driving Position
	SSD1289_SendCommand(Vertical_Scroll_Control, 0x0000);
	SSD1289_SendCommand(Vertical_Scroll_Control_2, 0x0000);
	SSD1289_SendCommand(Screen_driving_position, 0x0000);
	SSD1289_SendCommand(Screen_driving_position_2, 0x013F);
	SSD1289_SendCommand(Second_screen_driving_position, 0x0000);
	SSD1289_SendCommand(Second_screen_driving_position_2, 0x013F);
	
}


void SSD1289_Address_set()
{
	struct LCD_Geometry *pointCurrent = &Current;  /* To declare p as a pointer of type struct point */
	struct LCD_Properties *pointProperties = &Properties;  /* To declare p as a pointer of type struct point */
	
	SSD1289_SendCommand(Horizontal_Ram_Address_Pos,(pointCurrent->mem_x1Position<<8)+pointCurrent->mem_xPosition); // Horizontal ram address position
	SSD1289_SendCommand(Vertical_Ram_Address_Pos,pointCurrent->mem_yPosition);  // vertical ram address position
	SSD1289_SendCommand(Vertical_Ram_Address_Pos_2,pointCurrent->mem_y1Position);
	SSD1289_SendCommand(Horizontal_Ram_Address_X_Initial,pointCurrent->mem_xPosition); // initial settings for GDDRAM x
	SSD1289_SendCommand(Vertical_Ram_Address_Y_Initial,pointCurrent->mem_yPosition); // initial settings for GDDRAM y
	
	CLEARBIT(SSD_1289_DA_PORT, SSD_1289_RS); // pull RS low to send command
	SSD1289_SendPacket(0x00, 0x22);	  /// write to gddr
}




void SSD1289_SendPixel(int x, int y, long int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	int countx, county;
	int red, green, blue;
	pointCurrent->mem_xPosition= x; pointCurrent->mem_x1Position = (x+1);
	pointCurrent->mem_yPosition= y; pointCurrent->mem_y1Position = (y+1);
	
	red = (color >> 16);
	green = (color >> 8);
	blue = (color);
		// if xposition is greater than resolution return
	if (pointCurrent->mem_x1Position > pointProperties->xResolution || pointCurrent->mem_y1Position > pointProperties->yResolution){return;}

	CLEARBIT(SSD_1289_RCW_DDR, SSD_1289_CS);

	SSD1289_Address_set();
	SETBIT(SSD_1289_DA_PORT, SSD_1289_RS);
	SSD1289_SendPacket(red, green);
	SSD1289_SendPacket(blue, 0x00);
	SETBIT(SSD_1289_RCW_DDR, SSD_1289_CS);
}



void SSD1289_Fill_Rectangle(int x, int y, int width, int height, long int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	int red, green, blue;
	int drawrow, drawcolumn;
	
	// Left side Portriait 
	if (pointProperties->Rotation == 1)
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->xResolution || y > pointProperties->yResolution || y< 0 || x < 0)return;

		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width > pointProperties->xResolution) width= pointProperties->xResolution - (x);
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y);

		pointCurrent->mem_xPosition= (pointProperties->xResolution - (x+width));
		pointCurrent->mem_x1Position = (pointProperties->xResolution - x)-1;
		
		pointCurrent->mem_yPosition= y;
		pointCurrent->mem_y1Position = ((y+height)-1);
	}
	
	// Right side Portriait 
	else if (pointProperties->Rotation == 2)
	{	
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->xResolution || y > pointProperties->yResolution || y< 0 || x < 0)return;

		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width > pointProperties->xResolution) width= pointProperties->xResolution - (x+1);
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y);

		pointCurrent->mem_xPosition=x; 
		pointCurrent->mem_x1Position =(x+width)-1;
			
		pointCurrent->mem_yPosition= (pointProperties->yResolution - (y+height));
		pointCurrent->mem_y1Position = (pointProperties->yResolution - y)-1;
	}
	
	// Upside down  Landscape
	else if (pointProperties->Rotation == 3)
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->yResolution || y > pointProperties->xResolution || y< 0 || x < 0)return;
		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width  > pointProperties->yResolution) width = pointProperties->yResolution - (x);
		if(height+y > pointProperties->xResolution) height= pointProperties->xResolution -(y);
				
		// Put display in landscape mode swap the height and width values
		pointCurrent->mem_xPosition= (pointProperties->xResolution - (y+height)); 
		pointCurrent->mem_x1Position = (pointProperties->xResolution - y)-1;
		pointCurrent->mem_yPosition= (pointProperties->yResolution - ((x+width)));
		pointCurrent->mem_y1Position = (pointProperties->yResolution -x)-1;	
	}
	
	
	// default rotation LandScape
	else 
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->yResolution || y > pointProperties->xResolution || y< 0 || x < 0)return;
		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width  > pointProperties->yResolution) width = pointProperties->yResolution - (x);
		if(height+y > pointProperties->xResolution) height= pointProperties->xResolution -(y);
		
		// Put display in landscape mode swap the height and width values
		pointCurrent->mem_xPosition= y; 
		pointCurrent->mem_x1Position = ((y+height)-1);
		pointCurrent->mem_yPosition= x; 
		pointCurrent->mem_y1Position = ((x+width)-1);

	}
	
	
	

	red = (color >> 16);
	green = (color >> 8);
	blue = (color);
	// Set the lcd address
	CLEARBIT(SSD_1289_RCW_DDR, SSD_1289_CS);
	SSD1289_Address_set();
	SETBIT(SSD_1289_DA_PORT, SSD_1289_RS);
	
	for(drawcolumn=0; drawcolumn<=height; drawcolumn++){
		for (drawrow=0; drawrow<=width; drawrow++)
		{
			SSD1289_SendPacket(red, green);
			SSD1289_SendPacket(blue, 0x00);
		}
	}
	SETBIT(SSD_1289_RCW_DDR, SSD_1289_CS);
}



void Swapvalues(void){
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	
	pointCurrent->xPosition = pointCurrent->mem_xPosition +  pointCurrent->yPosition; 
	pointCurrent->yPosition = pointCurrent->mem_xPosition - pointCurrent->yPosition;
	pointCurrent->xPosition = pointCurrent->mem_xPosition - pointCurrent->yPosition;

}