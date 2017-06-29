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

#ifndef MMA8652_H
#define MMA8652_H

#include "mbed.h"


// MMA8652 Slave Address
#define MMA8652_SLAVE_ADDR 0x3A

// MMA8652 internal register addresses
#define MMA8652_STATUS 0x00
#define MMA8652_OUT_X_MSB 0x01
#define MMA8652_WHOAMI 0x0D
#define MMA8652_XYZ_DATA_CFG 0x0E
#define MMA8652_CTRL_REG1 0x2A
#define MMA8652_WHOAMI_VAL 0x4A

/**
 * MMA8652 Xtrinsic accelerometer on I2C
 */
class MMA8652
{
public:
    /**
     * MMA8652 constructor
     *
     * @param sda SDA pin
     * @param sdl SCL pin
     */
    MMA8652(PinName sda, PinName scl);

    /**
     * MMA8652 destructor
     */
    ~MMA8652();
    
    /**
     * Get XYZ axis acceleration in floating point G's
     *
     * @param res array where acceleration data will be stored
     */
    void ReadXYZ(float * a);

    /**
     * Get XYZ axis acceleration, signed 16 bit values
     *
     * @param res array where acceleration data will be stored
     */
    void ReadXYZraw(int16_t * d);
    
    /**
     * Get the value of the WHO_AM_I register
     *
     * @returns DEVICE_ID value == 0x3A
     */
    char getWhoAmI(void);

private:
    I2C _i2c;

    /**
     * Set the device in active mode
     */
    void begin( void);

    void RegRead( char reg, char * d, int len);

};

#endif
