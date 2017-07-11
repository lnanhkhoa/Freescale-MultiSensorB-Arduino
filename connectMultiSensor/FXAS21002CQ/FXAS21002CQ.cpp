

#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif



#ifndef WIRE_H
#define WIRE_H
#include "Wire.h"
#endif

#include "i2c.h"
#include "FXAS21002CQ.h"
 
I2C _i2c(0,0);


// Specify sensor full scale
uint8_t gyroFSR = GFS_1600DPS;
uint8_t gyroODR = GODR_200HZ;

float temperature;





FXAS21002CQ::FXAS21002CQ(PinName sda, PinName scl)
{ 
    _i2c = decltype(_i2c)(sda, scl);
    begin();
}
                                                        

void FXAS21002CQ::begin(void)
{

    _i2c.start();
}


uint8_t FXAS21002CQ::status(void){
    return _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_STATUS);
}

uint8_t FXAS21002CQ::getWhoAmI(void)
{
    uint8_t d = _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_WHOAMI);
    return(d);
}

char FXAS21002CQ::WhoAmIValue(){
    return (FXAS21002CQ_WHOAMI_VALUE);
}

void FXAS21002CQ::ReadXYZ(float * g)
{
    int16_t gyroCount[3];  // Stores the 12-bit signed value
    float gRes, gBias[3] = {0, 0, 0}; // scale resolutions per LSB for the sensors
    ReadXYZraw(gyroCount);  // Read the x/y/z adc values
    gRes = getGres();
    
    // Calculate the gyro value into actual degrees per second
    g[0] = (float)gyroCount[0]*gRes - gBias[0];  // get actual gyro value, this depends on scale being set
    g[1] = (float)gyroCount[1]*gRes - gBias[1];  
    g[2] = (float)gyroCount[2]*gRes - gBias[2];
}


void FXAS21002CQ::ReadXYZraw(int16_t * destination)
{
    uint8_t rawData[6];  // x/y/z accel register data stored here
    _i2c.readBytes(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
    destination[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 2; // signed 14-bit integers
    destination[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 2;
    destination[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 2;
}

///////////////////////////////////////////////////////////////////////////////
// Useful functions to access the FXAS21002CQ gyroscope
///////////////////////////////////////////////////////////////////////////////

float FXAS21002CQ::getGres() {
    float gRes;
    switch (gyroFSR)
    {
            // Possible gyro scales (and their register bit settings) are:
            // 200 DPS (11), 400 DPS (10), 800 DPS (01), and 1600 DPS  (00). 
            case GFS_1600DPS:
                                    gRes = 1600.0/8192.0;
                                    break;
            case GFS_800DPS:
                                    gRes = 800.0/8192.0;
                                    break;
            case GFS_400DPS:
                                    gRes = 400.0/8192.0;
                                    break;           
            case GFS_200DPS:
                                    gRes = 200.0/8192.0;
    }
    return gRes;
}


float FXAS21002CQ::readTemperature()
{
    int8_t tempCount;
    tempCount = readTempData();  // Read the x/y/z adc values
    return (float) tempCount; // Temperature in degrees Centigrade
}

int8_t FXAS21002CQ::readTempData()
{
        return (int8_t) _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_TEMP);  // Read the 8-bit 2's complement data register 
}


void FXAS21002CQ::calibrate(float * gBias)
{
        int32_t gyro_bias[3] = {0, 0, 0};
        uint16_t ii, fcount;
        int16_t temp[3];
        
        // Clear all interrupts by reading the data output and STATUS registers
        ReadXYZraw(temp);
        _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_STATUS);
        
        Standby();  // Must be in standby to change registers

        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, 0x08);   // select 50 Hz ODR
        fcount = 50;                                     // sample for 1 second
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG0, 0x03);   // select 200 deg/s full scale
        uint16_t gyrosensitivity = 41;                   // 40.96 LSB/deg/s

        Active();  // Set to active to start collecting data
         
        uint8_t rawData[6];  // x/y/z FIFO accel data stored here
        for(ii = 0; ii < fcount; ii++)   // construct count sums for each axis
        {
        _i2c.readBytes(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_OUT_X_MSB, 6, &rawData[0]);  // Read the FIFO data registers into data array
        temp[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 2;
        temp[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 2;
        temp[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 2;
        
        gyro_bias[0] += (int32_t) temp[0];
        gyro_bias[1] += (int32_t) temp[1];
        gyro_bias[2] += (int32_t) temp[2];
        
        delay(25); // wait for next data sample at 50 Hz rate
        }
 
        gyro_bias[0] /= (int32_t) fcount; // get average values
        gyro_bias[1] /= (int32_t) fcount;
        gyro_bias[2] /= (int32_t) fcount;
        
        gBias[0] = (float)gyro_bias[0]/(float) gyrosensitivity; // get average values
        gBias[1] = (float)gyro_bias[1]/(float) gyrosensitivity; // get average values
        gBias[2] = (float)gyro_bias[2]/(float) gyrosensitivity; // get average values

        Ready();  // Set to ready
}
        
        
// Set up sensor software reset
void FXAS21002CQ::Reset() 
{
                _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, 0x20); // set reset bit to 1 to assert software reset to zero at end of boot process
                delay(100);
                while(!(_i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_INT_SRC_FLAG) & 0x08))  { // wait for boot end flag to be set
                }
}



// Initialize the FXAS21002CQ registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FXAS21002CQQ
// Feel free to modify any values, these are settings that work well for me.
void FXAS21002CQ::init()
{
        Standby();  // Must be in standby to change registers

        // Set up the full scale range to 200, 400, 800, or 1600 deg/s.
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG0, gyroFSR);      // write FSR

        // Setup the 3 data rate bits, 4:2
        if(gyroODR < 8) { // data rate can only be 0 to 7
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, gyroODR << 2); 
        }

        // Disable FIFO, route FIFO and rate threshold interrupts to INT2, enable data ready interrupt, route to INT1
        // Active HIGH, push-pull output driver on interrupts
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG2,  0x0E);
        
        // Set up rate threshold detection; at max rate threshold = FSR; rate threshold = THS*FSR/128
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_RT_CFG, 0x07);         // enable rate threshold detection on all axes
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_RT_THS, 0x00 | 0x0D);  // unsigned 7-bit THS, set to one-tenth FSR; set clearing debounce counter
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_RT_COUNT, 0x04);       // set to 4 (can set up to 255)
                                
        Active();  // Set to active to start reading
}


// Sets the FXAS21002CQ to standby mode.
// It must be in standby to change most register settings
void FXAS21002CQ::Standby()
{
        byte c = _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1);
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
}

// Sets the FXAS21002CQ to active mode.
// Needs to be in this mode to output data
void FXAS21002CQ::Ready()
{
        byte c = _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1);
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
        _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, c |   0x01);   // Set bit 0 to 1, ready mode; no data acquisition yet
}

// Sets the FXAS21002CQ to active mode.
// Needs to be in this mode to output data
void FXAS21002CQ::Active()
{
        byte c = _i2c.readByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1);
 _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
 _i2c.writeByte(FXAS21002CQ_SLAVE_ADDR, FXAS21002CQ_CTRL_REG1, c |   0x02);   // Set bit 1 to 1, active mode; data acquisition enabled
}


void FXAS21002CQ::RegRead( char reg, char * d, int len)
{
    char cmd[1];
    cmd[0] = reg;
    char i2c_addr = FXAS21002CQ_SLAVE_ADDR;
    _i2c.write( i2c_addr, cmd, 1, true);
    _i2c.read ( i2c_addr, d, len);
}