/*
 * Filename:     main.c
 * Compiler:     XC8
 * Target:       PIC16F877A
 * Date:         July 4, 2020
 *
 * MIT License 
 *
 * Copyright (c) 2020 - Diaa Eldeen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */


#include "deviceConfig.h"
#include <xc.h>
#include <stdint.h>

#include "stateMachine.h"


void main(void) {

    // System initialization
    Timer0_Initialize();
    ADC_Initialize();
    IO_Initialize();
    SSD_Initialize();
    I2C_Initialize();
    Heater_Initialize();
    StateMachine_Initialize();
    
    // Enable global interrupts
    INTCONbits.GIE = 1; 
    

    StateMachine_Run();

    return;
}


void __interrupt() INTERRUPT_InterruptManager(void) {

    if (INTCONbits.TMR0IF) {
        Timer0_ISR();
    }
    if (INTCONbits.INTF) {
        INTB0_ISR();
    }
}
