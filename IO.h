/* 
 * File:   IO.h
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 9:04 AM
 */

#ifndef IO_H
#define	IO_H


#include "deviceConfig.h"

#include "xc.h"
#include <pic16f877a.h>
#include "stdint.h"


#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif


#define HEATER_LED_TRIS TRISB3
#define HEATER_LED      RB3

#define DOWN_BUTTON     RB1
#define DOWN_BUTTON_IS_PRESSED()  (DOWN_BUTTON == 0)
#define DOWN_BUTTON_IS_RELEASED()  (DOWN_BUTTON == 1)

#define UP_BUTTON       RB2
#define UP_BUTTON_IS_PRESSED()  (UP_BUTTON == 0)
#define UP_BUTTON_IS_RELEASED()  (UP_BUTTON == 1)

#define DEBOUNCING_DELAY_MS   50
    




    typedef struct LED_t {
        uint8_t state;
        volatile uint8_t timerFlag;
    } LED_t;

    typedef struct button_t {
        volatile uint8_t state;
        uint8_t prevState;
        volatile uint8_t timer;
    } button_t;

    void IO_Initialize(void);
    static void LED_Initialize(void);
    static void Buttons_Initialize(void);
    


#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif	/* IO_H */

