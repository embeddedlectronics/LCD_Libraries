/*
 * Demo_App.c
 *
 * Created: 4/18/2013 4:19:20 PM
 *  Author: mfolz
 */ 
#include "Demo_App.h"


// CRAPPY TEST ROUTINE !!! YAY IT SUCKS
void testLibrary(void){
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;

	char *helloworld = "hello world"; 
	char *FontDemo = "!#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; 

	//ReadBMP_ARRAY(40,40,BMP_IMAGE2);
	for (int y= 3; y < 4; y++){
		pointProperties->Rotation= y; 
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		
		LCD_Write_Line("Demo for Rotation: ",50,180,2,0xFFFFFF);
		pointCurrent->xPosition = 130;
		pointCurrent->yPosition = 40; //// if the string goes off the screen my code doesn't care doesn't display at all only x is cared about
		pointCurrent->size = 9;
		LCD_Write_Char(y+0x30);
		_delay_ms(6000);
			
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		for (int colorFill=0; colorFill <3; colorFill++){
			LCD_Draw_FillCircle(colorFill*20, 10*colorFill*2, 20+(colorFill*10), 1, colorArray[colorFill]);
		}
		
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);
		
		for (int colorFill=0; colorFill <16; colorFill++){
			LCD_Fill_Rectangle(0,0,500,500, colorArray[colorFill]);
		}
		LCD_Fill_Rectangle(0,0,900,900, 0x000000);

		for (int x=0; x < 2; x++)
		{
			for (int y=0; y < 16; y++ )
			{
				LCD_Draw_Line(0,0,x*20,y*30,2,colorArray[x]);
			}
		}
	
		LCD_Fill_Rectangle(0,0,900,900, 0x000000); 
		_delay_ms(6000);
		
		
		LCD_Write_Line("Standard Terminal Font:",10, 220,2,0xFFFFFF);  
		LCD_Write_Line(FontDemo, 20,180,1, 0xFFFFFF);
		LCD_Write_Line("Arial Font:",10, 120,2,0xFFFFFF);  
		LCD_Write_Style_Line(FontDemo,20,80,1, 0xFFFFFF);	
		_delay_ms(6000);
	
		
		LCD_Fill_Rectangle(0,0,900,900, 0x000000); 
		ReadBMP_ARRAY(0,150, 2,BMP_IMAGE);
		ReadBMP_ARRAY(10,0, 1,BMP_IMAGE);
		_delay_ms(6000);
		

		terminalemulator();
		Terminal_Set_Properties(2, 1, 0xFFFFFF);
		printf("\n Lets display some variables!  \n Hex: 0x%x  0x%x  0x%x  0x%x   \n Decimal: %d  \n String: %s",
		 1,2,3,4 , 42, helloworld);
		 printf(" You are currently viewing the display in rotation: %d ", y);
		printf("\n Thanks for choosing my software be sure to check out my other libraries and demos on www.electricsheep.info");
		_delay_ms(6000);
		
	}
}