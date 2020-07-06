/**
  ADC Driver API Header File

  @author
    Diaa Eldeen

  @file
    ADC.h

  @brief
    This header file provides APIs for driver for ADC.
 
*/



#ifndef ADC_H
#define ADC_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define ADC_CHANNEL 2
        
/**
  Section: Data Types Definitions
*/

/**
 *  result size of an A/D conversion
 */

typedef uint16_t adc_result_t;


/**
  Section: Macros
*/

/**
  @Summary
    Starts conversion

  @Description
    This routine is used to start conversion of desired channel.
    
  @Preconditions
    ADC_Initialize() function should have been called before calling this function.

  @Returns
    None

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC_Initialize();    
    ADC_StartConversion();
    convertedValue = ADC_GetConversionResult();
    </code>
*/
#define ADC_startConversion() ADCON0bits.GO_nDONE = 1



/**
  @Summary
    Returns true when the conversion is completed otherwise false.

  @Description
    This routine is used to determine if conversion is completed.
    When conversion is complete routine returns true. It returns false otherwise.

  @Preconditions
    ADC_Initialize() and ADC_StartConversion(void)
    function should have been called before calling this function.

  @Returns
    true  - If conversion is complete
    false - If conversion is not completed

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC_Initialize();
    ADC_StartConversion();

    while(!ADC_IsConversionDone());
    convertedValue = ADC_GetConversionResult();
    </code>
 */
#define ADC_isConversionDone() (!ADCON0bits.GO_nDONE)


/**
  @Summary
    Returns the ADC conversion value.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.

  @Preconditions
    This routine returns the conversion value only after the conversion is complete.
    Completion status can be checked using
    ADC_IsConversionDone() routine.

  @Returns
    Returns the converted value.

  @Param
    None

  @Example
    <code>
    uint16_t convertedValue;

    ADC_Initialize();
    ADC_StartConversion();

    while(ADC_IsConversionDone());

    convertedValue = ADC_GetConversionResult();
    </code>
 */
#define ADC_getConversionResult() ((adc_result_t)((ADRESH << 8) + ADRESL))


/**
  Section: ADC Module APIs
*/

/**
  @Summary
    Initializes the ADC

  @Description
    This routine initializes the Initializes the ADC.
    This routine must be called before any other ADC routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    uint16_t convertedValue;

    ADC_Initialize();
    convertedValue = ADC_GetConversionResult();
    </code>
*/
void ADC_initialize(void);



/**
  @Summary
    Returns the ADC conversion value
    also allows selection of a channel for conversion.

  @Description
    This routine is used to select desired channel for conversion
    and to get the analog to digital converted value.

  @Preconditions
    ADC_Initialize() function should have been called before calling this function.

  @Returns
    Returns the converted value.

  @Param
    Pass in required channel number.
    "For available channel refer to enum under adc.h file"

  @Example
    <code>
    uint16_t convertedValue;

    ADC_Initialize();

    conversion = ADC_GetConversion(AN1_Channel);
    </code>
*/
adc_result_t ADC_getConversion(void);



uint8_t ADC_readTemp(void);


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif	//ADC_H
/**
 End of File
*/

