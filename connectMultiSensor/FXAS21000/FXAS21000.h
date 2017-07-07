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

#ifndef FXAS21000_H
#define FXAS21000_H


/**
 * FXAS21000 Slave Address
 */

#define FXAS21000_SLAVE_ADDR 0x40       //(0x20)

// FXAS21000 internal register addresses
#define FXAS21000_STATUS           0x00
#define FXAS21000_OUT_X_MSB        0x01    
#define FXAS21000_OUT_X_LSB        0x02
#define FXAS21000_OUT_Y_MSB        0x03
#define FXAS21000_OUT_Y_LSB        0x04
#define FXAS21000_OUT_Z_MSB        0x05
#define FXAS21000_OUT_Z_LSB        0x06
#define FXAS21000_DR_STATUS        0x07
#define FXAS21000_F_STATUS         0x08
#define FXAS21000_F_EVENT          0x0A
#define FXAS21000_INT_SRC_FLAG     0x0B
#define FXAS21000_WHOAMI         0x0C   // Should return 0xD1
#define FXAS21000_CTRL_REG0        0x0D
#define FXAS21000_RT_CFG           0x0E   
#define FXAS21000_RT_SRC           0x0F
#define FXAS21000_RT_THS           0x10
#define FXAS21000_RT_COUNT         0x11
#define FXAS21000_TEMP             0x12
#define FXAS21000_CTRL_REG0        0x0D
#define FXAS21000_CTRL_REG1        0x13
#define FXAS21000_CTRL_REG2        0x14



/**
 * FXAS21000 gyroscope on I2C
 */
class FXAS21000
{
public:
    /**
     * FXAS21000 constructor
     *
     * @param sda SDA pin
     * @param sdl SCL pin
     */
    FXAS21000(PinName sda, PinName scl);
 
    /**
     * Get the Gyro values as floating point degrees / second
     *
     * @param floating point array where the results will be placed
     */
    void ReadXYZ(float * a);

    /**
     * Get the Gyro values as signed 16 bit value
     *
     * @param int16_t point array where the results will be placed
     */
    void ReadXYZraw(int16_t * t);
   
    /**
     * Get the value of the WHO_AM_I register
     *
     * @returns DEVICE_ID value == 0xD1
     */
    char getWhoAmI(void);
    

    void begin( void);
    
    
    void getGres();

    void readGyroData(int16_t * destination);

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
