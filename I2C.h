/* 
 * File:   I2C.h
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 6:31 PM
 */

#ifndef I2C_H
#define	I2C_H

#include "xc.h"
#include "IO.h"

#ifdef	__cplusplus
extern "C" {
#endif


    
void I2C_masterInitialize(void);
void I2C_masterWait(void);
void I2C_masterStart(void);
void I2C_masterRepeatedStart(void);
void I2C_masterStop(void);
uint8_t I2C_masterWrite(uint8_t data);
uint8_t I2C_masterRead(uint8_t ack);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

