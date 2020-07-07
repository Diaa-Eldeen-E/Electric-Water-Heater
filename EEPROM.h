/* 
 * File:   EEPROM.h
 * Author: Diaa Eldeen
 *
 * Created on July 7, 2020, 5:51 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#include "I2C.h"

#ifdef	__cplusplus
extern "C" {
#endif

    
#define EEPROM_DEVICE_ADDR  0xa0    //10100000
#define EEPROM_SET_TEMP_ADDR    1

    
    
void EEPROM_writeByte(uint8_t address, uint8_t data);
uint8_t EEPROM_readByte(uint8_t address);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

