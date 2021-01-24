/*
 * Filename:     EEPROM.h
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

/**
 * @file    EEPROM.h
 * @brief   EEPROM device driver interface
 * @author  Diaa Eldeen
 * @date    July 7, 2020
 */
 
/** 
 * @addtogroup EEPROM
 * @{
 */
 
#ifndef EEPROM_H_
#define EEPROM_H_


#ifdef __cplusplus	// Provide C++ Compatibility
extern "C"{
#endif


//*****************************************************************************
// Includes
//*****************************************************************************
#include "I2C.h"


//*****************************************************************************
// Preprocessor Constants
//*****************************************************************************
    
/**
 * @brief The external EEPROM device address on the I2C bus.
 * @note This must be defined to a valid address before reading or writing data.
 */
#define EEPROM_DEVICE_ADDR  0xa0    //10100000

/**
 * @brief The address in the EEPROM at which the temperature set value is stored
 * 
 * An optional value of an address in the EEPROM used in the application code
 * to store the temperature set value.
 */
#define EEPROM_SET_TEMP_ADDR    1


//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void EEPROM_WriteByte(uint8_t address, uint8_t data);
uint8_t EEPROM_ReadByte(uint8_t address);




#ifdef __cplusplus	// Provide C++ Compatibility
} // extern "C"
#endif

#endif /*EEPROM_H_*/

