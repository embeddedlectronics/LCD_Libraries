/*
 * Macros_Functions.c
 *
 * Created: 4/26/2013 3:15:35 PM
 *  Author: mfolz
 */ 
/*
 * Macros_Functions.h
 *
 * Created: 4/8/2013 11:07:38 AM
 *  Author: mfolz
 */ 

#include "Macros_Functions.h"

int max(int a, int b) {
	return (a<b)?b:a;     // or: return comp(a,b)?b:a; for version (2)
}

char* stringcopy(char *string1, char *string2){
	char x=0,zero=0 ;
	for (x=(strlen(string1)); x < (strlen(string1)+ strlen(string2)) ; x++)
	{
		string1[x]= string2[zero++];  // x= end of string 1, add string 2 to the end of string 1.
	}
	return string1;
}


// this code is fucking meh
char* IntToArray(int Int_In, int length)
{
	int x;
	static char buffer[12];
	for (x =0; x <= length; x++)
	{
		buffer[x] =(Int_In/ pow(10,length-x))  +0x30;
		Int_In = Int_In % (int)pow(10,length-x);
	}
	buffer[length+1] ='\0';  // always terminate with a null character to eliminate the possibility of erronous characters.
	return buffer;
}


char* FloatToArray(float analogin, int vref, int analogconversion)
{
	static char buffer[integer+fractional+3];         // creates a static char buffer
	int x, zero= 1, scope =0;
	int y = (integer-1);
	float divisor, voltagecalc;
	scope = pow(2.0, (analogBit));
	/*This section takes care of the type of conversion (int or analog with vref) */
if (analogconversion == 1) { voltagecalc = ((analogin*vref)/ scope); }
		else {( voltagecalc = analogin); vref = 1;}
	
	/*This section is responsible for the conversion of the float into the array of characters */
	for (x = 0; x < (integer+fractional+1) ; x++ ){  
		if (x == integer){buffer[x] = '.'; } // place decimal point when x = the number integers  
		else {
			divisor =  pow(10.0, (y)); // used for the division factor. will decrease. 
			if (((voltagecalc/divisor) <= 1) & (x <= integer) & (zero == 1)) { buffer[x] = 0x20;}  // removes extra zeros on the integer end ie will never display 10 as 0010. 
			else { buffer[x] = (0x30 + (voltagecalc/divisor)); zero = 0; }  // the addition of 0x30 is to convert the number to an assci value 
			voltagecalc = fmod(voltagecalc,divisor);  // keep the remainder, ditch the rest. 
			y--;  
		}
	}
	buffer[integer+fractional+2] ='\0';  // always terminate with a null character to eliminate the possibility of erronous characters. 
	return buffer;
}





unsigned char ASCII_2_HEX(char ASCII){
	if (ASCII >= 0x30 && ASCII <= 0x39){
		HT1632_Write_Char(8, 2, 2,(ASCII));
		return ASCII-0x30;
	}
	
	else if (ASCII >=41 && ASCII <= 0x46){
		HT1632_Write_Char(8, 2, 2,(ASCII-0x37)+0x30);
		return (ASCII-0x37);
	}
	else return 0;
}

int bin_to_bcd(int temp)//decimal to BCD
{
	int a,b,c;
	a=temp;
	b=0;
	if(a>=10)
	{
		while(a>=10)
		{
			a=a-10;
			b=b+16;
			c=a+b;
			temp=c;
		}
	}
	return temp;
}

char bcdtobin(char bcd)
{
	char a;
	a = ((bcd >> 4) * 10)  + (bcd & 0x0F);
	return a;
}


char* DateExtension(char date) /// up to 99
{
	char datexetension[2];
	if (date == 11 || date == 12 || date == 13)
	return "th";
	switch (date % 10)
	{
		case 1: return "st";
		case 2: return "nd";
		case 3: return "rd";
		default: return "th";
	}
	return 0;
}

