/*
 * Terminal.c
 *
 * Created: 5/30/2013 4:22:18 PM
 *  Author: mfolz
 
 hen you change to landscape orientation the corridnates are as follows (top left:  x=320  y=0, bottom right: x=0, y=240)
 when you use the potriat orientation the corridnates are as follows ( top left: x=0 y =0, bottom right: x= 240, y=320)
 This was the easiest and most logical way
 
 
 Version 1.0
 ---------------
	- works 
	
1.2
	- Works better 
 */ 
#define F_CPU 16000000UL  // 16 MHz
#include "Terminal.h"
#include "../LCD_Libs/LCD_Draw.h"
#include <avr/pgmspace.h>
#include <avr/delay.h>
/* 
*/

void terminalemulator(void){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	
	if (pointProperties->Rotation == 1 || pointProperties->Rotation == 2 )
	{
		pointTerminal->xResolution = pointProperties->xResolution;
		pointTerminal->yResolution = pointProperties->yResolution;
	}

	else 
	{
		pointTerminal->xResolution = pointProperties->yResolution;
		pointTerminal->yResolution = pointProperties->xResolution; 
	}
	
	LCD_Fill_Rectangle(0,0,pointTerminal->xResolution,pointTerminal->yResolution, 0x000000);
	LCD_Write_Line("Microcrap Dops[1.2]",10, (pointTerminal->yResolution - 15),1,0xFFFFF);

	pointTerminal->currentline = 1;
	pointTerminal->previousx= pointTerminal ->xResolution;
	pointTerminal->previousy= 0 ;		
	
}

void Terminal_Set_Line(char line){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	pointTerminal->currentline = line+3;	
}

void Terminal_Set_Properties(char line, char size, long int color){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	pointTerminal->currentline = line+1;
	pointCurrent->color = color;
	pointCurrent->size = size; 
}

void TerminalChar(char c){
	struct Terminal_Program_Values	*pointTerminal = &TerminalProperties;
	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	
	
	if (c == 0x0D || c == 0x0A) {
		 pointTerminal->currentline += 1;  
		 pointTerminal->previousx = 5;  
		 c = 0x3E;
	}
	
	else if (pointTerminal->previousx  > (pointTerminal->xResolution -(6*pointCurrent->size)) ){
		pointTerminal->currentline += 1;
		pointTerminal->previousx = 15;
	}
	
	else if( pointTerminal->currentline*(10*pointCurrent->size) > pointTerminal->yResolution){
		_delay_ms(100);
		terminalemulator();
		Terminal_Set_Properties(2, 1, 0xFFFFFF);
	} 
	pointCurrent->xPosition = pointTerminal->previousx;
	pointCurrent->yPosition = pointTerminal->yResolution - pointTerminal->currentline*(10*pointCurrent->size);
	LCD_Write_Char(c);
	pointTerminal->previousx += (6* pointCurrent->size); 	
}

