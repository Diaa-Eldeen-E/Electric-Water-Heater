/*
 * File:   main.c
 * Author: Diaa Eldeen
 *
 * Created on July 4, 2020, 8:36 PM
 */


// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#include <xc.h>
#include <stdint.h>

#include "timer.h"
#include "IO.h"
#include "SSD.h"


//volatile uint8_t ms = 0; // Count ms
//volatile uint8_t ms100 = 0; // Count quarter seconds
//uint8_t timer0ReloadVal;

led_t onLed;
button_t onButton;

void TMR0_ISR();

void turnOff(void) {
    
    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;
    
    //stop the fan
    
    SLEEP();
    
}





// ADC read temp
// temp adjustment buttons
// heating element
// cooling element

void main(void) {


    // Make a state machine
    // Use the ticks time to drive timed behavior

    TMR0_Initialize();
    ledInit();
    buttonsInit();
    SSD_init();
    INTCONbits.GIE = 1;

    
    
    uint8_t i=0;
    while (1) {

        if (onButton.state) {
            
            switch (onLed.state) {

                case STATE_INIT:
                    onLed.state = STATE_WAIT;
                    break;

                case STATE_WAIT:
                    if (onLed.timerFlag) {
                        onLed.state = STATE_BLINK;
                        onLed.timerFlag = 0;
                    }
                    break;

                case STATE_BLINK:
                    STATUS_LED ^= 1;
//                    SSD1 = 1;
//                    if(i == 10) i = 0;
                    i++;
//                    SSD1 ^= 1;
//                    SSD2 ^= 1;
                    onLed.state = STATE_WAIT;
                    break;

                default:
                    break;
                    
            }
            SSD_multiplex(i);
        }
        // The button is off, go to sleep
        else {
            turnOff();
        }
        
    }

    return;
}

// Use microChip youtube video as reference for debouncing

// On/Off button interrupt and debouncing



void __interrupt() INTERRUPT_InterruptManager(void) {

    if (INTCONbits.TMR0IF) {
        TMR0_ISR();
    }
    if (INTCONbits.INTF) {
        INTB0_ISR();
    }
}

