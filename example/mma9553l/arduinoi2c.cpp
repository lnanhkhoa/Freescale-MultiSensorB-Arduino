

/******************************************************************** 
*                File arduino_i2c.h
********************************************************************/
#include "Wire.h"
#include "arduinoi2c.h"



void I2c_ReadRegister(unsigned char SlaveID, unsigned char RegisterAddress,unsigned char *Data,unsigned int len){
	Wire.beginTransmission(SlaveID);   // Initialize the Tx buffer
	Wire.write(RegisterAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	int i = 0;
        Wire.requestFrom(SlaveID, len);  // Read bytes from slave register address 
	while (Wire.available()) {
        Data[i++] = Wire.read(); 
        }         // Put read results in the Rx buffer
}



void I2c_WriteRegister(unsigned char SlaveID, unsigned char RegisterAddress, unsigned char *Data,unsigned int len){
    int i;
    Wire.beginTransmission(SlaveID);  // Initialize the Tx buffer
	Wire.write(RegisterAddress); 
	for (i = 0; i < len; ++i){
		Wire.write(Data[i]);        
	}
	Wire.endTransmission();
}


/******************************************************************** 
*                Complete arduino_i2c.h
********************************************************************/
