

/******************************************************************** 
*                File arduino_i2c.h
********************************************************************/
#ifndef WIRE_H
#define WIRE_H
#include "Wire.h"
#endif

#include "i2c.h"

//
// Created by User on 6/29/2017.
//

I2C::I2C(PinName sda, PinName scl, const char *name) {

}


void I2C::frequency(int hz) {


}


int I2C::read(int address, char *data, int length, bool repeated) {
    int i = 0;
    Wire.requestFrom(address, length);
    while (Wire.available() && i<=length) {
        data[i++] = Wire.read();
    }   
}

int I2C::write(int address, const char *cmd, int length, bool repeated) {

    int i;
    Wire.beginTransmission(address);  // Initialize the Tx buffer
    Wire.write(address); 
    for (i = 0; i < length; ++i){
        Wire.write(cmd[i]);        
    }
    Wire.endTransmission(!repeated);
}




void I2C::start() {
    Wire.begin();
}


void I2C::stop() {
    Wire.endTransmission();
}


void I2C::aquire() {
    
}



// void I2c_ReadRegister(unsigned char SlaveID, unsigned char RegisterAddress,unsigned char *Data,unsigned int len){
//  Wire.beginTransmission(SlaveID);   // Initialize the Tx buffer
//  Wire.write(RegisterAddress);            // Put slave register address in Tx buffer
//  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
//  int i = 0;
//         Wire.requestFrom(SlaveID, len);  // Read bytes from slave register address 
//  while (Wire.available()) {
//         Data[i++] = Wire.read(); 
//         }         // Put read results in the Rx buffer
// }



// void I2c_WriteRegister(unsigned char SlaveID, unsigned char RegisterAddress, unsigned char *Data,unsigned int len){
//     int i;
//     Wire.beginTransmission(SlaveID);  // Initialize the Tx buffer
//  Wire.write(RegisterAddress); 
//  for (i = 0; i < len; ++i){
//      Wire.write(Data[i]);        
//  }
//  Wire.endTransmission();
// }


/******************************************************************** 
*                Complete arduino_i2c.h
********************************************************************/
