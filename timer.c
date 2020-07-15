/*
 * Filename:     timer.c
 * Compiler:     XC8
 * Target:       PIC16F877A
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

/******************************************************************************
 * @file    timer.c
 * @brief   Timer peripheral driver implementation
 * @author  Diaa Eldeen
 * @date    July 5, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup Timer
 * @{
 */


//*****************************************************************************
//                                   Includes
//*****************************************************************************
#include "timer.h"
#include "IO.h"


//*****************************************************************************
// Variables
//*****************************************************************************
volatile uint8_t g_msTicks = 0; ///< The timer's millisecond ticks
volatile uint8_t g_100msTicks = 0; ///< The timer's hundred millisecond ticks
volatile uint8_t g_ButtonsTimer = 0; ///< 100 milliseconds counter used by buttons

volatile uint8_t g_FlagSSDBlink; ///< 500MS time event used for blinking the SSD
volatile uint8_t g_FlagADCPeriod;    ///< 100MS time event for ADC read routine

extern button_t g_UpButton;
extern button_t g_DownButton;
extern LED_t g_HeaterLED;

static uint8_t g_Timer0ReloadVal; ///< Reload value for 1 millisecond inetrrupt


//*****************************************************************************
// Function Definitions
//*****************************************************************************
/**
 * @brief   Initialize timer0 peripheral.
 * Initializes the 8-bit timer (timer0), sets it's period to 1 millisecond
 * and enables the overflow interrupt.
 * 
 * @param   none.
 * @return  none.
 * @note    #_XTAL_FREQ must be running at 16MHz clock.
 */
void Timer0_Initialize(void) {

    // Mask the nWPUEN and INTEDG bits and set Prescaler to 1:16,  
    OPTION_REG = (uint8_t) ((OPTION_REG & 0xC0) | (0x03 & 0x3F));

    // Load the TMR with the reload variable for an interrupt every 1 ms
    g_Timer0ReloadVal = 6;
    TMR0 = g_Timer0ReloadVal;
    
    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enable TMR0 interrupt
    INTCONbits.TMR0IE = 1;
}

/**
 * @brief   Timer0 ISR used for timed events.
 * Counts the time in #g_msTicks, #g_100msTicks, #g_ButtonsTimer, #g_UpButton and
 * #g_DownButton timers.
 * Also triggers time events by setting some flags #g_FlagADCPeriod
 * #g_FlagSSDBlink and #g_HeaterLED timer flag.
 * 
 * @param   none.
 * @return  none.
 */
void Timer0_ISR(void) {

    // Clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    TMR0 = g_Timer0ReloadVal;

    if(g_msTicks == 100) {    // 100 MS
        
        g_FlagADCPeriod = 1;
        g_100msTicks++;
        g_ButtonsTimer++;
        g_msTicks = 0;        // Wrap around 100 milliseconds
        
        if(g_100msTicks == 200)    // Wrap around 20 seconds
            g_100msTicks = 0;
        
        if(g_100msTicks % 5 == 0) {   // 500 milliseconds flag
            g_HeaterLED.timerFlag = 1;
            g_FlagSSDBlink = 1;
        }

    }
    g_msTicks++;
    g_UpButton.timer++;
    g_DownButton.timer++;
        
}