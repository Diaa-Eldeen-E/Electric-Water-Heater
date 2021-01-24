/*
 * Filename:     heater.c
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
 * @file    heater.c
 * @brief   Heater device driver implementation
 * @author  Diaa Eldeen
 * @date    July 9, 2020
 ******************************************************************************/

/** 
 * @addtogroup Heater
 * @{
 */


//*****************************************************************************
// Includes
//*****************************************************************************
#include "heater.h"


//*****************************************************************************
// Function Definitions
//*****************************************************************************
/**
 * @brief   Initialize the heater.
 * 
 * Configures the heating and cooling elements IOs and initializes them to
 * the off state.
 * 
 * @param   none.
 * @return  none.
 */
void Heater_Initialize(void) {
    COOLING_ELEMENT_TRIS = 0; // Cooling element pin output
    HEATING_ELEMENT_TRIS = 0; // Heating element pin output
    HEATING_ELEMENT = 0;    // Turn off
    COOLING_ELEMENT = 0;    // Turn off
}

/**
 * @brief   Turn on the heater.
 * 
 * Turns on the heating element and turns off the cooling element.
 * 
 * @param   none.
 * @return  none.
 */
void Heater_TurnOn(void) {
    HEATING_ELEMENT = 1;
    COOLING_ELEMENT = 0;
}


/**
 * @brief   Turn off the heater.
 * 
 * Turns off the heating element and turns on the cooling element.
 * 
 * @param   none.
 * @return  none.
 */
void Heater_TurnOff(void) {
    HEATING_ELEMENT = 0;
    COOLING_ELEMENT = 1;
}