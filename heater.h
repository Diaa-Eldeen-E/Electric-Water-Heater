/*
 * Filename:     heater.h
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
 * @file    heater.h
 * @brief   Heater device driver interface
 * @author  Diaa Eldeen
 * @date    July 9, 2020
 ******************************************************************************/
 
/** 
 * @addtogroup Heater
 * @{
 */

 
#ifndef HEATER_H_
#define HEATER_H_


#ifdef __cplusplus	// Provide C++ Compatibility
extern "C"{
#endif

    
//*****************************************************************************
// Includes
//*****************************************************************************
#include <xc.h>	
    
    
//*****************************************************************************
// Preprocessor Constants
//*****************************************************************************
#define HEATING_ELEMENT         PORTCbits.RC5   ///< Heating element port pin
#define HEATING_ELEMENT_TRIS    TRISCbits.TRISC5
    
#define COOLING_ELEMENT         PORTCbits.RC2   ///< Cooling element porst pin
#define COOLING_ELEMENT_TRIS    TRISCbits.TRISC2
  
//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void Heater_Initialize(void);
void Heater_TurnOn(void);
void Heater_TurnOff(void);


#ifdef __cplusplus	// Provide C++ Compatibility
} // extern "C"
#endif

#endif	/* HEATER_H */

