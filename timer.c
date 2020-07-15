/*
 * File:   timer.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 7:26 AM
 */

#include "timer.h"
#include "IO.h"

volatile uint8_t gTickMS = 0; // Count timer millisecond ticks
volatile uint8_t gTick100MS = 0; // Count quarter seconds
volatile uint8_t gButtonsTimer = 0;  // 100 millisecond counts

volatile uint8_t gFlagSSDBlink;
volatile uint8_t gFlagADCPeriod;

extern button_t upButton;
extern button_t downButton;
extern LED_t heaterLED;

static uint8_t timer0ReloadVal;

void Timer0_Initialize(void) {
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


// A timer interrupt counts gTickMS
void Timer0_ISR(void) {

    // Clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    TMR0 = timer0ReloadVal;

    if(gTickMS == 100) {    // 100 MS
        
        gFlagADCPeriod = 1;
        gTick100MS++;
        gButtonsTimer++;
        gTickMS = 0;        // Wrap around 100 milliseconds
        
        if(gTick100MS == 200)    // Wrap around 20 seconds
            gTick100MS = 0;
        
        if(gTick100MS % 5 == 0) {   // 500 milliseconds flag
            heaterLED.timerFlag = 1;
            gFlagSSDBlink = 1;
        }

    }
    gTickMS++;
    upButton.timer++;
    downButton.timer++;
        
}