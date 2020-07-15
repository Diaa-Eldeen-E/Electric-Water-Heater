/* 
 * File:   stateMachines.h
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */

#ifndef STATEMACHINES_H
#define	STATEMACHINES_H

#include "IO.h"
#include "SSD.h"
#include "EEPROM.h"
#include "heater.h"
#include "ADC.h"
#include "timer.h"

#ifdef	__cplusplus
extern "C" {
#endif

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
    STATE_DEBOUNCE        
}ButtonState_t;  


void StateMachine_Initialize(void);
void StateMachine_TurnOff(void);
void StateMachine_OnWakeUp(void);
void StateMachine_Run(void);
void StateMachine_Heater(uint8_t tempAvg);
void StateMachine_SSD(uint8_t tempCur);
void StateMachine_LED(void);
void StateMachine_Buttons(void);
void INTB0_ISR(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATEMACHINES_H */

