/*
 * Filename:     IO.h
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
 * @file    IO.h
 * @brief   LED and buttons device driver interface.
 * @author  Diaa Eldeen
 * @date    July 5, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup IO
 * @{
 */


#ifndef IO_H
#define	IO_H


#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    
//*****************************************************************************
// Includes
//*****************************************************************************
#include "xc.h"
#include "stdint.h"
#include "deviceConfig.h"

 
//*****************************************************************************
// Preprocessor Macros and Constants
//*****************************************************************************
#define HEATER_LED_TRIS TRISB3  ///< TRIS bit for the heater LED pin
#define HEATER_LED      RB3     ///< PORT bit for the heater LED pin

#define DEBOUNCING_DELAY_MS   50    ///< Button debouncing delay time
    
#define ON_BUTTON_TRIS      TRISB0     ///< TRIS bit for the on button pin
#define DOWN_BUTTON_TRIS    TRISB1     ///< TRIS bit for the down button pin
#define UP_BUTTON_TRIS      TRISB2     ///< TRIS bit for the up button pin  
    
#define ON_BUTTON       RB0     ///< PORT bit for the on button pin
#define DOWN_BUTTON     RB1     ///< PORT bit for the down button pin
#define UP_BUTTON       RB2     ///< PORT bit for the up button pin    
    
    
/**
 * Check the down button state.
 * @return  1   If The button is pressed.
 * @return  0   If the button is released.
 */
#define DOWN_BUTTON_IS_PRESSED()  (DOWN_BUTTON == 0)
 
    
/**
 * Check the up button state.
 * @return  1   If The button is pressed.
 * @return  0   If the button is released.
 */   
#define UP_BUTTON_IS_PRESSED()  (UP_BUTTON == 0)

    
/**
 * Check the on button state.
 * @return  1   If The button is pressed.
 * @return  0   If the button is released.
 */   
#define ON_BUTTON_IS_PRESSED()  (ON_BUTTON == 0)
    
    

//*****************************************************************************
// Typedefs
//*****************************************************************************
/**
 * LED state and event container.
 */ 
typedef struct LED_t {
    uint8_t state;
    volatile uint8_t timerFlag; ///< Time out event used for blinking
} LED_t;


/**
 * Button states and event container.
 */ 
typedef struct button_t {
    volatile uint8_t state;
    uint8_t prevState;
    volatile uint8_t timer; ///< Used for debouncing delay
} button_t;

    
//*****************************************************************************
// Function Prototypes
//*****************************************************************************   
void IO_Initialize(void);
static void LED_Initialize(void);
static void Buttons_Initialize(void);
    


#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif	/* IO_H */

