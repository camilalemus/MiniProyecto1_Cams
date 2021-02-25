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

uint8_t POT;
uint8_t CONT;
uint8_t THERM;

//******************************************************************************
//                           INSTANCIAR FUNCIONES
//******************************************************************************

void setup(void);
void slave_1(void);
void slave_2(void);
void slave_3(void);


//******************************************************************************
//                            CICLO PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1){
        slave_1();
        slave_2();
        slave_3();
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
    PORTB = spiRead();

    __delay_ms(1);
    PORTCbits.RC1 = 1; //Slave Deselect 

}

void slave_3(void) {
    PORTCbits.RC2 = 0; //Slave Select Thermometer
    __delay_ms(1);

    spiWrite(1);
    THERM = spiRead();

    __delay_ms(1);
    PORTCbits.RC2 = 1; //Slave Deselect 

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
    PORTB = 0;
    PORTD = 0;
    PORTCbits.RC0 = 1;
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}