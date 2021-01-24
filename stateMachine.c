/*
 * Filename:     stateMachine.c
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
 * @file    stateMachine.c
 * @brief   Electric water heater state machine implementation
 * @author  Diaa Eldeen
 * @date    July 6, 2020
 ******************************************************************************/

/** 
 * @addtogroup SM State Machine
 * @{
 */


#include "stateMachine.h"

static uint8_t g_TempSet;    ///< The desired temperature value
static uint8_t g_TempSetNew;    ///< The choosen temperature value by buttons

extern volatile uint8_t g_msTicks;
extern volatile uint8_t g_FlagADCPeriod; ///< ADC reading routine time event
extern volatile uint8_t g_FlagSSDBlink;  ///< SSD blinking time event
extern volatile uint8_t g_ButtonsTimer;  ///< 100 milliseconds ticks counter

LED_t g_HeaterLED;  ///< Heater LED state and event container
button_t g_OnButton;    ///< On button states and event container
button_t g_UpButton;    ///< Up button states and event container
button_t g_DownButton;  ///< Down button states and event container

static uint8_t g_HeaterState;
static uint8_t g_SSDState;


/**
 * @brief   Puts the state machine in it's initial state.
 */
void StateMachine_Initialize(void) {

    g_HeaterLED.state = STATE_OFF;
    g_HeaterState = STATE_INIT;
    g_SSDState = STATE_TEMP_DISPLAY;

    g_OnButton.state = STATE_OFF; // Start the heater device with off state
    g_UpButton.state = STATE_RELEASED;
    g_DownButton.state = STATE_RELEASED;
}

/**
 * @brief   The turning off action when the off button is pressed.
 */
void StateMachine_TurnOff(void) {
    
    g_OnButton.prevState = STATE_OFF;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    SLEEP();    // Sleep until the on button interrupt
}

/**
 * @brief   The waking up action after turning on the device.
 */
void StateMachine_OnWakeUp(void) {
    
    // Read the g_TempSet from the EEPROM
    g_TempSet = EEPROM_ReadByte(EEPROM_SET_TEMP_ADDR);

    // Initialize the g_TempSet value if not initialized with a valid value
    if (g_TempSet < 35 || g_TempSet > 75) {
        
        g_TempSet = 60;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, g_TempSet);
        
    }
    
    // Reset the states
    g_TempSetNew = g_TempSet;
    
    g_HeaterState = STATE_INIT;
    g_SSDState = STATE_TEMP_DISPLAY;
    g_UpButton.state = STATE_RELEASED;
    g_DownButton.state = STATE_RELEASED;
    g_OnButton.prevState = STATE_ON;
}

/**
 * @brief   The main state machine.
 */
void StateMachine_Run(void) {
    
    uint8_t tempVals[10] = {0}; // Holds the last 10 temperature readings
    uint16_t tempAccum = 0; // The summation of the last 10 readings
    uint8_t idx = 0; // tempVals array index
    uint8_t tempAvg = 0; // Average of the last 10 readings
    uint8_t tempCur = 0; // Current temperature reading
    
    while (1) {

        if (g_OnButton.state == STATE_ON) {

            if (g_OnButton.prevState == STATE_OFF) {
                StateMachine_OnWakeUp();
            }

            // ADC read temperature periodic routine
            if (g_FlagADCPeriod) {

                // Subtract the tenth read before the current one
                tempAccum -= tempVals[idx];

                tempCur = ADC_ReadTemperature();

                // Add the current read to the previous 9 reads
                tempAccum += tempCur;

                tempAvg = tempAccum / 10; // Average of 10 reads

                // Save the current temperature read in the array
                tempVals[idx] = tempCur;

                if (++idx > 9) // Increment array index
                    idx = 0; // Wrap the array index value on 10

                g_FlagADCPeriod = 0;
            }

            StateMachine_Buttons();
            StateMachine_Heater(tempAvg);
            StateMachine_LED();
            StateMachine_SSD(tempCur);
        }
        // The on button is off, turn off the device
        else {
            StateMachine_TurnOff();
        }
    }
    
    ASSERT(0);  // This should never be reached
}

/**
 * @brief   The heater action applied when the water is hot.
 */
static void Heater_TransitToHeating(void) {
    
    g_HeaterState = STATE_HEATING;
    Heater_TurnOn();
    g_HeaterLED.state = STATE_BLINK;
}

/**
 * @brief   The heater action applied when the water is cold.
 */
static void Heater_TransitToCooling(void) {
    
    g_HeaterState = STATE_COOLING;
    Heater_TurnOff();
    g_HeaterLED.state = STATE_ON;
}

/**
 * @brief   Heater state machine.
 * 
 * Controls the heater on/off action.
 */
static void StateMachine_Heater(uint8_t tempAvg) {

    switch (g_HeaterState) {
        
        case STATE_INIT:
            if(tempAvg >= g_TempSet + 5) {
                Heater_TransitToCooling();
            }
            else {
                Heater_TransitToHeating();
            }
            
            break;
            
        case STATE_HEATING:
            if (tempAvg >= g_TempSet + 5) {
                Heater_TransitToCooling();
            }
   
            break;

        case STATE_COOLING:
            if (tempAvg <= g_TempSet - 5) {
                Heater_TransitToHeating();
            }
 
            break;

        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   Display 2-digit number on the 7-segment displays.
 * 
 * Multiplexes two 7-segment displays for displaying a 2-digit number.
 * 
 * @param   num The 2-digit number to be printed on the 7-segment displays.
 * @return  none.
 */
static void SSD_Multiplex(uint8_t num) {

    ASSERT(num <= 99);

    static int8_t prevTime = -MULTIPLEX_TIME_PERIOD; // Last switching time
    static uint8_t state = STATE_ONES_DIGIT; // Used to indicate which SSD is on

    // Check the MULTIPLEX_TIME_PERIOD event (10 ms)
    if ((int8_t) g_msTicks - prevTime >= MULTIPLEX_TIME_PERIOD) {
        
        // Switch 7-segment displays
        switch(state) {
            
            case STATE_ONES_DIGIT:
                state = STATE_TENS_DIGIT;   // Transit to tens digit state
                SSD_Print(TENS, num / 10); // Extract tens digit from the number
                break;
                
            case STATE_TENS_DIGIT:
                state = STATE_ONES_DIGIT;   // Transit to ones digit state
                SSD_Print(ONES, num % 10); // Extract ones digit from the number
                break;
                
            default:
                ASSERT(0);
                break;
        }

        prevTime = g_msTicks;   // Start counting for a new 10 ms event
        
        // The g_msTicks counter variable wraps around 100
        if (prevTime >= (100 - MULTIPLEX_TIME_PERIOD))
            prevTime = -MULTIPLEX_TIME_PERIOD;
    }

}

/**
 * @brief   Displays and blinks 2-digit number on the 7-segment displays.
 * 
 * @param   num The 2-digit number to be printed on the 7-segment displays.
 * @return  none.
 */
static void SSD_Blink(uint8_t num) {
    
    static uint8_t state = STATE_SHOW; // Blinking state
    
    switch (state) {
                
        case STATE_SHOW:
            if (g_FlagSSDBlink) {   // Check the 500 ms time event flag
            
            // Transit to state hide
            state = STATE_HIDE;
            SSD_PORT = 0;
            g_FlagSSDBlink = 0;
            
            } 
            else {
                SSD_Multiplex(num); // Show the temp value being set
            }
                    
            break;

        case STATE_HIDE:
            if (g_FlagSSDBlink) {   // Check the 500 ms time event flag
                
                // Transit to state show
                state = STATE_SHOW;
                g_FlagSSDBlink = 0;
                
            }
            break;

        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   Seven segment display composite state machine.
 * 
 * Checks the current mode of operation and which physical value to display.
 */
static void StateMachine_SSD(uint8_t tempCur) {

    switch (g_SSDState) {

        case STATE_TEMP_DISPLAY:
            
            // Display the current ADC temperature reading
            SSD_Multiplex(tempCur); 
            break;

        case STATE_TEMP_SET:
            
            // Blink the seven segment display (Nested state machine)
            SSD_Blink(g_TempSetNew);
            break;

        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   Heater LED state machine.
 */
static void StateMachine_LED(void) {

    switch (g_HeaterLED.state) {

        case STATE_ON:
            HEATER_LED = 1;
            break;

        case STATE_OFF:
            HEATER_LED = 0;
            break;

        case STATE_BLINK:
            if (g_HeaterLED.timerFlag) {    // Check the 500 ms time event flag
                
                HEATER_LED ^= 1;
                g_HeaterLED.timerFlag = 0;
                
            }
            break;

        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   The action applied when the up button is pressed.
 */
static void UpButton_TransitToPressed(void) {
    
    g_UpButton.state = STATE_PRESSED;
                
    if (g_SSDState == STATE_TEMP_DISPLAY) {   // First button press
        g_SSDState = STATE_TEMP_SET;    // Go to temp set state
    }
    else {
        if (g_TempSetNew < 75) {
            g_TempSetNew += 5;        
        }
    }
}

/**
 * @brief   The action applied when the down button is pressed.
 */
static void DownButton_TransitToPressed(void) {
    
    g_DownButton.state = STATE_PRESSED;
                
    if (g_SSDState == STATE_TEMP_DISPLAY) {   // First button press
        g_SSDState = STATE_TEMP_SET;    // Go to temp set state   
    }
    else {
        if (g_TempSetNew > 35) {
            g_TempSetNew -= 5;
        }
    }
}

/**
 * @brief   Up button state machine.
 */
static void StateMachine_UpButton(void) {
    
    switch (g_UpButton.state) {
        
        case STATE_RELEASED:
            if (UP_BUTTON_IS_PRESSED() == 1) {
                g_UpButton.state = STATE_PRE_PRESSED;
            }
            break;

        case STATE_PRESSED:
            if (UP_BUTTON_IS_PRESSED() == 0) {
                g_UpButton.state = STATE_PRE_RELEASED;
            }
            
            g_ButtonsTimer = 0;
            break;

        case STATE_PRE_RELEASED:
            if (g_UpButton.timer == DEBOUNCING_DELAY_MS) {
                g_UpButton.state = STATE_RELEASED;
            }

            g_ButtonsTimer = 0;
            break;
            
        case STATE_PRE_PRESSED:
            if (g_UpButton.timer == DEBOUNCING_DELAY_MS) {
                UpButton_TransitToPressed();
            }

            g_ButtonsTimer = 0;
            break;    

        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   Down button state machine.
 */
static void StateMachine_DownButton(void) {
    
    switch (g_DownButton.state) {
        
        case STATE_RELEASED:
            if (DOWN_BUTTON_IS_PRESSED() == 1) {
                g_DownButton.state = STATE_PRE_PRESSED;
            }

            break;

        case STATE_PRESSED:
            if (DOWN_BUTTON_IS_PRESSED() == 0) {
                g_DownButton.state = STATE_PRE_RELEASED;
            }

            g_ButtonsTimer = 0;
            break;

        case STATE_PRE_RELEASED:
            if (g_DownButton.timer == DEBOUNCING_DELAY_MS) {
                g_DownButton.state = STATE_RELEASED;
            }
            
            g_ButtonsTimer = 0;
            break;

        case STATE_PRE_PRESSED:
            if (g_DownButton.timer == DEBOUNCING_DELAY_MS) {
                DownButton_TransitToPressed();
            }
  
            g_ButtonsTimer = 0;
            break;
            
        default:
            ASSERT(0);
            break;
    }
}

/**
 * @brief   Up/Down buttons tasks handler.
 * 
 * Checks the up/down buttons and changes the required water temperature value
 *  #g_TempSet based on the input from the buttons.
 */
static void StateMachine_Buttons(void) {

    // Up button task
    StateMachine_UpButton();

    // Down button task
    StateMachine_DownButton();

    // Update the gTempSet value after 5 seconds from releasing the buttons
    if (g_ButtonsTimer >= 50 && g_SSDState == STATE_TEMP_SET) {
        
        g_TempSet = g_TempSetNew;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, g_TempSet);
        
        // SSD transit to temperature display state
        g_SSDState = STATE_TEMP_DISPLAY; 
        
    }
}

/**
 * @brief   On/Off button interrupt handler.
 */
void INTB0_ISR(void) {

    if (g_OnButton.state == STATE_ON) {
        g_OnButton.state = STATE_OFF;
    }
    else {
        g_OnButton.state = STATE_ON;
    }

    // Clear the interrupt flag
    INTCONbits.INTF = 0;
}