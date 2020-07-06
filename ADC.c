/**
  ADC Driver File

  @author
    Diaa Eldeen

  @file
    ADC.c

  @brief
    This source file provides implementations for driver APIs for ADC.
  
*/


/**
  Section: Included Files
*/

#include <xc.h>
#include "ADC.h"
#include "device_config.h"



void ADC_initialize(void) {
    // Pin configuration
    TRISAbits.TRISA2 = 1;   // Input
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA0 = 1;
    TRISE = 0xff;
    
    // FOSC/32, power up
    ADCON0 = 0x81;   // bit 7 1, bit 6 0
    ADCON0bits.CHS = ADC_CHANNEL;
    
    // Right justified, FOSC/32, all channels analog mode, VDD reference
    ADCON1 = 0x80;  // bit 7 1, bit6 zero
    
    // Reset the ADC read registers
    ADRESL = 0x00;
    ADRESH = 0x00;
    
}


adc_result_t ADC_getConversion(void) {

    // Start the conversion
    ADCON0bits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE);

    // Conversion finished, return the result
    return ((adc_result_t)((ADRESH << 8) + ADRESL));
}

uint8_t ADC_readTemp(void) {
    adc_result_t result =  ADC_getConversion();
//    result = (float) result * 5 / 1024 * 100;
    result = result * 0.49;
    return result;
}

/**
 End of File
*/