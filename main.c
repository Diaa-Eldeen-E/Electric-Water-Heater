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
#include "stateMachines.h"


//volatile uint8_t ms = 0; // Count ms
//volatile uint8_t ms100 = 0; // Count quarter seconds

led_t onLed;
button_t onButton;


void turnOff(void) {

    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;

    //stop the fan

    SLEEP();

}



extern volatile uint8_t flag100MS;
extern volatile uint8_t flag5Sec;
extern volatile uint8_t flag500MS2;

// ADC read temp
    // Make an array, get the average of last 10 reads

// heating element
// cooling element

// temp adjustment buttons


uint8_t tempSet = 60;

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
    
    
    while (1) {

        if (onButton.state) {

            // ADC read temperature periodic routine
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
            
            stateMachine_heater(tempAvg);
            stateMachine_LED();
          
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

