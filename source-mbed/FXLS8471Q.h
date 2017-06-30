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


#ifndef FXLS8471Q_H
#define FXLS8471Q_H

#include "mbed.h"

// FXLS8471Q internal register addresses

#define FXLS8471Q_STATUS 0x00
#define FXLS8471Q_OUT_X_MSB 0x01
#define FXLS8471Q_WHOAMI 0x0D
#define FXLS8471Q_XYZ_DATA_CFG 0x0E
#define FXLS8471Q_CTRL_REG1 0x2A
#define FXLS8471Q_WHOAMI_VAL 0x6A

// MMA8652 Slave Address (For I2C)
//#define FXLS8471Q_ADDRESS 0x1E

/**
 * FXLS8471Q Xtrinsic accelerometer on SPI
 */
class FXLS8471Q
{
public:
    /**
    * FXLS8471Q constructor
    *
    * @param mosi MOSI pin
    * @param miso MISO pin
    * @param scl  SCL  pin
    * @param cs   Device Chip Select pin
    */
    FXLS8471Q(PinName mosi, PinName miso, PinName scl, PinName cs);

    /**
     * Get XYZ axis acceleration in G's as floating point
     *
     * @param res array where acceleration data will be stored
     */
    void ReadXYZ(float * a);

    /**
     * Get XYZ axis acceleration as signed 16 bit values
     *
     * @param res array where acceleration data will be stored
     */
    void ReadXYZraw(int16_t * d);

    /**
     * Get the value of the WHO_AM_I register
     *
     * @returns WHO_AM_I value
     */
    char getWhoAmI(void);

private:
    SPI _spi;

    DigitalOut _spi_cs;

    /**
     * Set the device in active mode
     */
    void begin( void);

    void RegWrite( int reg, int * d, int len);

    void RegRead( int reg, int * d, int len);
};

#endif
