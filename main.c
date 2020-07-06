/*
 * File:   main.c
 * Author: Diaa Eldeen
 *
 * Created on July 4, 2020, 8:36 PM
 */


#include "device_config.h"
#include <xc.h>
#include <stdint.h>


#include "timer.h"
#include "IO.h"
#include "SSD.h"
#include "ADC.h"


//volatile uint8_t ms = 0; // Count ms
//volatile uint8_t ms100 = 0; // Count quarter seconds
//uint8_t timer0ReloadVal;

led_t onLed;
button_t onButton;

void TMR0_ISR();

void turnOff(void) {

    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;

    //stop the fan

    SLEEP();

}

#define HEATING_ELEMENT PORTCbits.RC5
#define COOLING_ELEMENT PORTCbits.RC2

#define HEATING_ELEMNT_TURN_ON()    PORTCbits.RC5 = 1
#define HEATING_ELEMNT_TURN_OFF()   PORTCbits.RC5 = 0

#define COOLING_ELEMENT_TURN_ON()   PORTCbits.RC2 = 1
#define COOLING_ELEMENT_TURN_OFF()  PORTCbits.RC2 = 0

extern volatile uint8_t flag100MS;
extern volatile uint8_t flag5Sec;
extern volatile uint8_t flag500MS1;
extern volatile uint8_t flag500MS2;

// ADC read temp
    // Make an array, get the average of last 10 reads

// heating element
// cooling element

// temp adjustment buttons


#define STATE_HEATING   0
#define STATE_COOLING   1

void main(void) {


    // Make a state machine
    // Use the ticks time to drive timed behavior

    TMR0_Initialize();
    ADC_initialize();
    ledInit();
    buttonsInit();
    SSD_init();
    INTCONbits.GIE = 1;

    TRISC = 0;

    uint8_t tempVals[10] = {0};
    uint16_t tempAccum = 0;
    uint8_t idx = 0;
    uint8_t tempAvg =0;
    uint8_t tempCur =0;
    
    uint8_t tempSet = 60;
    
    uint8_t heaterState = 0;
    
    while (1) {

        if (onButton.state) {

            switch (onLed.state) {

                case STATE_INIT:
                    onLed.state = STATE_BLINK;
                    break;
                    
                case STATE_ON:
                    STATUS_LED = 1;
                    break;
                    
                case STATE_OFF:
                    STATUS_LED = 0;
                    break;

                case STATE_BLINK:
                    if (onLed.timerFlag) {
                        STATUS_LED ^= 1;
                        onLed.timerFlag = 0;
                    }
                    break;

                default:
                    break;

            }
            if(flag100MS) {
                tempAccum -= tempVals[idx];
                tempCur = ADC_readTemp();
                tempVals[idx] = tempCur;
                tempAccum += tempCur;
                
                tempAvg = tempAccum / 10;
                
                if(idx < 9)
                    idx++;
                else
                    idx = 0;
                
                flag100MS = 0;
            }
            switch(heaterState) {
                case STATE_HEATING:
                    if(tempAvg >= tempSet + 5)
                        heaterState = STATE_COOLING;
                    
                    else {
                        // blink LED state
                        onLed.state = STATE_BLINK;

                        // turn on heat
                        HEATING_ELEMNT_TURN_ON();

                        // turn off cool
                        COOLING_ELEMENT_TURN_OFF();
                    }
                    break;
                    
                case STATE_COOLING:
                    if(tempAvg <= tempSet - 5)
                        heaterState = STATE_HEATING;
                    else {
                        // LED on
                        onLed.state = STATE_ON;

                        // turn off heat
                        HEATING_ELEMNT_TURN_OFF();

                        // turn on cool
                        COOLING_ELEMENT_TURN_ON();
                    }
                    break;
                    
                default:
                    break;
            }
          
            SSD_multiplex(tempCur);
        }// The button is off, go to sleep
        else {
            turnOff();
        }

    }

    return;
}

// Use microChip youtube video as reference for debouncing

// On/Off button interrupt and debouncing

void __interrupt() INTERRUPT_InterruptManager(void) {

    if (INTCONbits.TMR0IF) {
        TMR0_ISR();
    }
    if (INTCONbits.INTF) {
        INTB0_ISR();
    }
}

