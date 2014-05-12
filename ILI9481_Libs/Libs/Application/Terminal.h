/*
 * Terminal.h
 *
 * Created: 5/30/2013 4:22:36 PM
 *  Author: mfolz
 */ 


#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

void terminalemulator(void);
void terminalstring(char* string);
void Terminal_Set_Line(char line);
void TerminalChar(char c);
void Terminal_Set_Properties(char line, char size, long int color);


struct Terminal_Program_Values{
	char currentline;
	int previousx;
	int previousy;
	char previousline;
	int xResolution;
	int yResolution;  
}TerminalProperties;
#endif /* TERMINAL_H_ */