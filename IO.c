/* 
 * File:   IO.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 9:04 AM
 */


#include "IO.h"

extern uint8_t ledState;
extern button_t onButton;
extern button_t upButton;
extern button_t downButton;
extern volatile uint8_t flag500MS1;


void ledInit(void) {
    STATUS_LED_TRIS = 0; // output
    STATUS_LED = 0;
    ledState = 0;   // STATE_INIT
    flag500MS1 = 0;
}

void buttonsInit(void) {
    
    OPTION_REGbits.nRBPU = 0;   // pull up enable
    OPTION_REGbits.INTEDG = 1;  // Rising edge
//    TRISB |= 0x7;   // 3 buttons input
//    PORTB = 0x7;   // Pull up
    
    onButton.state = 0; // Start with off state
    upButton.state = STATE_RELEASED;
    downButton.state = STATE_RELEASED;
    
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