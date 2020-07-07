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
#include "EEPROM.h"



button_t onButton;

void turnOff(void);


extern volatile uint8_t flag100MS;


uint8_t tempSet = 60;


void main(void) {

    TMR0_Initialize();
    ADC_initialize();
    ledInit();
    buttonsInit();
    SSD_init();
    I2C_masterInitialize();
    
    INTCONbits.GIE = 1; // Enable global interrupts

    TRISCbits.TRISC2 = 0; // Cooling element
    TRISCbits.TRISC5 = 0; // Heating element

    uint8_t tempVals[10] = {0};
    uint16_t tempAccum = 0;
    uint8_t idx = 0;
    uint8_t tempAvg = 0;
    uint8_t tempCur = 0;


    while (1) {

        if (onButton.state) {

            // Initialization
            if (onButton.prevState == 0) {
                
                // Read the tempSet from the EEPROM
                tempSet = EEPROM_readByte(EEPROM_SET_TEMP_ADDR);

                // Initialize the tempSet value if not initialized
                if (tempSet == 0xff) {
                    tempSet = 60;
                    EEPROM_writeByte(EEPROM_SET_TEMP_ADDR, tempSet);
                }
                
                onButton.prevState = 1;
            }
            
            // ADC read temperature periodic routine
            if (flag100MS) {
                
                // Subtract the tenth read before the current one
                tempAccum -= tempVals[idx]; 
                
                tempCur = ADC_readTemp();
                
                // Add the current read to the previous 9 reads
                tempAccum += tempCur;   
                
                tempAvg = tempAccum / 10;   // Average of 10 reads
                
                // Save the current temperature read in the array
                tempVals[idx] = tempCur;

                if (idx < 9)
                    idx++;
                else
                    idx = 0;

                flag100MS = 0;
            }

            int8_t tempSetDiff = buttonsUpDown_Task();

            stateMachine_heater(tempAvg);
            stateMachine_LED();
            SSD_task(tempCur, tempSet + tempSetDiff);
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