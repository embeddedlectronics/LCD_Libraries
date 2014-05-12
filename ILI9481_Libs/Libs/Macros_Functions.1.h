/*
 * Macros_Functions.h
 *
 * Created: 4/8/2013 11:07:38 AM
 *  Author: mfolz
 */ 


#ifndef MACROS_FUNCTIONS_H_INCLUDED_
#define MACROS_FUNCTIONS_H_INCLUDED_

#define analogBit 10   // Analog bit conversion amount
#define integer 2   // # of integer parts in the decimal number
#define fractional 1    //  # of fractional parts in the decimal number

int max(int a, int b);
char* stringcopy(char *string1, char *string2);
// this code is fucking meh
char* IntToArray(int Int_In, int length);
char* FloatToArray(float analogin, int vref, int analogconversion);
unsigned char ASCII_2_HEX(char ASCII);
int bin_to_bcd(int temp);
char bcdtobin(char bcd);




#endif /* MACROS_FUNCTIONS_H_ */