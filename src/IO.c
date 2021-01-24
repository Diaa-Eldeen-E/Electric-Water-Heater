/*
 * Filename:     IO.c
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
 * @file    IO.c
 * @brief   LED and buttons device driver implementation.
 * @author  Diaa Eldeen
 * @date    July 5, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup IO
 * @{
 */


//*****************************************************************************
// Includes
//*****************************************************************************
#include "IO.h"


//*****************************************************************************
// Function Definitions
//*****************************************************************************
/**
 * @brief   Initialize IOs (LED and buttons).
 * 
 * Initializes the heater LED, up/down buttons and the on/off button.
 * It Also enables the PORTB pull up and enables interrupt on rising edge for 
 * the on/off button.
 * 
 * @param   none.
 * @return  none.
 * @note    #HEATER_LED_TRIS and #HEATER_LED must be appropriately defined
 *          to the corresponding connected hardware.
 */
void IO_Initialize(void) {
    LED_Initialize();
    Buttons_Initialize();
}


/**
 * @brief   Initialize the heater LED.
 * 
 * Configures the heater LED pin and initializes it to off state.
 * 
 * @param   none.
 * @return  none.
 * @note    #HEATER_LED_TRIS and #HEATER_LED must be appropriately defined
 *          to the corresponding pin connected to the LED.
 */
static void LED_Initialize(void) {
    HEATER_LED_TRIS = 0; // output
    HEATER_LED = 0;
}


/**
 * @brief   Initialize the up/down buttons.
 * 
 * Configures the up/down buttons and the on/off button.
 * It Also enables the PORTB pull up and enables interrupt on rising edge for 
 * the on/off button.
 * 
 * @param   none.
 * @return  none.
 * @note     must be appropriately defined
 *          to the corresponding connected hardware.
 */
static void Buttons_Initialize(void) {
    
    OPTION_REGbits.nRBPU = 0;   // pull up enable
    OPTION_REGbits.INTEDG = 1;  // Rising edge
    
    // 3 buttons input
    ON_BUTTON_TRIS = 1;
    DOWN_BUTTON_TRIS = 1;
    UP_BUTTON_TRIS = 1;
    
    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.INTF = 0;
    
    // Enable RB0 external interrupt
    INTCONbits.INTE = 1;    // External interrupt enable
}

