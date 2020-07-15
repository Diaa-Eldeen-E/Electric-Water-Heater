/* 
 * File:   stateMachines.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */


#include "stateMachines.h"

extern uint8_t gTempSet;

extern volatile uint8_t gFlagSSDBlink;
extern volatile uint8_t gButtonsTimer;

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