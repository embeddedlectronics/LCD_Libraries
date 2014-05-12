/*
 * ILI9481.c
 *
 * Created: 25/11/2013 8:43:31 AM
 *  Author: Michael
 */ 
#include "ILI9481.h"


char Exit_Sleep_Mode_Settings= 0x11;
char Gamma_Settings_array[12] = { 0x00,0x32,0x36,0x45,0x06,0x16,0x37,0x75,0x77,0x54,0x0C,0x00};

// Initalize resoultion arrays
char X_Max_Column_Settings[4] = {0x00, 0x00, 0x01, 0x40}; // 320
char Y_Max_Row_Settings[4] = {0x00, 0x00, 0x01, 0xE0}; // 480 
	
	
char X_Max[4] = {0x00, 0x00, 0x00, 0x01}; // 320
char Y_Max[4] = {0x00, 0x00, 0x00, 0x01}; // 480

// Pannel Driving arays 
char Panel_Driving_Settings[5] = {0x10, 0x3B, 0x00, 0x02, 0x11 } ; 

char COLOR_array[3] = {0x00, 0xFF, 0xFF,0xFF};

char Set_Pixle_Format_Settings = 0x66; 
char Set_address_Mode_Settings = 0x00; 
char Frame_Rate_and_inversion_control = 0x03; 

/// Power settings Arrays 
char Power_Settings_Array[3] = {0x07, 0x42, 0x18};
char Vcom_Control_Array[3] = {0x00, 0x07, 0x10};
char Power_Settings_For_Norm[2] = {0x01, 0x02};
 

// FOR ALL INSTANCES RESX IS HIGH & CSX IS LOW 
// DC Low for initial Command
// RDX = HIGH 
// WRX = PULL HIGH
void ILI9481_SendCommand_Single(char command_address, int command_data){
	char Lower_Byte =0; 
	char Upper_Byte =0; 
	
	Lower_Byte = command_data & 0xFF; 
	Upper_Byte = (command_data >> 8) & 0xFF; 
	
	CLEARBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
	/// pull respective command lines here
	CLEARBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS LOW COMMAND
	ILI9481_SendPacket(command_address, 0x00);
	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR COMMAND
	
	// send command paramaters
	SETBIT(ILI9481_RS_PORT, ILI9481_RS);
	ILI9481_SendPacket(Lower_Byte, Upper_Byte);

	// PULL WRX HIGH
	SETBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
}

// useful for sending commands only no pixles or boxes or anything
void ILI9481_SendCommand(char command_address, char *command_data, char length){

	
	CLEARBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
	
	/// pull respective command lines here 
	CLEARBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS LOW COMMAND
	ILI9481_SendPacket(command_address, 0x00); 
	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR COMMAND
	
	// send command paramaters
	for (char command_data_count = 0; command_data_count < length; command_data_count++)
	{
		 ILI9481_SendPacket((char)command_data[command_data_count], 0x00);
	}
	// PULL WRX HIGH
	SETBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
}

void ILI9481_SendPacket(char LowerByte, char UpperByte){
	ILI9481_LB_PORT = LowerByte; 
	ILI9481_HB_PORT = UpperByte; 
	
	//PULL WRX HIGH
	CLEARBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_WR);
	_delay_us(20);
    SETBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_WR);
	// PULL WRX LOW 
	
}


// ALL commands are fine! 
void ILI9481_Initalize(void){
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
		
	ILI9481_CS_WR_RESET_DDR |= (1 << ILI9481_CS) |  (1 << ILI9481_WR) |  (1 << ILI9481_RESET) ;
	ILI9481_RS_DDR =  (1 << ILI9481_RS);  
	ILI9481_LB_DDR |= 0xFF;
	ILI9481_HB_DDR |= 0xFF;
	
	ILI9481_RS_PORT=  (1 << ILI9481_RS);  
	ILI9481_CS_WR_RESET_PORT |= (1 << ILI9481_CS) |  (1 << ILI9481_WR) |  (1 << ILI9481_RESET) ;
	ILI9481_CS_WR_RESET_PORT |= (1 << ILI9481_RESET);
	
		
	pointProperties->colorBytes = 16;
	pointProperties->xResolution = 320;
	pointProperties->yResolution = 480;

	
	ILI9481_SendCommand_Single(EXIT_SLEEP_MODE, Exit_Sleep_Mode_Settings);
	_delay_ms(120); // must wait 120 mili seconds
	ILI9481_SendCommand(POWER_SETTING, Power_Settings_Array, 3);
	ILI9481_SendCommand(VCOM_CONTROL, Vcom_Control_Array, 3);
	ILI9481_SendCommand(POWER_SETTING_FOR_NORMAL_MODE,  Power_Settings_For_Norm, 2);
	
	ILI9481_SendCommand(PANEL_DRIVING_SETTING, Panel_Driving_Settings, 5);
	ILI9481_SendCommand_Single(FRAME_RATE_AND_INVERSION_CONTROL, Frame_Rate_and_inversion_control);
	ILI9481_SendCommand(GAMMA_SETTING, Gamma_Settings_array, 12); 
	
	ILI9481_SendCommand_Single(SET_ADDRESS_MODE, 0x40);
	ILI9481_SendCommand_Single(SET_PIXLE_FORMAT,Set_Pixle_Format_Settings); 
	ILI9481_SendCommand(SET_COLUMN_ADDRESS, X_Max_Column_Settings, 4);
	ILI9481_SendCommand(SET_ROW_ADDRESS, Y_Max_Row_Settings, 4);

	ILI9481_SendCommand_Single(SET_DISPLAY_ON, Set_address_Mode_Settings); 
	
}
// 
// void ILI9481_Address_set(void){
//   // Store x and y in ints, 
//   // place ints in an array 
//   	ILI9481_SendCommand(SET_COLUMN_ADDRESS, X_Max, 4);
//   	ILI9481_SendCommand(SET_ROW_ADDRESS, Y_Max, 4);
// }

//void LCD_SendPixel(int x, int y, long int color)
//SSD1289_SendPixel(x,y,color);


void ILI9481_SendPixel(int x, int y, long int color){
	//ILI9481_Address_set();
	//ILI9481_SendCommand(WRITE_MEMORY_START, COLOR_array, 4);
	struct LCD_Properties *pointProperties = &Properties;
	struct LCD_Geometry *pointCurrent = &Current;
	
	int red, green, blue;

	pointCurrent->mem_xPosition= x; pointCurrent->mem_x1Position = (x+1);
	pointCurrent->mem_yPosition= y; pointCurrent->mem_y1Position = (y+1);
	
	if (pointCurrent->mem_x1Position > pointProperties->xResolution || pointCurrent->mem_y1Position > pointProperties->yResolution){
		return;}

	red = color >> 16;
	green = color >> 8;
	blue = color;
	
	CLEARBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
	ILI9481_Address_set();

	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR data  // BGR  green 
	ILI9481_SendPacket(green,blue);
	ILI9481_SendPacket(0x00,red);

	SETBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
}

void ILI9481_Address_set(void){
	struct LCD_Geometry *pointCurrent = &Current;  /* To declare p as a pointer of type struct point */
	struct LCD_Properties *pointProperties = &Properties;  /* To declare p as a pointer of type struct point */
	
	CLEARBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS LOW COMMAND
	ILI9481_SendPacket(0x2a, 0x00);

	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR data
	ILI9481_SendPacket((pointCurrent->mem_xPosition >>8) ,0x00); // start -upper 
	ILI9481_SendPacket((pointCurrent->mem_xPosition) ,0x00); // start- lower 
	ILI9481_SendPacket((pointCurrent->mem_x1Position>>8) ,0x00);  // end -upper 
	ILI9481_SendPacket(pointCurrent->mem_x1Position ,0x00); // end - lower
	
	CLEARBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS LOW COMMAND
	ILI9481_SendPacket(0x2b, 0x00);

	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR data
	ILI9481_SendPacket((pointCurrent->mem_yPosition>>8) ,0x00);
	ILI9481_SendPacket((pointCurrent->mem_yPosition) ,0x00);
	ILI9481_SendPacket(((pointCurrent->mem_y1Position)>>8) ,0x00);
	ILI9481_SendPacket((pointCurrent->mem_y1Position) ,0x00);
	

	/// Set Write to the GDDR All Packets sent afterwards will be taken as input color
	CLEARBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS LOW COMMAND
	ILI9481_SendPacket(0x2c, 0x00);
	SETBIT(ILI9481_RS_PORT, ILI9481_RS);  // PULL RS HIGH FOR data // send pixle data after this routine 
	
}



void ILI9481_Fill_Rectangle(int x, int y, int width, int height, long int color){
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
		if (x+width > pointProperties->xResolution) width= pointProperties->xResolution - (x+1);
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y+1);

		pointCurrent->mem_xPosition= x;
		pointCurrent->mem_x1Position = (x+width)-1;
		
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
		if(height+y > pointProperties->yResolution) height = pointProperties->yResolution -(y+1);

		pointCurrent->mem_xPosition= pointProperties->xResolution - (x+width); 
		pointCurrent->mem_x1Position =  pointProperties->xResolution - x-1;
			
		pointCurrent->mem_yPosition= (pointProperties->yResolution - (y+height));
		pointCurrent->mem_y1Position = (pointProperties->yResolution - y)-1;
	}
	
	// Upside down  Landscape
	else if (pointProperties->Rotation == 3)
	{
		//  Do not display anything outside of the displays GDDR
		if (x > pointProperties->yResolution || y > pointProperties->xResolution || y< 0 || x < 0)return;
		/// Proceed to draw the shape within the display, block out any other area.
		if (x+width  > pointProperties->yResolution) width = pointProperties->yResolution - (x+1);
		if(height+y > pointProperties->xResolution) height= pointProperties->xResolution -(y+1);
				
		// Put display in landscape mode swap the height and width values
		pointCurrent->mem_xPosition= (pointProperties->xResolution - (y+height)); 
		pointCurrent->mem_x1Position = (pointProperties->xResolution - y)-1;
		pointCurrent->mem_yPosition = x; 
		pointCurrent->mem_y1Position =(x+width)-1;	
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
		pointCurrent->mem_yPosition=  (pointProperties->yResolution - ((x+width)));
		pointCurrent->mem_y1Position =(pointProperties->yResolution -x)-1;	

	}
	
	
	
	red = (color >> 16);
	green = (color >> 8);
	blue = (color);
	
	// Set the lcd address
	CLEARBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
	ILI9481_Address_set(); 
	
	for(drawcolumn=0; drawcolumn<=(height); drawcolumn++){
		for (drawrow=0; drawrow<=(width); drawrow++)
		{
			ILI9481_SendPacket(green,blue); //1
			ILI9481_SendPacket(blue,red); //1-2
			ILI9481_SendPacket(red,green); //2 			
		}
	}
	SETBIT(ILI9481_CS_WR_RESET_PORT, ILI9481_CS);
}


//ILI9481_SendPacket(green,blue);
//ILI9481_SendPacket(0x00,red);