/* 
 * File:   stateMachines.h
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 7:55 AM
 */

#ifndef STATEMACHINES_H
#define	STATEMACHINES_H

#include "IO.h"


#ifdef	__cplusplus
extern "C" {
#endif

    
#define STATE_INIT  0
#define STATE_ON    1
#define STATE_OFF   2
#define STATE_BLINK 3
    

#define STATE_HEATING   0
#define STATE_COOLING   1
    
    
void stateMachine_heater(uint8_t tempAvg);
void stateMachine_LED(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATEMACHINES_H */

