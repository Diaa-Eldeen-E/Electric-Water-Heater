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

//volatile uint8_t ms = 0; // Count ms
//volatile uint8_t ms100 = 0; // Count quarter seconds
//uint8_t timer0ReloadVal;


#define STATE_INIT  0
#define STATE_WAIT  1
#define STATE_BLINK 2



led_t onLed;



void ledInit() {
    TRISB = 0x00; // all pints output
    PORTB = 0;
    onLed.state = STATE_INIT;
    onLed.timerFlag = 0;
}

void TMR0_ISR();

void main(void) {


    // Make a state machine
    // Use the ticks time to drive timed behavior

    TMR0_Initialize();
    ledInit();
    INTCONbits.GIE = 1;

    while (1) {

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
                PORTBbits.RB1 ^= 1;
                onLed.state = STATE_WAIT;
                break;
                
            default:
                break;
        }
        
        
    }

    return;
}

// Use microChip youtube video as reference for debouncing

// On/Off button interrupt and debouncing

// A timer interrupt counts ms
//Timer0
//Prescaler 1:8; TMR0 Preload = 6; Actual Interrupt Time : 1 ms

void __interrupt() INTERRUPT_InterruptManager(void) {

    if (INTCONbits.TMR0IF) {
        TMR0_ISR();
    }
}

