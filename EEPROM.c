/*
* Filename:     EEPROM.c
* Compiler:     XC8
* Target:       PIC16F877A
*
* MIT License 
*
* Copyright (c) 2020 - Diaa Eldeen
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

/******************************************************************************
 * @file    EEPROM.c
 * @brief   EEPROM device driver implementation
 * @author  Diaa Eldeen
 * @date    July 7, 2020
 ******************************************************************************/

/** 
 * @addtogroup EEPROM
 * @{
 */

//*****************************************************************************
//                                   Includes
//*****************************************************************************
#include "EEPROM.h"


//*****************************************************************************
// Function Definitions
//*****************************************************************************
/**
 * @brief   Write a byte to an external EEPROM.
 * 
 * Writes a data byte in a specific address to the I2C connected
 * external EEPROM. 
 * 
 * @param      address Address to be written to inside the EEPROM.
 * @param      data    Data to be written to the specified address.
 * @return     none.
 * @note    #EEPROM_DEVICE_ADDR must be defined with the correct EEPROM address
 *          on the bus.
 */
void EEPROM_WriteByte(uint8_t address, uint8_t data) {

    I2C_masterStart();
    while (I2C_masterWrite(EEPROM_DEVICE_ADDR)) //7 bit address + Write
        I2C_masterRepeatedStart();

    I2C_masterWrite((uint8_t) address);
    I2C_masterWrite(data);
    I2C_masterStop();
}


/**
 * @brief   Read a byte from an external EEPROM.
 * 
 * Reads a data byte from a specific address in the I2C connected
 * external EEPROM.
 * 
 * @param   address Address location to be read from the EEPROM.
 * @return  The Data inside that address location.
 * @note    #EEPROM_DEVICE_ADDR must be defined with the correct EEPROM address
 *          on the bus.
 */
uint8_t EEPROM_ReadByte(uint8_t address) {

    uint8_t data = 0;
    I2C_masterStart();
    while (I2C_masterWrite(EEPROM_DEVICE_ADDR)) //7 bit address + Write
        I2C_masterRepeatedStart();

    I2C_masterWrite((uint8_t) address); // Read address

    I2C_masterStart(); // Repeated start
    I2C_masterWrite(EEPROM_DEVICE_ADDR + 1); //7 bit address + Read
    data = I2C_masterRead(0);
    I2C_masterStop();

    return data;
}