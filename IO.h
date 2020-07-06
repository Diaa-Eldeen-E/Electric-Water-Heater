/* 
 * File:   IO.h
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 9:04 AM
 */

#ifndef IO_H
#define	IO_H


#include "device_config.h"

#include "xc.h"
#include "stdint.h"


#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    
#define STATUS_LED_TRIS TRISBbits.TRISB1
#define STATUS_LED      PORTBbits.RB1

#define HEATING_ELEMENT PORTCbits.RC5
#define COOLING_ELEMENT PORTCbits.RC2

#define HEATING_ELEMNT_TURN_ON()    PORTCbits.RC5 = 1
#define HEATING_ELEMNT_TURN_OFF()   PORTCbits.RC5 = 0

#define COOLING_ELEMENT_TURN_ON()   PORTCbits.RC2 = 1
#define COOLING_ELEMENT_TURN_OFF()  PORTCbits.RC2 = 0
    
    
   
typedef struct led_t {
    uint8_t state;
    volatile uint8_t timerFlag;
} led_t;


typedef struct button_t {
    uint8_t state;
    uint8_t prevState;
    volatile uint8_t timerFlag;
} button_t;

void ledInit(void);
void buttonsInit(void);
void INTB0_ISR(void);


#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif	/* IO_H */

