/* 
 * File:   ADC.h
 * Author: Camila Lemus
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __LCD_H_
#define	__LCD_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

//******************************************************************************
//This routine initializes PIC’s internal ADC module.
//First parameter: A/D Conversion Clock Select bits
//Second parameter: Channel
//Third parameter: Justistified
//Fourth parameter: Voltage Reference bit
//Look page 104 and 105 for more information.
//Read ADC.c comments
//******************************************************************************

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear();
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init();
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right();
void Lcd_Shift_Left();


#endif	/* __LCD_H_ */


