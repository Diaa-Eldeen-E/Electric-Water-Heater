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

#ifdef	__cplusplus
extern "C" {
#endif

    
#define STATE_HEATING   0
#define STATE_COOLING   1
    
#define STATE_TEMP_SET      0
#define STATE_TEMP_DISPLAY  1
#define STATE_SHOW          2
#define STATE_HIDE          3
    

    
void stateMachine_heater(uint8_t tempAvg);
void SSD_task(uint8_t tempCur, uint8_t tempSetVar);
void stateMachine_LED(void);
int8_t buttonsUpDown_Task();


#ifdef	__cplusplus
}
#endif

#endif	/* STATEMACHINES_H */

