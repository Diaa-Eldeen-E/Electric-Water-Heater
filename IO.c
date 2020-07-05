/* 
 * File:   IO.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 9:04 AM
 */

#include <pic16f877a.h>

#include "IO.h"

extern led_t onLed;
extern button_t onButton;

void ledInit(void) {
    TRISBbits.TRISB1 = 0; // output
    TRISBbits.TRISB2 = 0; // output
    PORTBbits.RB1 = 0;
    onLed.state = STATE_INIT;
    onLed.timerFlag = 0;
}

void buttonsInit(void) {
    
    OPTION_REGbits.nRBPU = 0;   // pull up enable
    OPTION_REGbits.INTEDG = 1;  // Rising edge
    TRISBbits.TRISB0 = 1;   // input
    
    onButton.state = 0; // Start with off state
    
    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.INTF = 0;
    
    // Enable RB0 external interrupt
    INTCONbits.INTE = 1;    // External interrupt enable
    
}


void INTB0_ISR(void) {
    
    if(onButton.state) {
        onButton.state = 0;
    }
    else {
        onButton.state = 1;
    }
    
    // Clear the interrupt flag
    INTCONbits.INTF = 0;
}