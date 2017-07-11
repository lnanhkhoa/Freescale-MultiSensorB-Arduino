/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FXAS21002CQ_H
#define FXAS21002CQ_H


/**
 * FXAS21002CQ Slave Address
 */

#define FXAS21002CQ_SLAVE_ADDR 0x20       //(0x20)

// FXAS21002CQ internal register addresses
#define FXAS21002CQ_STATUS           0x00
#define FXAS21002CQ_OUT_X_MSB        0x01    
#define FXAS21002CQ_OUT_X_LSB        0x02
#define FXAS21002CQ_OUT_Y_MSB        0x03
#define FXAS21002CQ_OUT_Y_LSB        0x04
#define FXAS21002CQ_OUT_Z_MSB        0x05
#define FXAS21002CQ_OUT_Z_LSB        0x06
#define FXAS21002CQ_DR_STATUS        0x07
#define FXAS21002CQ_F_STATUS         0x08
#define FXAS21002CQ_F_EVENT          0x0A
#define FXAS21002CQ_INT_SRC_FLAG     0x0B
#define FXAS21002CQ_WHOAMI           0x0C   // Should return 0xD7
#define FXAS21002CQ_WHOAMI_VALUE     0xD7
#define FXAS21002CQ_CTRL_REG0        0x0D
#define FXAS21002CQ_RT_CFG           0x0E   
#define FXAS21002CQ_RT_SRC           0x0F
#define FXAS21002CQ_RT_THS           0x10
#define FXAS21002CQ_RT_COUNT         0x11
#define FXAS21002CQ_TEMP             0x12
#define FXAS21002CQ_CTRL_REG0        0x0D
#define FXAS21002CQ_CTRL_REG1        0x13
#define FXAS21002CQ_CTRL_REG2        0x14

typedef int PinName;


// Set initial input parameters
enum gyroFSR {
  GFS_1600DPS = 0,
  GFS_800DPS,
  GFS_400DPS,
  GFS_200DPS
};

enum gyroODR {
  GODR_200HZ = 0, // 200 Hz
  GODR_100HZ,
  GODR_50HZ,
  GODR_25HZ,
  GODR_12_5HZ,
  GODR_6_25HZ, // 6.25 Hz, etc.
  GODR_3_125HZ,
  GODR_1_5625HZ
};





/**
 * FXAS21002CQ gyroscope on I2C
 */
class FXAS21002CQ
{
public:
    /**
     * FXAS21002CQ constructor
     *
     * @param sda SDA pin
     * @param sdl SCL pin
     */
    FXAS21002CQ(PinName sda, PinName scl);
 

    void begin( void);
    
    uint8_t getWhoAmI(void);
    

    char WhoAmIValue();
    uint8_t status(void);

    /**
     * Get the Gyro values as floating point degrees / second
     *
     * @param floating point array where the results will be placed
     */
    void ReadXYZ(float * g);

    /**
     * Get the Gyro values as signed 16 bit value
     *
     * @param int16_t point array where the results will be placed
     */
    void ReadXYZraw(int16_t * destination);
   
    /**
     * Get the value of the WHO_AM_I register
     *
     * @returns DEVICE_ID value == 0xD1
     */
    float getGres();

    float readTemperature();

    int8_t readTempData();

    void calibrate(float * gBias);

    void Reset();

    void init();

    void Standby();

    void Ready();

    void Active();


    void RegRead( char reg, char * d, int len);
};

#endif
