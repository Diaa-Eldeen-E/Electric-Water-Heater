/* 
 * File:   stateMachines.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */


#include "stateMachines.h"

extern volatile uint8_t flag500MS1;
extern volatile uint8_t flag500MS2;
extern volatile uint8_t buttonsTimer;

static uint8_t ledState = STATE_INIT;
static uint8_t heaterState = STATE_HEATING;
static uint8_t SSDState = STATE_TEMP_DISPLAY;

extern uint8_t tempSet;



button_t upButton;
button_t downButton;

void stateMachine_heater(uint8_t tempAvg) {

    switch (heaterState) {

        case STATE_HEATING:
            if (tempAvg >= tempSet + 5)
                heaterState = STATE_COOLING;
            else {
                Heater_TurnOn();
                ledState = STATE_BLINK;
            }
            break;

        case STATE_COOLING:
            if (tempAvg <= tempSet - 5)
                heaterState = STATE_HEATING;
            else {
                Heater_TurnOff();
                ledState = STATE_ON;
            }
            break;

        default:
            ASSERT(0);
            break;
    }

}

void SSD_task(uint8_t tempCur, uint8_t tempSetVar) {

    static uint8_t blinkState = STATE_SHOW; // Nested state inside STATE_TEMP_SET

    switch (SSDState) {

        case STATE_TEMP_DISPLAY:
            SSD_multiplex(tempCur); // Display the ADC temperature reading
            break;

        case STATE_TEMP_SET:
            // Blink the seven segment display
            switch (blinkState) {
                case STATE_SHOW:
                    if (flag500MS2) {
                        blinkState = STATE_HIDE;
                        SSD = 0;
                        flag500MS2 = 0;
                    } 
                    else
                        SSD_multiplex(tempSetVar);
                    break;

                case STATE_HIDE:
                    if (flag500MS2) {
                        blinkState = STATE_SHOW;
                        flag500MS2 = 0;
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

void stateMachine_LED(void) {

    switch (ledState) {

        case STATE_INIT:
            ledState = STATE_BLINK;
            break;

        case STATE_ON:
            STATUS_LED = 1;
            break;

        case STATE_OFF:
            STATUS_LED = 0;
            break;

        case STATE_BLINK:
            if (flag500MS1) {
                STATUS_LED ^= 1;
                flag500MS1 = 0;
            }
            break;

        default:
            ASSERT(0);
            break;

    }

}

int8_t buttonsUpDown_Task(void) {

    static int8_t tempSetDiff = 0; // Holds the update amount of tempSet value

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
                    if (tempSet + tempSetDiff < 75)
                    tempSetDiff += 5;
            }
            if (UP_BUTTON_IS_RELEASED()) {
                upButton.timer = 0;
                upButton.state = STATE_DEBOUNCE;
            }

            upButton.prevState = STATE_PRESSED;
            buttonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (upButton.timer == DEBOUNCING_DELAY_MS) {

                if (upButton.prevState == STATE_RELEASED)
                    upButton.state = STATE_PRESSED;
                else
                    upButton.state = STATE_RELEASED;
            }
            buttonsTimer = 0;
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
                    if (tempSet + tempSetDiff > 35)
                    tempSetDiff -= 5;
            }
            if (DOWN_BUTTON_IS_RELEASED()) {
                downButton.state = STATE_DEBOUNCE;
                downButton.timer = 0;
            }

            downButton.prevState = STATE_PRESSED;
            buttonsTimer = 0;
            break;

        case STATE_DEBOUNCE:
            if (downButton.timer == DEBOUNCING_DELAY_MS) {

                if (downButton.prevState == STATE_RELEASED)
                    downButton.state = STATE_PRESSED;
                else
                    downButton.state = STATE_RELEASED;
            }
            buttonsTimer = 0;
            break;

        default:
            ASSERT(0);
            break;
    }

    // Update the tempSet value after 5 seconds from releasing the buttons
    if (buttonsTimer >= 50 && SSDState == STATE_TEMP_SET) {
        SSDState = STATE_TEMP_DISPLAY;
        tempSet += tempSetDiff;
        tempSetDiff = 0;
        EEPROM_WriteByte(EEPROM_SET_TEMP_ADDR, tempSet);
        buttonsTimer = 0;
    }

    return tempSetDiff;
}