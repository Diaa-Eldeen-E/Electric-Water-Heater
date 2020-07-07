/* 
 * File:   EEPROM.c
 * Author: Diaa Eldeen
 *
 * Created on July 7, 2020, 5:51 PM
 */

#include "EEPROM.h"

extern uint8_t tempSet;



void EEPROM_writeByte(uint8_t address, uint8_t data) {

    I2C_masterStart();
    while (I2C_masterWrite(EEPROM_DEVICE_ADDR)) //7 bit address + Write
        I2C_masterRepeatedStart();

    I2C_masterWrite((uint8_t) address);
    I2C_masterWrite(data);
    I2C_masterStop();
}

uint8_t EEPROM_readByte(uint8_t address) {

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