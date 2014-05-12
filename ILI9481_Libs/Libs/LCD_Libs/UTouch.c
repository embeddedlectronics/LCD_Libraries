/*
 * UTouch.c
 *
 * Created: 5/23/2013 1:42:00 PM
 *  Author: mfolz
 */ 

#include "UTouch.h"


//// write x and y coridnates crap
/// 320 = y   240 = x
//// 3900 - 360     3950-200
/// used post Touch Read

#define X_End 3950
#define X_Begin 200
#define Y_End 3900
#define Y_Begin 360

void UTouch_InitTouch(char orientation)
{
	struct XPT2046_Variables	*pointTouch = &Touch;
	pointTouch->orientation			= orientation;
	pointTouch->precision				= 10;

	SPI_EX_Clock_Busy_DDR	|= (1 << SPI_EX_DClock);
	SPI_EX_DOUT_DDR	  		|= (1 << SPI_EX_DOut);
	SPI_EX_DIN_CS_DDR		= (1 << SPI_EX_IRQ) | (1 << SPI_EX_CS);
	SPI_EX_DIN_CS_DDR 		&= ~(1 << SPI_EX_DIN);

	SPI_EX_CS_High();
	SPI_EX_CLK_High();
	SPI_EX_Data_Out_High();
	SPI_EX_CLK_High();
}

void UTouch_WriteData(char data)
{
	char temp;
	char nop;

	temp=data;
	SPI_EX_CLK_Low(); _delay_us(10);

	for(char count=0; count<8; count++)
	{
		if(temp & 0x80)
		SPI_EX_Data_Out_High();
		else
		SPI_EX_Data_Out_Low();
		temp = temp << 1;
		SPI_EX_CLK_Low(); _delay_us(10);
		nop++;
		SPI_EX_CLK_High(); _delay_us(10);
		nop++;
	}
}

int UTouch_ReadData(void)
{
	int data = 0x0000;
	for(int count=0; count < 12; count++){
		
		SPI_EX_CLK_High();
		_delay_us(1);
		SPI_EX_CLK_Low();
		_delay_us(1);
		data <<=1;
		if (SPI_EX_Data_In_Check()){
			data++;
		}
	}
	return data;
}

void UTouch_Temperature_Read(void){
	struct XPT2046_Variables	*pointTouch = &Touch;
	UTouch_WriteData(0x80);        // 001   Xdrivers off y drivers on read x
	SPI_EX_CLK_High(); _delay_us(10);
	SPI_EX_CLK_Low(); _delay_us(10);
	pointTouch->tempterature=UTouch_ReadData();
}

void UTouch_read(void)
{
	struct XPT2046_Variables	*pointTouch = &Touch;
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	int x[10], y[10];
	int TP_X, TP_Y;
	int datacount=0;
	int differencex=0, differencey=0;
	int previousy=0, previousx=0;
	
	SPI_EX_CS_Low();

	for (int i=0; i< pointTouch->precision; i++)
	{
		UTouch_WriteData(0x90);        // 001  x drivers off y drivers on read x
		SPI_EX_CLK_High(); _delay_us(10);
		SPI_EX_CLK_Low(); _delay_us(10);
		temp_x=UTouch_ReadData();

		UTouch_WriteData(0xD0);      //101  x drivers on y drivers off read y
		SPI_EX_CLK_High(); _delay_us(10);
		SPI_EX_CLK_Low(); _delay_us(10);
		temp_y=UTouch_ReadData();

		if(UTouch_dataAvailable()){
			if (!(temp_x>0x3FFF) || (temp_x==0) || (temp_y> 0x3FFF) || (temp_y==0))
			{
				
				if (i > 1 && i < pointTouch->precision){
					differencex = previousx - temp_x;
					differencey = previousy - temp_y;
					if (differencey <= -40  && differencey >=40) temp_y= previousy;
					if (differencex <= -40  && differencex >=40) temp_x= previousx;
				}
				previousx= temp_x;
				previousy= temp_y;
				ty+=temp_x;
				tx+=temp_y;
				datacount++;
			}
		}
		else break;
	}

	SPI_EX_CS_High();
	if (datacount>0)
	{
		TP_X=tx/datacount;
		TP_Y=ty/datacount;
	}
	pointTouch->xValue = (int)TP_X;
	pointTouch->yValue = (int)TP_Y;
}

char UTouch_dataAvailable(void)
{
	char avail;
	SPI_EX_IRQ_DDR_Low();
	_delay_us(1);
	avail = !SPI_EX_IRQ_In_Check();
	SPI_EX_IRQ_DDR_High();
	_delay_us(1);
	return avail;
}



//// write x and y coridnates crap 
/// 320 = y   240 = x 
//// 3900 - 360     3950-200
/// used post Touch Read
void UTouch_XYCoridnates(int xResolution, int yResolution){
	struct XPT2046_Variables	*pointTouch = &Touch;
	
	pointTouch->xPositionRes = (pointTouch->xValue/ ((X_End - X_Begin) /  (xResolution)) ) -15;
	pointTouch->yPositionRes = (pointTouch->yValue/ ((Y_End - Y_Begin) /  (yResolution)) ) -25;
	
}
