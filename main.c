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



button_t onButton;
button_t upButton;
button_t downButton;

void turnOff(void);


extern volatile uint8_t flag100MS;
extern volatile uint8_t flag500MS2;
extern volatile uint8_t buttonsTimer;

// ADC read temp
    // Make an array, get the average of last 10 reads

// heating element
// cooling element

// temp adjustment buttons


uint8_t tempSet = 60;

#define STATE_TEMP_SET  0
#define STATE_TEMP_DISP 1
#define STATE_SHOW  2
#define STATE_HIDE  3

uint8_t SSDState = STATE_TEMP_DISP;

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
    
    uint8_t tempSetLocal = tempSet;
    
    TRISAbits.TRISA3 = 0;
    PORTAbits.RA3 = 0;
    
    while (1) {

        if (onButton.state) {
            
            if(onButton.state != onButton.prevState) {
                // Read the tempSet from EEPROM
                
                onButton.prevState = 1;
            }
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
            
            switch (upButton.state){
                case STATE_HIGH:
                    if(UP_BUTTON == 0) {
                        upButton.timer = 0;
                        upButton.state = STATE_DEBOUNCE;
                    }
                    
                    upButton.prevState = STATE_HIGH;
                    
                    break;
                    
                case STATE_LOW:
                    if(upButton.state != upButton.prevState) {
                        if(SSDState == STATE_TEMP_DISP)
                            SSDState = STATE_TEMP_SET;
                        else {
                            if(tempSetLocal < 75)
                                tempSetLocal +=5;
                        }
                    }
                    if(UP_BUTTON == 1) {
                        upButton.timer = 0;
                        upButton.state = STATE_DEBOUNCE;
                    }
                    
                    upButton.prevState = STATE_LOW;
                    buttonsTimer = 0;
                    break;
                    
                case STATE_DEBOUNCE:
                    if(upButton.timer == 50) {
                        if(upButton.prevState == STATE_HIGH) {
                            upButton.state = STATE_LOW;
                        }
                        else {
                            upButton.state = STATE_HIGH;
                        }
                    }
                    buttonsTimer = 0;
                    
                    break;
                    
                default:
                    break;
            }
            
            switch (downButton.state) {
                case STATE_HIGH:
                    if (DOWN_BUTTON == 0) {
                        downButton.timer = 0;
                        downButton.state = STATE_DEBOUNCE;
                    }

                    downButton.prevState = STATE_HIGH;

                    break;

                case STATE_LOW:
                    if (downButton.state != downButton.prevState) {
                        if (SSDState == STATE_TEMP_DISP)
                            SSDState = STATE_TEMP_SET;
                        else {
                            if(tempSetLocal > 35)
                                tempSetLocal -= 5;
                        }
                    }
                    if (DOWN_BUTTON == 1) {
                        downButton.timer = 0;
                        downButton.state = STATE_DEBOUNCE;
                    }

                    downButton.prevState = STATE_LOW;
                    buttonsTimer = 0;

                    break;

                case STATE_DEBOUNCE:
                    if (downButton.timer == 50) {
                        if (downButton.prevState == STATE_HIGH) {
                            downButton.state = STATE_LOW;
                        } else {
                            downButton.state = STATE_HIGH;
                        }
                    }
                    buttonsTimer = 0;

                    break;

                default:
                    break;
            }
            
            stateMachine_heater(tempAvg);
            stateMachine_LED();
          
            if(buttonsTimer >= 50) {    // 5 secs (50 * 100 MS)
                SSDState = STATE_TEMP_DISP;
                tempSet = tempSetLocal;
                // EEPROM write tempSet value;
                buttonsTimer = 0;
            }
            
            switch(SSDState) {
                
                case STATE_TEMP_DISP:
                    SSD_multiplex(tempCur);
                    break;
                    
                case STATE_TEMP_SET:
                    SSDState = STATE_SHOW;
                    break;
                
                case STATE_SHOW:
                    if(flag500MS2) {
                        SSDState = STATE_HIDE;
                        flag500MS2 = 0;
                    }
                    SSD_multiplex(tempSetLocal);
                    
                    break;
                    
                case STATE_HIDE:
                    if(flag500MS2) {
                        SSDState = STATE_SHOW;
                        flag500MS2 = 0;
                    }
                    SSD_L = 0;
                    SSD_M = 0;
                    break;
                    
                default:
                    break;
            }
            
        }
        // The button is off, turn off the device
        else {
            onButton.prevState = 0;
            turnOff();
        }

    }

    return;
}



void __interrupt() INTERRUPT_InterruptManager(void) {

    if (INTCONbits.TMR0IF) {
        TMR0_ISR();
    }
    if (INTCONbits.INTF) {
        INTB0_ISR();
    }
}


void turnOff(void) {

//    PORTB = 0;
    PORTC = 0;
    PORTA = 0;
    PORTD = 0;
    PORTE = 0;

    //stop the fan

    SLEEP();
}