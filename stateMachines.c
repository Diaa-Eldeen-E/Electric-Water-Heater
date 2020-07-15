/* 
 * File:   stateMachines.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */


#include "stateMachines.h"

static uint8_t g_TempSet;    ///< The desired temperature value
static uint8_t g_TempSetNew;    ///< The choosen temperature value by buttons

extern volatile uint8_t g_FlagADCPeriod; ///< ADC reading routine time event
extern volatile uint8_t g_FlagSSDBlink;  ///< SSD blinking time event
extern volatile uint8_t g_ButtonsTimer;  ///< 100 milliseconds ticks counter

LED_t g_HeaterLED;
button_t g_OnButton;
button_t g_UpButton;
button_t g_DownButton;

static uint8_t g_HeaterState;
static uint8_t g_SSDState;

void StateMachine_Initialize(void) {

    g_HeaterLED.state = STATE_OFF;
    g_HeaterState = STATE_INIT;
    g_SSDState = STATE_TEMP_DISPLAY;

    g_OnButton.state = STATE_OFF; // Start the heater device with off state
    g_UpButton.state = STATE_RELEASED;
    g_DownButton.state = STATE_RELEASED;
}

void StateMachine_TurnOff(void) {
    
    g_OnButton.prevState = STATE_OFF;
    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;

    //stop the fan

    SLEEP();
}

void StateMachine_OnWakeUp(void) {
    // Read the gTempSet from the EEPROM
    g_TempSet = EEPROM_ReadByte(EEPROM_SET_TEMP_ADDR);

    // Initialize the gTempSet value if not initialized
    if (g_TempSet == 0xff) {
        g_TempSet = 60;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, g_TempSet);
    }
    
    g_TempSetNew = g_TempSet;
    
    g_HeaterState = STATE_INIT;
    g_SSDState = STATE_TEMP_DISPLAY;
    g_UpButton.state = STATE_RELEASED;
    g_DownButton.state = STATE_RELEASED;
    g_OnButton.prevState = STATE_ON;
}


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

                tempCur = ADC_ReadTemp();

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

void Heater_TransitToHeating(void) {
    g_HeaterState = STATE_HEATING;
    Heater_TurnOn();
    g_HeaterLED.state = STATE_BLINK;
}
void Heater_TransitToCooling(void) {
    g_HeaterState = STATE_COOLING;
    Heater_TurnOff();
    g_HeaterLED.state = STATE_ON;
}
void StateMachine_Heater(uint8_t tempAvg) {

    switch (g_HeaterState) {
        
        case STATE_INIT:
            if(tempAvg >= g_TempSet + 5) {
                Heater_TransitToCooling();
            }
            else {
                Heater_TransitToHeating();
            }

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

void StateMachine_SSD(uint8_t tempCur) {

    static uint8_t blinkState = STATE_SHOW; // Nested state inside STATE_TEMP_SET

    switch (g_SSDState) {

        case STATE_TEMP_DISPLAY:
            SSD_Multiplex(tempCur); // Display the ADC temperature reading
            break;

        case STATE_TEMP_SET:
            // Blink the seven segment display
            switch (blinkState) {
                case STATE_SHOW:
                    if (g_FlagSSDBlink) {
                        blinkState = STATE_HIDE;
                        SSD_PORT = 0;
                        g_FlagSSDBlink = 0;
                    } else
                        SSD_Multiplex(g_TempSetNew);
                    break;

                case STATE_HIDE:
                    if (g_FlagSSDBlink) {
                        blinkState = STATE_SHOW;
                        g_FlagSSDBlink = 0;
                    }
                    break;

                default:
                    ASSERT(0);
                    break;
            }
            break;

        default:
            ASSERT(0);
            break;
    }
}

void StateMachine_LED(void) {

    switch (g_HeaterLED.state) {

        case STATE_ON:
            HEATER_LED = 1;
            break;

        case STATE_OFF:
            HEATER_LED = 0;
            break;

        case STATE_BLINK:
            if (g_HeaterLED.timerFlag) {
                HEATER_LED ^= 1;
                g_HeaterLED.timerFlag = 0;
            }
            break;

        default:
            ASSERT(0);
            break;
    }

}

void StateMachine_Buttons(void) {

    // Up button task
    switch (g_UpButton.state) {
        case STATE_RELEASED:
            if (UP_BUTTON_IS_PRESSED()) {
                g_UpButton.state = STATE_DEBOUNCE;
                g_UpButton.timer = 0;
            }

            g_UpButton.prevState = STATE_RELEASED;
            break;

        case STATE_PRESSED:
            if (g_UpButton.state != g_UpButton.prevState) {
                
                if (g_SSDState == STATE_TEMP_DISPLAY)
                    g_SSDState = STATE_TEMP_SET;
                else
                    if (g_TempSetNew < 75)
                        g_TempSetNew += 5;
            }
            if (UP_BUTTON_IS_RELEASED()) {
                g_UpButton.timer = 0;
                g_UpButton.state = STATE_DEBOUNCE;
            }

            g_UpButton.prevState = STATE_PRESSED;
            g_ButtonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (g_UpButton.timer == DEBOUNCING_DELAY_MS) {

                if (g_UpButton.prevState == STATE_RELEASED)
                    g_UpButton.state = STATE_PRESSED;
                else
                    g_UpButton.state = STATE_RELEASED;
            }
            g_ButtonsTimer = 0;
            break;

        default:
            ASSERT(0);
            break;
    }

    // Down button task
    switch (g_DownButton.state) {
        case STATE_RELEASED:
            if (DOWN_BUTTON_IS_PRESSED()) {
                g_DownButton.state = STATE_DEBOUNCE;
                g_DownButton.timer = 0;
            }

            g_DownButton.prevState = STATE_RELEASED;
            break;

        case STATE_PRESSED:
            if (g_DownButton.state != g_DownButton.prevState) {
                
                if (g_SSDState == STATE_TEMP_DISPLAY)
                    g_SSDState = STATE_TEMP_SET;
                else
                    if (g_TempSetNew > 35)
                        g_TempSetNew -= 5;
            }
            if (DOWN_BUTTON_IS_RELEASED()) {
                g_DownButton.state = STATE_DEBOUNCE;
                g_DownButton.timer = 0;
            }

            g_DownButton.prevState = STATE_PRESSED;
            g_ButtonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (g_DownButton.timer == DEBOUNCING_DELAY_MS) {

                if (g_DownButton.prevState == STATE_RELEASED)
                    g_DownButton.state = STATE_PRESSED;
                else
                    g_DownButton.state = STATE_RELEASED;
            }
            g_ButtonsTimer = 0;
            break;

        default:
            ASSERT(0);
            break;
    }

    // Update the gTempSet value after 5 seconds from releasing the buttons
    if (g_ButtonsTimer >= 50 && g_SSDState == STATE_TEMP_SET) {
        g_SSDState = STATE_TEMP_DISPLAY;
        g_TempSet = g_TempSetNew;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, g_TempSet);
        g_ButtonsTimer = 0;
    }

}

void INTB0_ISR(void) {

    if (g_OnButton.state) {
        g_OnButton.state = 0;
    } else {
        g_OnButton.state = 1;
    }

    // Clear the interrupt flag
    INTCONbits.INTF = 0;
}