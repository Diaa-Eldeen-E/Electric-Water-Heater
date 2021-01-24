/*
 * Filename:     deviceConfig.h
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
 * @file    deviceConfig.h
 * @brief   Device and application specific Configuration.
 * @author  Diaa Eldeen
 * @date    July 5, 2020
 ******************************************************************************/
 

 /** 
 * @addtogroup CONF Device Configuration
 * @{
 */


#ifndef DEVICE_CONFIG_H
#define	DEVICE_CONFIG_H


//*****************************************************************************
// Preprocessor Macros and Constants
//*****************************************************************************
/**
 * The MCU external crystal oscillator frequency and system clock.
 */
#define _XTAL_FREQ 16000000


/**
 * Assert macro for arguments and states checking.
 * A simple software breakpoint is used here but it could be replaced with
 * any desired behavior on such failures.
 */
#define ASSERT(expr)    if(!(expr)) while(1)


#endif	/* DEVICE_CONFIG_H */

