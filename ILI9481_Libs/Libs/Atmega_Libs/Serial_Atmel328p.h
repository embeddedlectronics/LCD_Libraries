/*
 * Serial_Atmel328p.h
 *
 * Created: 10/29/2012 10:42:26 AM
 *  Author: mfolz
 */ 


#ifndef SERIAL_ATMEL328P_H_INCLUDED_
#define SERIAL_ATMEL328P_H_INCLUDED_

void USART_putstring(char* StringPtr);
void USART_send( unsigned char data); 
unsigned char USART_receive(void); 
void USART_init(void); 
void USART_Flush(void);
char* COM_String();

struct Serial_Properties{
	char ComReadBuffer[100];  //Try to remove/ eliminate. 
	int buffer_increment;  
	char String_Display_Flag; 
	
}Status;



#endif /* SERIAL_ATMEL328P_H_ */