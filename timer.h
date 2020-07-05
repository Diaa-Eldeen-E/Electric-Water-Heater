/* 
 * File:   timer.h
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 7:28 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "xc.h"
#include "stdint.h"


#ifdef	__cplusplus
extern "C" {
#endif

typedef struct led_t {
    uint8_t state;
    volatile uint8_t timerFlag;
} led_t;

void TMR0_Initialize();
void TMR0_ISR();

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

