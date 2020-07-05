/* 
 * File:   SSD.c
 * Author: Diaa Eldeen
 *
 * Created on July 5, 2020, 10:50 AM
 */

#include "SSD.h"

// 7-segment driver


const unsigned char SEGMENT_MAP[10] = {0x3F,0x06,0x5B,0x4F,0x66, 
                                    0x6D,0x7D,0x07,0x7F,0x6F};

extern volatile uint8_t ms;
int8_t prevTime;
uint8_t state;

// 7-segment driver and multiplexing
void SSD_print(uint8_t digit) {
    SSD = SEGMENT_MAP[digit];
}

void SSD_init(void) {
    SSD_TRIS = 0x00;   // Output
    SSD = 0x00;   // Off
    
    SSD_CONTROL_TRIS &= ~ (SSD_CONTROL_BITS); // Output
    SSD_CONTROL &= ~ (SSD_CONTROL_BITS); // Off
    
    state = STATE_LSD;
    prevTime = -10;
    SSD_L = 1;
    SSD_M = 0;
}

void SSD_multiplex(uint8_t num) {
   if((int8_t)ms - prevTime >= 10) {
       // Switch segments
       if(state == STATE_MSD) {
           
           SSD = SEGMENT_MAP[num % 10];   // Extract LSD from count
           SSD_M = 1;    // Enable digit 1
           SSD_L = 0;    // Disable digit 2
           state = STATE_LSD;
           
        } else {
            SSD = SEGMENT_MAP[num / 10]; // Extract LSD from count
            SSD_L = 1; // Enable digit 2
            SSD_M = 0; // Disable digit 1 
            state = STATE_MSD;
        }

        if (prevTime >= 90)
            prevTime = -10;
        else
            prevTime = ms;
    } 
   else if(prevTime >= 90)
       prevTime = -10;
   
}