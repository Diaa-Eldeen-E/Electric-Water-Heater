/*
 * Filename:     stateMachine.h
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
 * @file    stateMachine.h
 * @brief   Application state machine design.
 * @author  Diaa Eldeen
 * @date    July 6, 2020
 ******************************************************************************/
 
 
/** 
 * @addtogroup SM State Machine
 * @{
 */


#ifndef STATEMACHINES_H
#define	STATEMACHINES_H


#ifdef	__cplusplus     // Provide C++ Compatibility
extern "C" {
#endif

    
//*****************************************************************************
// Includes
//*****************************************************************************
#include "IO.h"
#include "SSD.h"
#include "EEPROM.h"
#include "heater.h"
#include "ADC.h"
#include "timer.h"
    
   
//*****************************************************************************
// Preprocessor Constants
//*****************************************************************************
#define MULTIPLEX_TIME_PERIOD   10  ///< Multiplexing SSDs time period in MS
    
    
//*****************************************************************************
// Typedefs
//*****************************************************************************
/**
 * @brief   7-segment displays multiplexing states.
 * A single 7-segment display can be operating at any given time, thus each
 * operating 7-segment display corresponds to a state.
 */
typedef enum {
    STATE_ONES_DIGIT,
    STATE_TENS_DIGIT        
}SSDMultiplexState_t;    


typedef enum {
    STATE_INIT,
    STATE_HEATING,
    STATE_COOLING
        
}HeaterState_t;


typedef enum {
    STATE_TEMP_SET,
    STATE_TEMP_DISPLAY
}SSDState_t;   

typedef enum {
    STATE_SHOW,
    STATE_HIDE
}SSDBlinkState_t;  


typedef enum {
    STATE_OFF = 0,
    STATE_ON = 1,
    STATE_BLINK        
}LEDState_t;  


typedef enum {
    STATE_PRESSED = 0,
    STATE_RELEASED = 1,
    STATE_PRE_PRESSED,
    STATE_PRE_RELEASED
}ButtonState_t;  


//*****************************************************************************
// Function Prototypes
//*****************************************************************************
void StateMachine_Initialize(void);
void StateMachine_TurnOff(void);
void StateMachine_Run(void);
static void StateMachine_OnWakeUp(void);

static void Heater_TransitToHeating(void);
static void Heater_TransitToCooling(void);
static void StateMachine_Heater(uint8_t tempAvg);

static void SSD_Multiplex(uint8_t num);
static void SSD_Blink(uint8_t num);
static void StateMachine_SSD(uint8_t tempCur);

static void StateMachine_LED(void);

static void UpButton_TransitToPressed(void);
static void DownButton_TransitToPressed(void);
static void StateMachine_UpButton(void);
static void StateMachine_DownButton(void);
static void StateMachine_Buttons(void);

void INTB0_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATEMACHINES_H */

