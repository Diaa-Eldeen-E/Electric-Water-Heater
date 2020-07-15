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


    
void I2C_Initialize(void);
void I2C_Wait(void);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read(uint8_t ack);



#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

