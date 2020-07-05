/* 
 * File:   SSD.h
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 10:50 AM
 */

// 7-segment driver

#ifndef SSD_H
#define	SSD_H

#include "xc.h"
#include "stdint.h"
#include "timer.h"

#define SSD_TRIS    TRISD
#define SSD         PORTD
#define SSD_L    PORTAbits.RA2
#define SSD_M    PORTAbits.RA3

#define SSD_CONTROL_TRIS    TRISA
#define SSD_CONTROL PORTA
#define SSD_CONTROL_BITS    (_PORTA_RA2_MASK | _PORTA_RA3_MASK)

#define STATE_MSD   10
#define STATE_LSD   11

#ifdef	__cplusplus
extern "C" {
#endif


void SSD_print(uint8_t digit);
void SSD_init(void);
void SSD_multiplex(uint8_t num);

#ifdef	__cplusplus
}
#endif

#endif	/* SSD_H */

