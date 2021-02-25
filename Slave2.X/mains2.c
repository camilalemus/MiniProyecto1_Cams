/*
 * File:   mains1.c
 * Author: camila 
 *
 * Created on February 23, 2021, 12:08 PM
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

#include <xc.h>
#include <stdint.h>
#include "SPI.h"

//******************************************************************************
//                                  VARIABLES
//******************************************************************************

#define _XTAL_FREQ (8000000)

//******************************************************************************
//                           INSTANCIAR FUNCIONES
//******************************************************************************

void setup(void);

//******************************************************************************
//                              CICLO PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1) {
        spiWrite(PORTD); //Port value
    }
}

//******************************************************************************
//                                  SETUP
//******************************************************************************

void setup(){
        
    ANSEL = 0;
    ANSELH = 0;
    //ANSELbits.ANS2 = 1;
    
    INTCONbits.GIE = 1;             //Set Global interrupts enable
    TRISA = 0;
//    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA5 = 1;           //Select bit
    TRISC = 0;                      //Port C and B are outputs
    TRISCbits.TRISC4 = 1;           //Input of slave SDI
    TRISD = 0;
    TRISB = 0;
    PORTC = 0;                      // Turn off display and LEDs
    PORTD = 0;
    TRISBbits.TRISB1 = 1;           //Inputs B0 and B1 because of push buttons
    TRISBbits.TRISB0 = 1;
    INTCONbits.PEIE = 1;            //Periferal Enable
    INTCONbits.RBIE = 1;            //Interrupts on change
    IOCBbits.IOCB0 = 1;             //Pines
    IOCBbits.IOCB1 = 1;
    INTCONbits.RBIF = 0;
    
//    PIR1bits.ADIF = 0; //ADC interrupt flag cleared
//    PIE1bits.ADIE = 1; //ADC interrupt enable ON
//    ADCON0bits.ADON = 1; //ADC Enable bit
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

//******************************************************************************
//                                  FUNCIONES
//******************************************************************************

//******************************************************************************
//                          INTERRUPCIONES
//******************************************************************************

void __interrupt() isr(void) {
    if (INTCONbits.RBIF == 1) {
        if (PORTBbits.RB0 == 1) { //If the push button is pressed down, add to the Debounce Counter
            PORTD--;
        } else if (PORTBbits.RB1 == 1) {
            PORTD++;
        }
        INTCONbits.RBIF = 0; //Turn off the interrupt flag
    }
//    else if (SSPIF == 1) { //If master send something
//        spiRead();
//        spiWrite(PORTD); //Port value
//        SSPIF = 0;
//    }

}
