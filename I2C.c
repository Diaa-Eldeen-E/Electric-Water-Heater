/* 
 * File:   I2C.c
 * Author: Diaa Eldeen
 *
 * Created on July 6, 2020, 6:31 PM
 */


#include "I2C.h"


// Initialize the I2C master mode with 100kHz clock 
void I2C_Initialize(void) {
    
    // When enabling SSP, the SCL and SDA pins must be configured as inputs
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;

    // Disable slew rate, disable SMBus
    SSPSTAT = 0x00;

    // Enable SSP, I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSPCON = 0x28;

    SSPCON2 = 0x00;

    // Setting Clock Speed to 100kHz
    SSPADD = (_XTAL_FREQ / (4 * 100000)) - 1; 
}

// Wait until I2C is idle
void I2C_Wait(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); // Transmit is in progress
}

// I2C send start condition
void I2C_Start(void) {

    I2C_Wait();
    SEN = 1; // Initiate start condition
}

// I2C send repeated start condition
void I2C_RepeatedStart(void) {

    I2C_Wait();
    RSEN = 1; // Initiate repeated start condition
}

// I2C send stop condition
void I2C_Stop(void) {

    I2C_Wait();
    PEN = 1; // Initiate stop condition
}

// I2C write data byte to the I2C buffer and return the slave ACK status
uint8_t I2C_Write(uint8_t data) {

    I2C_Wait();
    SSPBUF = data; //Write data to SSPBUF
    
    I2C_Wait();
    return ACKSTAT;
}

/* I2C read data byte from the I2C buffer
 * Set the ack to 1 for consecutive byte reads or 0 on the last read operation
 */
uint8_t I2C_Read(uint8_t ack) {

    uint8_t data;

    I2C_Wait();
    RCEN = 1; // Receive enable
    I2C_Wait();

    data = SSPBUF; // Read data from SSPBUF
    
    I2C_Wait();
    ACKDT = (ack) ? 0 : 1; // Acknowledge bit
    ACKEN = 1; // Acknowledge sequence

    return data;
}





