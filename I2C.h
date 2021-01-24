/*
 * Filename:     I2C.h
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
 * @file    I2C.h
 * @brief   I2C peripheral driver interface.
 * @author  Diaa Eldeen
 * @date    July 6, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup I2C
 * @{
 */


#ifndef I2C_H
#define	I2C_H


#ifdef	__cplusplus     // Provide C++ Compatibility
extern "C" {
#endif

    
//*****************************************************************************
// Includes
//*****************************************************************************
#include "xc.h"
#include "stdint.h"
#include "deviceConfig.h"
    
    
//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void I2C_Initialize(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

