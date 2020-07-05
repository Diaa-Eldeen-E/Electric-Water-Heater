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

#include "IO.h"


#ifdef	__cplusplus
extern "C" {
#endif


void TMR0_Initialize(void);
void TMR0_ISR(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

