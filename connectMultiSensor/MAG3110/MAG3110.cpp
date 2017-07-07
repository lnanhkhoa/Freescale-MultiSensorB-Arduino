
#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif



#ifndef WIRE_H
#define WIRE_H
#include "Wire.h"
#endif


#include "MAG3110.h"
#include "i2c.h"

#define UINT14_MAX        16383

// Specify sensor sample data rate and oversampling
uint8_t magODR = mODR_80Hz_16os;



I2C _i2c(0, 0);
int m_addr = MAG3110_ADDRESS;

/******************************************************************************
 * Constructors
 ******************************************************************************/
MAG3110::MAG3110(PinName sda, PinName scl)
{
    _i2c = decltype(_i2c)(sda, scl);
    // char cmd[2];
    // cmd[0] = MAG_CTRL_REG2;
    // cmd[1] = 0x80;
    // _i2c.write(m_addr, cmd, 2);

}

MAG3110::~MAG3110(){

}

void MAG3110::begin(void){
    _i2c.start();
}


// Set up sensor software reset
void MAG3110::Reset(void) 
{
    _i2c.writeByte(m_addr, MAG_CTRL_REG2, 0x10); // set reset bit to 1 to assert software reset to zero at end of boot process
}


uint8_t MAG3110::status(void){
    return _i2c.readByte(m_addr, MAG_DR_STATUS);
}

uint8_t  MAG3110::getWhoAmI(void)
{
    uint8_t d = _i2c.readByte(m_addr, MAG_WHOAMI);
    return(d);
}

uint32_t MAG3110::dataReady(void) {
    uint8_t stat = 0;
    readRegs(MAG_DR_STATUS, &stat, 1);
    return (uint32_t) stat;
}


void MAG3110::ReadXYZraw(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  _i2c.readBytes(m_addr, MAG_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
  destination[0] = ((int16_t) rawData[0] << 8 | rawData[1]);
  destination[1] = ((int16_t) rawData[2] << 8 | rawData[3]);
  destination[2] = ((int16_t) rawData[4] << 8 | rawData[5]);
}

void MAG3110::ReadXYZ(float * destination){
    int16_t des[3];
    ReadXYZraw(des);
    destination[0] = (float)des[0]*10./32768.;  // get actual Gauss value 
    destination[1] = (float)des[1]*10./32768.;   
    destination[2] = (float)des[2]*10./32768.;
}



int8_t readTempData()
{
  return (int8_t) _i2c.readByte(m_addr, MAG_DIE_TEMP);  // Read the 8-bit 2's complement data register 
}


// Allow user compensation of acceleration errors
void MAG3110::Offsets(void)
{
   Standby();  // Must be in standby to change registers
   
   _i2c.writeByte(m_addr, MAG_OFF_X_MSB, 0x00); 
   _i2c.writeByte(m_addr, MAG_OFF_X_LSB, 0x00); 
   _i2c.writeByte(m_addr, MAG_OFF_Y_MSB, 0x00); 
   _i2c.writeByte(m_addr, MAG_OFF_Y_LSB, 0x00); 
   _i2c.writeByte(m_addr, MAG_OFF_Z_MSB, 0x00); 
   _i2c.writeByte(m_addr, MAG_OFF_Z_LSB, 0x00); 
   
   Active();  // Set to active to start reading
}

// Initialize the MAG3110 registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MAG3110
// Feel free to modify any values, these are settings that work well for me.
void MAG3110::init()
{
  Standby();  // Must be in standby to change registers

  // Set up the magnetometer sample rate and oversample ratio
    _i2c.writeByte(m_addr, MAG_CTRL_REG1, magODR << 3);  
  // Enable automatic magnetic sensor resets
    _i2c.writeByte(m_addr, MAG_CTRL_REG2, 0x80);  // set normal mode, correct with user offset registers

  Active();  // Set to active to start reading
}

// Sets the MAG3110 to standby mode.
// It must be in standby to change most register settings
void MAG3110::Standby()
{
  byte c = _i2c.readByte(m_addr, MAG_CTRL_REG1);
  _i2c.writeByte(m_addr, MAG_CTRL_REG1, c & ~(0x01));
}

// Sets the MAG3110 to active mode.
// Needs to be in this mode to output data
void MAG3110::Active()
{
  byte c = _i2c.readByte(m_addr, MAG_CTRL_REG1);
  _i2c.writeByte(m_addr, MAG_CTRL_REG1, c | 0x01);  
}




char MAG3110::WhoAmIValue(){
    return MAG3110_WHO_AM_I_VALUE;
}




void MAG3110::writeRegs(uint8_t * data, int len) {
    _i2c.write(m_addr, (char *)data, len);
}


void MAG3110::readRegs(int cmdl, uint8_t * data, int len){

    char cmd[1];
    cmd[0] = cmdl;
    _i2c.write( m_addr, cmd, 1, true);
    _i2c.read( m_addr, (char *) data, len);
}















/*

// read a register per, pass first reg value, reading 2 bytes increments register
// Reads MSB first then LSB
int MAG3110::readVal(char regAddr)
{
    char cmd[2];
    int16_t t;
    cmd[0] = regAddr;
    if(_i2c.write(m_addr, cmd, 1)) {
        printf("MAG3110 write error\r\n");
        _i2c.stop();
        _i2c.start();
        }

    cmd[0] = 0x00;
    cmd[1] = 0x00;
    _i2c.read(m_addr, cmd, 2);
    t = (cmd[0] * 256) + (unsigned short) cmd[1];
    return ((int) t); //concatenate the MSB and LSB
}


float MAG3110::getHeading()
{
    int xVal = readVal(MAG_OUT_X_MSB);
    int yVal = readVal(MAG_OUT_Y_MSB);
    return (atan2((double)(yVal - _avgY),(double)(xVal - _avgX)))*180/PI;
}

void MAG3110::getValues(int *xVal, int *yVal, int *zVal)
{
    *xVal = readVal(MAG_OUT_X_MSB);
    *yVal = readVal(MAG_OUT_Y_MSB);
    *zVal = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::ReadXYZ(float * mag)
{
    int x, y, z;
    x = readVal(MAG_OUT_X_MSB);
    y = readVal(MAG_OUT_Y_MSB);
    z = readVal(MAG_OUT_Z_MSB);
    mag[0] = (float) x / 10.0;
    mag[1] = (float) y / 10.0;
    mag[2] = (float) z / 10.0;
    
}

void MAG3110::ReadXYZraw(int16_t * mag_raw)
{
    mag_raw[0] = readVal(MAG_OUT_X_MSB);
    mag_raw[1] = readVal(MAG_OUT_Y_MSB);
    mag_raw[2] = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::setCalibration(int minX, int maxX, int minY, int maxY )
{
    _avgX=(maxX+minX)/2;
    _avgY=(maxY+minY)/2;
}
*/




