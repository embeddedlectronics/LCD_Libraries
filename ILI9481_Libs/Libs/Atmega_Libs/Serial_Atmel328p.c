/*
 * Serial_Atmel328p.c
 *
 * Created: 10/29/2012 10:50:04 AM
 *  Author: mfolz
 */ 
#include "Serial_Atmel328p.h"

#define  F_CPU    16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){
	while(!(UCSR0A & (1<<RXC0)));
	return ((UCSR0B << 8) | UDR0);
}

void USART_Flush(void){
	unsigned char dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
	
}

void USART_send( unsigned char data){

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}

}




//// This reads upto 1000 chars from the serial port and stores it into a char array.
char* COM_String()
{
	static char ComReadBuffer[100];         // creates a static char buffer 100 wish i could make it dyanmic
	static char terminator[2] = {0x5C,0x30}; // ill make this customizable, i need a termination char ~ is the least used ASCII char
	for (int clear=0; clear < 100; clear++){ ComReadBuffer[clear]= 0x00; }
	int x = 0, y=0;
	do 
		{ 
			ComReadBuffer[x] = USART_receive();
			x++;
		} while (ComReadBuffer[x-2] != terminator[0] && ComReadBuffer[x-1] != terminator[1]);  /// this isnt the best approach read incoming com string untill it reads a null char

	ComReadBuffer[x-2] = '\0';  // always terminate with a null character to eliminate the possibility of erroneous characters. also this ends the string 
	
	USART_putstring(ComReadBuffer);
	return ComReadBuffer; 
}





