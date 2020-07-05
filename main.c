/*
 * File:   main.c
 * Author: Diaa Eldeen
 *
 * Created on July 4, 2020, 8:36 PM
 */


// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums inst

#include <xc.h>
#include <stdint.h>

volatile uint8_t ms = 0; // Count ms
volatile uint8_t ms100 = 0; // Count quarter seconds
uint8_t timer0ReloadVal;

void TMR0_Initialize(void) {
    // Set TMR0 to the options selected in the User Interface

    // PSA assigned; PS 1:8; TMRSE Increment_hi_lo; mask the nWPUEN and INTEDG bits
    OPTION_REG = (uint8_t) ((OPTION_REG & 0xC0) | (0x02 & 0x3F));

    // TMR0 131; 
    TMR0 = 0x6;

    // Load the TMR value to reload variable
    timer0ReloadVal = 6;

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enabling TMR0 interrupt
    INTCONbits.TMR0IE = 1;
    //INTCON = 0xA0;
}

#define STATE_INIT  0
#define STATE_WAIT  1
#define STATE_BLINK 2

typedef struct led_t {
    uint8_t state;
    volatile uint8_t timerFlag;
    uint8_t timerCount;
} led_t;

led_t onLed;

uint8_t flag1Sec;
uint8_t flag5Sec;
uint8_t flag100MS;

void ledInit() {
    TRISB = 0x00; // all pints output
    PORTB = 0;
    onLed.state = STATE_INIT;
    onLed.timerCount = 0;
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

void TMR0_ISR() {

    // Clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    TMR0 = timer0ReloadVal;

    if(ms == 100) {
        
        flag100MS = 1;
        ms = 0;
        ms100++;
        
        if(ms100 == 200)    
            ms100 = 0;
        
        if(ms100 % 10 == 0) {
            onLed.timerFlag = 1;
            flag1Sec = 1;
        }
        if (ms100 % 50 == 0) {
            flag5Sec = 1;
        }
    }
    ms++;
        
}