

/******************************************************************** 
*                File arduino_i2c.h
********************************************************************/

#ifndef WIRE_H
#define WIRE_H
#include <Wire.h>
#endif

#include "i2c.h"

//
// Created by User on 6/29/2017.
//

I2C::I2C(PinName sda, PinName scl) {


}


void I2C::read(int address, char * data, uint8_t length) {
    // char data;
    Wire.requestFrom(address, (int) length);  // Read one uint8_t from slave register address 
    for (int i = 0; i < length; ++i){
        *data = Wire.read();
    }
    // return (I2C_OK);                             // Return data read from slave register
}

void I2C::write(int address, char cmd, uint8_t length, bool repeated) {
    Wire.beginTransmission(address);  
    for (int i = 0; i < length; ++i){
        Wire.write(cmd);              
    }
    Wire.endTransmission(!repeated);
    // return (I2C_OK);                             // Return data read from slave register
}


void I2C::start() {
    Wire.begin();
}


void I2C::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
    Wire.beginTransmission(address);  // Initialize the Tx buffer
    Wire.write(subAddress);           // Put slave register address in Tx buffer
    Wire.write(data);                 // Put data in Tx buffer
    Wire.endTransmission();           // Send the Tx buffer
}

uint8_t I2C::readByte(uint8_t address, uint8_t subAddress)
{
    uint8_t data; // `data` will store the register data     
    Wire.beginTransmission(address);         // Initialize the Tx buffer
    Wire.write(subAddress);                  // Put slave register address in Tx buffer
    Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
    Wire.requestFrom(address, (uint8_t) 1);  // Read one uint8_t from slave register address 
    data = Wire.read();                      // Fill Rx buffer with result
    return data;                             // Return data read from slave register
}

void I2C::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
    Wire.beginTransmission(address);   // Initialize the Tx buffer
    Wire.write(subAddress);            // Put slave register address in Tx buffer
    Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
    uint8_t i = 0;
                Wire.requestFrom(address, count);  // Read bytes from slave register address 
    while (Wire.available()) {
                dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
}






/******************************************************************** 
*                Complete arduino_i2c.h
********************************************************************/
