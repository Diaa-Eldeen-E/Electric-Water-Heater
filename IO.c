/* 
 * File:   IO.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 9:04 AM
 */


#include "IO.h"


void IO_Initialize(void) {
    LED_Initialize();
    Buttons_Initialize();
}


static void LED_Initialize(void) {
    HEATER_LED_TRIS = 0; // output
    HEATER_LED = 0;
}


static void Buttons_Initialize(void) {
    
    OPTION_REGbits.nRBPU = 0;   // pull up enable
    OPTION_REGbits.INTEDG = 1;  // Rising edge
//    TRISB |= 0x7;   // 3 buttons input
//    PORTB = 0x7;   // Pull up
    
    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.INTF = 0;
    
    // Enable RB0 external interrupt
    INTCONbits.INTE = 1;    // External interrupt enable
}

