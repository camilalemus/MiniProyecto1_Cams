/*
 * File:   mainm.c
 * Author: camil
 *
 * Created on February 23, 2021, 11:23 PM
 */

//******************************************************************************
//                          Configuration Bits
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
//                                  VARIABLES
//******************************************************************************
#define _XTAL_FREQ (8000000)

#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "lcd.h"

uint8_t POT;
uint8_t CONT;
uint8_t THERM;

//Las uso para desplegar el valor del Slave2 en la LCD
uint16_t adc_temp;
uint8_t unitV;
uint8_t decV;
uint8_t hunV;

//Las uso para desplegar el valor del Slave3 en la LCD
uint16_t cont_temp;
uint8_t unitC;
uint8_t decC;
uint8_t hunC;

//******************************************************************************
//                           INSTANCIAR FUNCIONES
//******************************************************************************

void setup(void);
void slave_1(void);
void slave_2(void);
void slave_3(void);
void screen(void);
void ss1_screen(void);
void ss2_screen(void);


//******************************************************************************
//                            CICLO PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init();
    Lcd_Clear();
    while (1){
        screen();
        slave_1();
        slave_2();
        slave_3();
        ss1_screen();
        ss2_screen();
    }
    return;
}

//******************************************************************************
//                               FUNCIONES
//******************************************************************************

void slave_1(void) {
    PORTCbits.RC0 = 0; //Slave Select ADC
    __delay_ms(1);

    spiWrite(1);
    POT = spiRead();

    __delay_ms(1);
    PORTCbits.RC0 = 1; //Slave Deselect 
}

void slave_2(void) {
    PORTCbits.RC1 = 0; //Slave Select Counter
    __delay_ms(1);

    spiWrite(1);
    CONT = spiRead();

    __delay_ms(1);
    PORTCbits.RC1 = 1; //Slave Deselect 

}

void slave_3(void) {
    PORTCbits.RC2 = 0; //Slave Select Thermometer
    __delay_ms(1);

    spiWrite(1);
    PORTB = spiRead();

    __delay_ms(1);
    PORTCbits.RC2 = 1; //Slave Deselect 

}

void screen(void){
        Lcd_Set_Cursor(1,1);        //Set cursor on file 1, position 1
        Lcd_Write_String ("S1: ");  //Write the text
        Lcd_Set_Cursor(1, 7);
        Lcd_Write_String ("S2: ");
        Lcd_Set_Cursor(1, 14);
        Lcd_Write_String ("S3: ");
}

void ss1_screen(){
                                    //   5.0V/255 = 0.02 V --> Bits to volt
    adc_temp = POT * 2;             //Voltage factor without decimals
    unitV = adc_temp/100;           //Unit value trunked
    adc_temp = adc_temp - unitV*100;   //Accumulate the rest numbers
    decV = adc_temp / 10;           //Tenth value
    adc_temp = adc_temp - decV*10;  
    hunV = adc_temp;                //Hundredth value
    
    unitV = unitV + 48;         //Numbers to ASCII
    decV = decV + 48;
    hunV = hunV + 48;
    
    Lcd_Set_Cursor(2,1);        //Set cursor on file 2, position 1
    Lcd_Write_Char (unitV);  //Write the text
    Lcd_Set_Cursor(2,2);        //Set cursor on file 2, position 2
    Lcd_Write_Char (decV);  //Write the text
    Lcd_Set_Cursor(2,3);        //Set cursor on file 2, position 3
    Lcd_Write_Char (hunV);  //Write the text
}

void ss2_screen(){
    cont_temp = CONT;               //Cont value
    unitC = cont_temp/100;           //Unit value trunked
    cont_temp = cont_temp - unitC*100;   //Accumulate the rest numbers
    decC = cont_temp / 10;           //Tenth value
    cont_temp = cont_temp - decC*10;  
    hunC = cont_temp;                //Hundredth value
    
    unitC = unitC + 48;         //Numbers to ASCII
    decC = decC + 48;
    hunC = hunC + 48;
    
    Lcd_Set_Cursor(2,7);        //Set cursor on file 2, position 7
    Lcd_Write_Char (unitC);  //Write the text
    Lcd_Set_Cursor(2,8);        //Set cursor on file 2, position 8
    Lcd_Write_Char (decC);  //Write the text
    Lcd_Set_Cursor(2,9);        //Set cursor on file 2, position 9
    Lcd_Write_Char (hunC);  //Write the text
}

//******************************************************************************
//                                  SETUP
//******************************************************************************

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC = 0;
    TRISCbits.TRISC4 = 1;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}