/* 
 * File:   stateMachines.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */


#include "stateMachines.h"

extern volatile uint8_t flag500MS1;

uint8_t ledState = STATE_INIT;
uint8_t heaterState = STATE_HEATING;
extern uint8_t tempSet;

void stateMachine_heater(uint8_t tempAvg){
    
    
    switch(heaterState) {
        case STATE_HEATING:
            if (tempAvg >= tempSet + 5)
                heaterState = STATE_COOLING;

            else {
                // blink LED state
                ledState = STATE_BLINK;

                // turn on heat
                HEATING_ELEMNT_TURN_ON();

                // turn off cool
                COOLING_ELEMENT_TURN_OFF();
            }
            break;

        case STATE_COOLING:
            if (tempAvg <= tempSet - 5)
                heaterState = STATE_HEATING;
            else {
                // LED on
                ledState = STATE_ON;

                // turn off heat
                HEATING_ELEMNT_TURN_OFF();

                // turn on cool
                COOLING_ELEMENT_TURN_ON();
            }
            break;

        default:
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
            break;

    }
    
}