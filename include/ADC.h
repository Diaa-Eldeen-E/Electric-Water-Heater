/*
 * Filename:     ADC.h
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
 * @file    ADC.h
 * @brief   ADC peripheral driver interface
 * @author  Diaa Eldeen
 * @date    July 6, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup ADC
 * @{
 */



#ifndef ADC_H
#define ADC_H


#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    
//*****************************************************************************
// Includes
//*****************************************************************************
#include <xc.h>
#include <stdint.h>    
    
    
//*****************************************************************************
// Preprocessor Macros and Constants
//*****************************************************************************
/**
 * The MCU ADC channel number to be initialized and read using this API.
 * This is the number of the ADC channel that is connected to the temperature 
 * sensor.
 */    
#define ADC_CHANNEL 2   
        
#define ADC_CHANNEL_PIN_TRIS TRISA2 ///< TRIS bit for that channel pin
    
//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void ADC_Initialize(void);
uint16_t ADC_ReadChannel(void);
uint8_t ADC_ReadTemperature(void);


#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif	//ADC_H
/**
 End of File
*/

