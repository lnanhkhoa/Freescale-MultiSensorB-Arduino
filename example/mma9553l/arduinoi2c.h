#ifndef ARDUINOI2C_H
#define ARDUINOI2C_H


void I2c_ReadRegister(unsigned char SlaveID, unsigned char RegisterAddress,unsigned char *Data,unsigned int len);

void I2c_WriteRegister(unsigned char SlaveID, unsigned char RegisterAddress, unsigned char *Data,unsigned int len);


#endif