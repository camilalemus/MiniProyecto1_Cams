#include "lcd.h"

#define _XTAL_FREQ 8000000

#define RS RE0
#define EN RE1

//LCD Functions Developed by electroSome


void Lcd_Port(char a)
{
	PORTD = a;  //Copy char on portD
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp;
	if(a == 1)      //Primera fila
	{
	  temp = 0x80 + b - 1;
		Lcd_Cmd(temp);
	}
	else if(a == 2) //Segunda fila
	{
		temp = 0xC0 + b - 1;
		Lcd_Cmd(temp);
	}
}

void Lcd_Init() {
    __delay_ms(20);
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_ms(11);
    Lcd_Cmd(0x30);

    Lcd_Cmd(0x38);
    Lcd_Cmd(0x08);
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);
    Lcd_Cmd(0x0C);
}

void Lcd_Write_Char(char a)
{
   RS = 1;             // => RS = 1
   Lcd_Port(a); //Data transfer
   EN = 1;      //Enable
   __delay_us(40);  //Delay
   EN = 0;      //Disable
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}


