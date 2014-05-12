/*
 * ILI9481_Libs.c
 *
 * Created: 25/11/2013 8:36:27 AM
 *  Author: Michael
 */ 

#define F_CPU 1600000UL  //16Mhz clock

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h> // provides delays
#include <avr/power.h> // used for setting clock prescaler
#include <stdbool.h>
#include <avr/interrupt.h>
#include <string.h>

#include "Libs/LCD_Libs/ILI9481.h"
#include "Libs/Application/Demo_App.h"
#include "Libs/Application/Terminal.h"

/// the library you have is shit. 
/// not sure what isnt working at this point i think i may have to 
/// humm
int main(void)
{
	FILE lcd_str = FDEV_SETUP_STREAM(TerminalChar, NULL, _FDEV_SETUP_RW);
	stdout = &lcd_str;

	struct LCD_Geometry *pointCurrent= &Current;
	struct LCD_Properties *pointProperties= &Properties;
	pointProperties->Rotation= 0;
	//ILI9481_Initalize();
SSD1289_Initalize();
	testLibrary(); 


}