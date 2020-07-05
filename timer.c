/*
 * File:   timer.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 7:26 AM
 */

#include "timer.h"

volatile uint8_t ms = 0; // Count ms
volatile uint8_t ms100 = 0; // Count quarter seconds
uint8_t timer0ReloadVal;


uint8_t flag1Sec;
uint8_t flag5Sec;
uint8_t flag100MS;

extern led_t onLed;


void TMR0_Initialize(void) {
    // Set TMR0 to the options selected in the User Interface

    // PSA assigned; PS 1:8,  mask the nWPUEN and INTEDG bits
    OPTION_REG = (uint8_t) ((OPTION_REG & 0xC0) | (0x02 & 0x3F));

    // Load the TMR value to reload variable
    TMR0 = 0x6;
    timer0ReloadVal = 6;

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enable TMR0 interrupt
    INTCONbits.TMR0IE = 1;
}


// A timer interrupt counts ms
void TMR0_ISR(void) {

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