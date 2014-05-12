/*
 * LCD_Draw.h
 *
 * Created: 3/26/2013 5:51:24 PM
 *  Author: mfolz
 
 This Library currently work with the SSD1289, ST7735 and ST7735R -- Will be adding more libraries soon 
 -----------------------------------------------------------------
		color: 
		- Before you use any of the functions from this library use the
		corresponding library. colors may be put in 24bit however only 18-16bits worth
		of that color counts.  0xRRGGBB color.
		
		x,y,width and height:
		-Send width and height (make sure initial xCorridnate+width or yCorinate+height
		are within the valid LCD Resolutions)
		
		LCD_SendPixel():
			-this will draw a 1x1 pixel anyplace that is valid.
		
		LCD_Fill_Rectangle();  // fast library
			-draws a rectangle anywhere on the screen within the screens valid resolution. 
		
		LCD_Draw_Line(); 
			-Draws a line from any initial point (x0,y0) - > (x1, y1)
		
		LCD_Write_PGM();
			-Takes in a string from the program memory (long strings can take up valuable space on the ram)
			prints out on a string(no limit to the screen wont write chars beyond a valid resolution)
			
		LCD_Write_Line(); 
			-Same as the PGM, only difference is you can place a string directly without using PGM
		
		LCD_FillScreen(); 
			-Self explanatory, fills screen with desired color, again 0xRRGGBB color 
		
		LCD_Write_Char() 
			- I do not recommend using this function, you will need to set the variables for the structures. 
 */ 


#ifndef LCD_DRAW_H_INCLUDED_
#define LCD_DRAW_H_INCLUDED_


void LCD_SendPixel(int x, int y, long int color);
void LCD_FillScreen (int color);
void LCD_Fill_Rectangle(int x, int y, int width, int height, long int color);
void LCD_Draw_Line(int x0, int y0, int x1, int y1, int size, long int color);
void LCD_Write_PGM(const char *string, int x, int y, int size, long int color);
void LCD_Write_Line(char *string, int x, int y, int size, long int color);
void LCD_Write_Char(char c);


/// UNFINISHED LIBS
void LCD_Draw_Shape(int x0, int y0, int radius, long int color, int shapeflag);
void LCD_Draw_EllipseRect(int x0, int y0, int x1, int y1, long int color);
void LCD_Draw_FillCircle(int x0, int y0, int radius, int fill, long int color);

void LCD_Write_Style_Line(char *string, int x, int y, int size, long int color); 
void LCD_Write_Style_Char(unsigned char c);


void LCD_plotEllipseRect(int x0, int y0, int x1, int y1, long int color);
void LCD_Draw_FastSprite(int x, int y, int size, const char *sprite_array, const long int *sprite_pallet);

long int colorArray[]; 



struct LCD_Properties{
	int xResolution;
	int yResolution;
	int colorBytes;
	char Rotation;
}Properties;

struct LCD_Geometry{
	int xPosition;
	int yPosition;
	int mem_xPosition;
	int mem_yPosition;
	int mem_x1Position;
	int mem_y1Position;
	int size;
	long int color;
}Current;

#endif /* LCD_DRAW_H_ */