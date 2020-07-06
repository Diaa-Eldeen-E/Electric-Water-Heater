/*
 * File:   timer.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 7:26 AM
 */

#include "timer.h"
#include "IO.h"

volatile uint8_t ms = 0; // Count ms
volatile uint8_t ms100 = 0; // Count quarter seconds
volatile uint8_t buttonsTimer = 0;
uint8_t timer0ReloadVal;


volatile uint8_t flag500MS1;
volatile uint8_t flag500MS2;
volatile uint8_t flag100MS;

extern button_t upButton;
extern button_t downButton;

void TMR0_Initialize(void) {
    // Set TMR0 to the options selected in the User Interface

    // PSA assigned; PS 1:16,  mask the nWPUEN and INTEDG bits
    OPTION_REG = (uint8_t) ((OPTION_REG & 0xC0) | (0x03 & 0x3F));

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
        buttonsTimer++;
        
        if(ms100 == 200)    
            ms100 = 0;
        
        if(ms100 % 5 == 0) {
            flag500MS1 = 1;
            flag500MS2 = 1;
        }

    }
    ms++;
    upButton.timer++;
    downButton.timer++;
        
}