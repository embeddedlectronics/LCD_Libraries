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
			
		LCD_Write_Style_Line();
			- Same as the write line except you will be using arial font. Do not use ~ or / keys. they will not print properly
			- Will make updates to allow other font
			
----------------------------------------------------------------------------------

	
 */ 

#include "ArialFont.h"
#include "font.h"
#include "LCD_Draw.h"
#include "LCD_Config.h"

long int colorArray[] = {
	0xFFFFFF		//	White
	, 0xFFFF00		//	Yellow
	, 0xFF00FF		//	Fuchsia
	, 0xFF0000		//	Red
	, 0xC0C0C0		//	Silver
	, 0x808080		//	Gray
	, 0x808000		//	Olive
	, 0x800080		//	Purple
	, 0x800000		//	Maroon
	, 0x00FFFF		//	Aqua
	, 0x00FF00		//	Lime
	, 0x008080		//	Teal
	, 0x008000		//	Green
	, 0x0000FF		//	Blue
	, 0x000080		//	Navy
	, 0x000000,		//	Black
};


void LCD_Geometry2(int xPosition, int yPosition, int size, long int color ){
	struct LCD_Geometry *pointCurrent= &Current;
	pointCurrent->xPosition = xPosition;
	pointCurrent->yPosition = yPosition;
	pointCurrent->size = size;
	pointCurrent->color = color;
}

void LCD_FillScreen (int color)
{
	struct LCD_Properties *pointProperties = &Properties;
	LCD_Fill_Rectangle(0,0,pointProperties->xResolution,pointProperties->yResolution, color);
}

void LCD_Fill_Rectangle(int x, int y, int width, int height, long int color)
{
		#if SSD1289
		SSD1289_Fill_Rectangle(x,y,width,height,color);
		#endif
		
		#if ST7735
		ST7735_Draw_FastRect(x,y,width,height,color);
		#endif

		#if ST7735R
		ST7735_Draw_FastRect(x,y,width,height,color);
		#endif
		
		#if ILI9481
		ILI9481_Fill_Rectangle(x,y,width,height,color);
		#endif
}

void LCD_SendPixel(int x, int y, long int color)
{
	#if SSD1289
		SSD1289_SendPixel(x,y,color);
	#endif 
	
	#if ILI9481
		ILI9481_SendPixel(x,y,color);
	#endif
	//#else
		//ST7735_DrawPixel(x,y,color);
	//#endif
}

void LCD_Write_Line(char *string, int x, int y, int size, long int color){
		struct LCD_Geometry *pointCurrent= &Current;
		struct LCD_Properties *pointProperties= &Properties;
		int charcount=0, xcolMAX= x, xLineMax;
		int space= 6; 
		char countstring =1;
		
		if (pointProperties->Rotation == 0 || pointProperties->Rotation == 3)	xLineMax = pointProperties->yResolution - (6 * size);
		else xLineMax= pointProperties->xResolution - (6 * size);

	
		while (*string){   // while char pointer != null
			/// allows the text to move to the next line, for both potrait and landscape modes. 
		
			if(xcolMAX >= xLineMax )
			{
				y+=(-10*size); // Add space between the lines
				xcolMAX = 5;
			}
			
			pointCurrent->xPosition = xcolMAX;
			pointCurrent->yPosition = y; //// if the string goes off the screen my code doesn't care doesn't display at all only x is cared about
			pointCurrent->size = size;
			pointCurrent->color= color;
			xcolMAX += (space*size);
			
			LCD_Write_Char(*string);  // writes char to screen
			
				string++; 
		}							
}

/// Store long strings of chars in the program memory, use this function to print it on screen.
void LCD_Write_PGM(const char *string, int x, int y, int size, long int color){
		struct LCD_Geometry *pointCurrent= &Current;
		struct LCD_Properties *pointProperties= &Properties;
		int charcount=0, xcolMAX= x, xLineMax;
		int space= 6;
		char izard;
		char countstring =1;
			
		if (pointProperties->Rotation == 0 || pointProperties->Rotation == 3)	xLineMax = pointProperties->yResolution - (6 * size);
		else xLineMax= pointProperties->xResolution - (6 * size);

			
		while (izard = pgm_read_byte(string)){   // while char pointer != null
			/// allows the text to move to the next line, for both potrait and landscape modes.
				
		if(xcolMAX >= xLineMax )
		{
			y+=(-10*size); // Add space between the lines
			xcolMAX = 5;
		}
				
		pointCurrent->xPosition = xcolMAX;
		pointCurrent->yPosition = y; //// if the string goes off the screen my code doesn't care doesn't display at all only x is cared about
		pointCurrent->size = size;
		pointCurrent->color= color;
		xcolMAX += (space*size);
				
		LCD_Write_Char(izard); // write char to screen//
				
		string++;
	}
}

void LCD_Write_Char(char c){
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	int Xcol =0 , Yrow=0;
	int Byte =0 , count= 0;

	
	for (Xcol= 0; Xcol < 5; Xcol++) {
		Byte = pgm_read_byte(font+((c*5)+Xcol));   // Access the font array (Char input * 5) + Xcol//
		// i chose to store the font in program memory as it is fairly large and will take up a lot of RAM, since values are static nothign is lost.
		for (Yrow = 8; Yrow > 0; Yrow--)
		{	// since byte is a char or 8 bytes we make the loop look through 8 bits
			if (Byte & 0x01)  // draw pixel only when shifted byte and 0x01 are equal //
			LCD_Fill_Rectangle(pointCurrent->xPosition+(Xcol*pointCurrent->size), pointCurrent->yPosition+(Yrow*pointCurrent->size),pointCurrent->size,pointCurrent->size, pointCurrent->color );
			Byte >>=1; // shift byte over 1
		}

	}
	return;
}


void LCD_Draw_Line(int x0, int y0, int x1, int y1, int size, long int color){
	struct LCD_Geometry *pointCurrent = &Current; // point to the
	struct HT1632C_Properties *point_properties = &Current;
	
	int dx = abs(x1-x0), sx = x0 < x1 ? 1: -1;   // give the absolute value of x1 - x0 , if x0 is less than x1 increment otherwise count down ->
	int dy = abs(y1-y0), sy = y0 < y1 ? 1: -1;// same as x just counting with the y corridnates.
	int err =  dx - dy;// ((dx > dy)? dx : -dy)/2, e2;
	int e2=0;
	
	
	
	pointCurrent->color = color;
	
	for (;;){
		
		for (int x=0; x < size; x++)
		{
			 LCD_Fill_Rectangle(x0,y0+x,size,size,color);
		}
		
		if (x0== x1 && y0==y1) break;
		
		e2 = err *2 ;
		
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	
}

void LCD_Draw_FillCircle(int x0, int y0, int radius, int fill, long int color)
{
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	unsigned char Mult_X = 1;
	unsigned char Mult_Y = 1;
		
	int x= radius, y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;
	
		while(x >= y)
		{	
			Mult_X=1;
			Mult_Y=1;
				
			if (fill == 0){
				for ( int quadrents = 1; quadrents <= 4; quadrents++){
					if(quadrents/2 == 1) Mult_X *=-1;
					if(quadrents/3 == 1) Mult_Y *=-1;
					if(quadrents/4 == 1) {Mult_X *=-1; Mult_Y*=-1;}
				
					LCD_Fill_Rectangle((x*Mult_X + x0), (y*Mult_Y + y0), 1,1,color);
					LCD_Fill_Rectangle((y*Mult_X + x0), (x*Mult_Y + y0),1,1,color);
					//LCD_SendPixel((x*Mult_X + x0), (y*Mult_Y + y0), color);   // quadrent 1
					//LCD_SendPixel((y*Mult_X + x0), (x*Mult_Y + y0), color);			
				}
			}			
			
			else{
				LCD_Draw_Line(x0 - x, y0 + y, x0 + x, y0 + y,1 , color);
				LCD_Draw_Line(x0 - x, y0 - y, x0 + x, y0 - y, 1, color);
				LCD_Draw_Line(x0 - y, y0 + x, x0 + y, y0 + x,1 ,color);
				LCD_Draw_Line(x0 - y, y0 - x, x0 + y, y0 - x, 1 ,color);
			}	
						
		y++;
		radiusError += yChange;
		yChange += 2;
	
		if(((radiusError << 1) + xChange) > 0) // when RadiusError * 2 + xChange is greater than 0
		{
			x--;    // lower the default x value which at start is = radius
			radiusError += xChange;   //
			xChange += 2;
		}
	}
}

//// not rotation compatible
void LCD_Write_Style_Line(char *string, int x, int y, int size, long int color){
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	unsigned char offset = 0x21; 
	int column;
	int charcount=0, space=0, xcolMAX= x, xLineMax;;
	int increase =0;
		
	if (pointProperties->Rotation == 0 || pointProperties->Rotation == 3)	xLineMax = pointProperties->yResolution - (6 * size);
	else xLineMax= pointProperties->xResolution - (6 * size);		

	while (*string){   // while char pointer != null
		if (*string == 0x5C) offset = 0x22; 
		if (*string == '~')return;

		column = (arial_8ptDescriptors[(*string)-offset].columns)+2;
		if (*string == 0x20) column = 6*size; 
		
		if(xcolMAX >= xLineMax) {
			 y-=(14*size); xcolMAX=5; 
			}  // if char begins after 123 pixles drop one line and set x and char count to 0
	
		else{
			
			pointCurrent->xPosition = xcolMAX;
			pointCurrent->yPosition = y; //// if the string goes off the screen my code doesn't care doesn't display at all only x is cared about
			pointCurrent->size = size;
			pointCurrent->color= color;
			LCD_Write_Style_Char(*string);  // writes char to screen
			string++; // incriment the string pointer +=1;
			xcolMAX += column*size;
		}
	}							
}


void LCD_Write_Style_Char(unsigned char c){
	struct LCD_Geometry *pointCurrent= &Current;
	unsigned char offset = 0x21; 
	int difference, column, location;
	int drawbyte, charwidth;
	int yPosition=0;
	unsigned char byte;
	unsigned char charC;
	
	
	//(9 pixels wide)
	if (c == 0x5C) offset = 0x22; /// takes care of that '/' problem 
	if (c == '~') return;   
	charC = (c -offset); 
		
	column = arial_8ptDescriptors[charC].columns; // get the number of columns for the character 
	location = arial_8ptDescriptors[charC].location;  // get its initial location 
	difference = (arial_8ptDescriptors[(charC+1)].location - arial_8ptDescriptors[charC].location);// obtain the difference 
	
	for (int Yrow=difference; Yrow >0; Yrow--) 
	{ 
		byte= pgm_read_byte(arial_8ptBitmaps+(location+Yrow));

		for (int Xcol=0; Xcol < column; Xcol++)
		{
			if (Xcol == 8) {  // special chars wide chars + 8 in columns 
				Yrow--;
				byte = pgm_read_byte(arial_8ptBitmaps+(location+Yrow));  		 
			}
				
			if (byte & 0x80) {
				LCD_Fill_Rectangle(pointCurrent->xPosition+(Xcol*pointCurrent->size), pointCurrent->yPosition+(yPosition*pointCurrent->size),pointCurrent->size,pointCurrent->size, pointCurrent->color );
			}
			byte <<= 1;
		}
		yPosition++;
	}	
	return; 	
}

/// not rotation compatible. 
void LCD_plotEllipseRect(int x0, int y0, int x1, int y1, long int color)
{
	struct LCD_Geometry *point = &Current; // point to the
	point->color = color;
	
	int a = abs(x1-x0), b = abs(y1-y0), b1 = b&1; /* values of diameter */
	long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; /* error increment */
	long err = dx+dy+b1*a*a, e2; /* error of 1.step */

	if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
	if (y0 > y1) y0 = y1; /* .. exchange them */
	y0 += (b+1)/2; y1 = y0-b1;   /* starting pixel */
	a *= 8*a; b1 = 8*b*b;

	do {
		LCD_SendPixel(x1, y0, color); /*   I. Quadrant */
		LCD_SendPixel(x0, y0, color); /*  II. Quadrant */
		LCD_SendPixel(x0, y1, color); /* III. Quadrant */
		LCD_SendPixel(x1, y1, color); /*  IV. Quadrant */
		e2 = 2*err;
		if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */
		if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } /* x step */
		
	} while (x0 <= x1);
	
	while (y0-y1 < b) {  /* too early stop of flat ellipses a=1 */
		LCD_SendPixel(x0-1, y0, color); /* -> finish tip of ellipse */
		LCD_SendPixel(x1+1, y0++, color);
		LCD_SendPixel(x0-1, y1, color);
		LCD_SendPixel(x1+1, y1--, color);
	}
}

//not rotation compatible.
void LCD_Draw_FastSprite(int x, int y, int size, const char *sprite_array, const long int *sprite_pallet){
	long int color=0;
	int Xcol= 0;
	unsigned int Byte =0;

	for (int Yrow = 0; Yrow < 17; Yrow++) {		 ///draw columns 2nd
		for (Xcol =0; Xcol < 16; Xcol++){  /// draw rows first
			Byte = pgm_read_byte(sprite_array + ((Yrow*16)+Xcol));// read 8 bit char   from the program read memory
			color =(sprite_pallet[Byte]);  // read 16 bit colors goes # byte into the color array.
			LCD_Fill_Rectangle(x+(Xcol*size), (y+(Yrow*size)),size,size, color); //
		}
		Xcol =0;  // reset x col so we can redraw the column on a differnt row
	}
	return;
}
