/*
 * Filename:     SSD.c
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
 * @file    SSD.c
 * @brief   7-Segment display device driver implementation
 * @author  Diaa Eldeen
 * @date    July 5, 2020
 ******************************************************************************/

/** 
 * @addtogroup SSD Seven Segment Display
 * @{
 */


//*****************************************************************************
//                                   Includes
//*****************************************************************************
#include "SSD.h"


//*****************************************************************************
// Variables
//*****************************************************************************
/**
 * Maps the decimal digit to the SSD appropriate data value.
 */
const unsigned char SEGMENT_MAP[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F};



//*****************************************************************************
// Function Definitions
//*****************************************************************************

/**
 * @brief   Print a digit number on a 7-segment display.
 * 
 * Prints a decimal digit (0 to 9) on either the ones 7-segment display or the
 * tens 7-segment display.
 * 
 * @param   unit  7-segment display selection (#ONES or #TENS).
 * @param   digit The decimal digit to be printed on the 7-segment display.
 * @return  none.
 * @note    #SSD_PORT, #SSD_ONES and #SSD_TENS must be previously defined with
 *          the appropriate settings corresponding to the connected hardware.
 */
void SSD_Print(unit_t unit, uint8_t digit) {
    
    ASSERT(digit <= 9);
    
    SSD_PORT = SEGMENT_MAP[digit];
    
    switch(unit) {
        case ONES:
            SSD_ONES = 1;
            SSD_TENS = 0;
            break;
            
        case TENS:
            SSD_TENS = 1;
            SSD_ONES = 0;
            break;
            
        default:
            ASSERT(0);
            break;
    }

}

/**
 * @brief   Initialize the 7-segment display.
 * 
 * Configures the 7-segment displays data and control ports and initializes 
 * them to the Off state.
 * 
 * @param   none.
 * @return  none.
 * @note    #SSD_PORT, #SSD_TRIS, #SSD_ONES_TRIS, #SSD_TENS_TRIS, #SSD_ONES
 *          and #SSD_TENS must be previously defined with the appropriate
 *          settings corresponding to the connected hardware.
 */
void SSD_Initialize(void) {
    SSD_TRIS = 0x00; // Output
    SSD_PORT = 0x00; // Off

    SSD_ONES_TRIS = 0; // Output
    SSD_TENS_TRIS = 0; // Output

    SSD_ONES = 0;
    SSD_TENS = 0;
}

