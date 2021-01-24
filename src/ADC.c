/*
 * Filename:     ADC.c
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
 * @file    ADC.c
 * @brief   ADC peripheral driver implementation
 * @author  Diaa Eldeen
 * @date    July 6, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup ADC
 * @{
 */


//*****************************************************************************
//                                   Includes
//*****************************************************************************
#include "ADC.h"


//*****************************************************************************
// Function Definitions
//*****************************************************************************
/**
 * @brief    Initialize the ADC peripheral.
 *
 * Powers up the ADC module, selects one ADC channel based on the #ADC_CHANNEL
 * constant and selects the appropriate configurations.
 *
 * @param    none.
 * @return   none.
 * @note    #_XTAL_FREQ system clock must be running at 16MHz.
*/
void ADC_Initialize(void) {
   
    ADC_CHANNEL_PIN_TRIS = 1;   // Input
    ADCON0 = 0x81;  // FOSC/32, power up
    ADCON0bits.CHS = ADC_CHANNEL;   // Channel select
    
    // Right justified, FOSC/32, all channels analog mode, VDD reference
    ADCON1 = 0x80; 
    
    // Reset the ADC read registers
    ADRESL = 0x00;
    ADRESH = 0x00;
}

    
/**
 * @brief   Read the #ADC_CHANNEL conversion.
 * 
 * Starts conversion on the specified #ADC_CHANNEL, polls until conversion 
 * is completed and then returns the result. 
 * 
 * @param   none.
 * @return  The ADC conversion value.
 * @note    ADC_Initialize() function should have been called before calling
 *          this function.
 */
uint16_t ADC_ReadChannel(void) {

    // Start the conversion
    ADCON0bits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE);

    // Conversion finished, return the result
    return ((uint16_t)((ADRESH << 8) + ADRESL));
}


/**
 * @brief   Read temperature value of the sensor connected to the #ADC_CHANNEL.
 * 
 * Starts conversion on the specified #ADC_CHANNEL, polls until conversion 
 * is completed and then returns the computed the temperature result. 
 * 
 * @param   none.
 * @return  The temperature value in degree celsius.
 * @note    ADC_Initialize() function should have been called before calling
 *          this function.
 */
uint8_t ADC_ReadTemperature(void) {
    return ADC_ReadChannel() * 0.49;    // linearity constant
}

