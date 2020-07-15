/* 
 * File:   stateMachines.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */


#include "stateMachines.h"

static uint8_t gTempSet;    ///< The desired temperature value

extern volatile uint8_t gFlagADCPeriod; ///< ADC reading routine time event
extern volatile uint8_t gFlagSSDBlink;  ///< SSD blinking time event
extern volatile uint8_t gButtonsTimer;  ///< 100 milliseconds ticks counter

LED_t heaterLED;
button_t onButton;
button_t upButton;
button_t downButton;

static uint8_t heaterState;
static uint8_t SSDState;

void StateMachine_Initialize(void) {

    heaterLED.state = STATE_OFF;
    heaterState = STATE_HEATING;
    SSDState = STATE_TEMP_DISPLAY;

    onButton.state = STATE_OFF; // Start the heater device with off state
    upButton.state = STATE_RELEASED;
    downButton.state = STATE_RELEASED;
}

void StateMachine_TurnOff(void) {
    
    onButton.prevState = STATE_OFF;
    //    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;

    //stop the fan

    SLEEP();
}

void StateMachine_OnWakeUp(void) {
    // Read the gTempSet from the EEPROM
    gTempSet = EEPROM_ReadByte(EEPROM_SET_TEMP_ADDR);

    // Initialize the gTempSet value if not initialized
    if (gTempSet == 0xff) {
        gTempSet = 60;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, gTempSet);
    }

    onButton.prevState = 1;
}


void StateMachine_Run(void) {
    
    uint8_t tempVals[10] = {0}; // Holds the last 10 temperature readings
    uint16_t tempAccum = 0; // The summation of the last 10 readings
    uint8_t idx = 0; // tempVals array index
    uint8_t tempAvg = 0; // Average of the last 10 readings
    uint8_t tempCur = 0; // Current temperature reading
    int8_t tempDiff; // Up/Down buttons temperature difference to be updated
    
    while (1) {

        if (onButton.state == STATE_ON) {

            if (onButton.prevState == STATE_OFF) {
                StateMachine_OnWakeUp();
            }

            // ADC read temperature periodic routine
            if (gFlagADCPeriod) {

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

                gFlagADCPeriod = 0;
            }

            tempDiff = StateMachine_Buttons();
            StateMachine_Heater(tempAvg);
            StateMachine_LED();
            StateMachine_SSD(tempCur, gTempSet + tempDiff);
        }
        // The on button is off, turn off the device
        else {
            StateMachine_TurnOff();
        }
    }
    
    ASSERT(0);  // This should never be reached
}


void StateMachine_Heater(uint8_t tempAvg) {

    switch (heaterState) {

        case STATE_HEATING:
            if (tempAvg >= gTempSet + 5)
                heaterState = STATE_COOLING;
            else {
                Heater_TurnOn();
                heaterLED.state = STATE_BLINK;
            }
            break;

        case STATE_COOLING:
            if (tempAvg <= gTempSet - 5)
                heaterState = STATE_HEATING;
            else {
                Heater_TurnOff();
                heaterLED.state = STATE_ON;
            }
            break;

        default:
            ASSERT(0);
            break;
    }

}

void StateMachine_SSD(uint8_t tempCur, uint8_t gTempSetVar) {

    static uint8_t blinkState = STATE_SHOW; // Nested state inside STATE_TEMP_SET

    switch (SSDState) {

        case STATE_TEMP_DISPLAY:
            SSD_Multiplex(tempCur); // Display the ADC temperature reading
            break;

        case STATE_TEMP_SET:
            // Blink the seven segment display
            switch (blinkState) {
                case STATE_SHOW:
                    if (gFlagSSDBlink) {
                        blinkState = STATE_HIDE;
                        SSD_PORT = 0;
                        gFlagSSDBlink = 0;
                    } else
                        SSD_Multiplex(gTempSetVar);
                    break;

                case STATE_HIDE:
                    if (gFlagSSDBlink) {
                        blinkState = STATE_SHOW;
                        gFlagSSDBlink = 0;
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

    switch (heaterLED.state) {

        case STATE_ON:
            HEATER_LED = 1;
            break;

        case STATE_OFF:
            HEATER_LED = 0;
            break;

        case STATE_BLINK:
            if (heaterLED.timerFlag) {
                HEATER_LED ^= 1;
                heaterLED.timerFlag = 0;
            }
            break;

        default:
            ASSERT(0);
            break;
    }

}

int8_t StateMachine_Buttons(void) {

    static int8_t tempDiff = 0; // Holds the update amount of gTempSet value

    // Up button task
    switch (upButton.state) {
        case STATE_RELEASED:
            if (UP_BUTTON_IS_PRESSED()) {
                upButton.state = STATE_DEBOUNCE;
                upButton.timer = 0;
            }

            upButton.prevState = STATE_RELEASED;
            break;

        case STATE_PRESSED:
            if (upButton.state != upButton.prevState) {
                
                if (SSDState == STATE_TEMP_DISPLAY)
                    SSDState = STATE_TEMP_SET;
                else
                    if (gTempSet + tempDiff < 75)
                        tempDiff += 5;
            }
            if (UP_BUTTON_IS_RELEASED()) {
                upButton.timer = 0;
                upButton.state = STATE_DEBOUNCE;
            }

            upButton.prevState = STATE_PRESSED;
            gButtonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (upButton.timer == DEBOUNCING_DELAY_MS) {

                if (upButton.prevState == STATE_RELEASED)
                    upButton.state = STATE_PRESSED;
                else
                    upButton.state = STATE_RELEASED;
            }
            gButtonsTimer = 0;
            break;

        default:
            ASSERT(0);
            break;
    }

    // Down button task
    switch (downButton.state) {
        case STATE_RELEASED:
            if (DOWN_BUTTON_IS_PRESSED()) {
                downButton.state = STATE_DEBOUNCE;
                downButton.timer = 0;
            }

            downButton.prevState = STATE_RELEASED;
            break;

        case STATE_PRESSED:
            if (downButton.state != downButton.prevState) {
                
                if (SSDState == STATE_TEMP_DISPLAY)
                    SSDState = STATE_TEMP_SET;
                else
                    if (gTempSet + tempDiff > 35)
                        tempDiff -= 5;
            }
            if (DOWN_BUTTON_IS_RELEASED()) {
                downButton.state = STATE_DEBOUNCE;
                downButton.timer = 0;
            }

            downButton.prevState = STATE_PRESSED;
            gButtonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (downButton.timer == DEBOUNCING_DELAY_MS) {

                if (downButton.prevState == STATE_RELEASED)
                    downButton.state = STATE_PRESSED;
                else
                    downButton.state = STATE_RELEASED;
            }
            gButtonsTimer = 0;
            break;

        default:
            ASSERT(0);
            break;
    }

    // Update the gTempSet value after 5 seconds from releasing the buttons
    if (gButtonsTimer >= 50 && SSDState == STATE_TEMP_SET) {
        SSDState = STATE_TEMP_DISPLAY;
        gTempSet += tempDiff;
        tempDiff = 0;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, gTempSet);
        gButtonsTimer = 0;
    }

    return tempDiff;
}

void INTB0_ISR(void) {

    if (onButton.state) {
        onButton.state = 0;
    } else {
        onButton.state = 1;
    }

    // Clear the interrupt flag
    INTCONbits.INTF = 0;
}