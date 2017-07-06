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

// #ifndef WIRE_H
// #define WIRE_H
// #include "Wire.h"
// #endif


#ifndef  SPI_H 
#define  SPI_H 
#include <SPI.h>
#endif

#include "FXLS8471Q.h"

/* import protocol for connect */
#include "spiConnect.h"

spi_arduino _spi;


FXLS8471Q::FXLS8471Q(PinName mosi, PinName miso, PinName scl, PinName cs)
{
    delay(0.1);
    _spi.setPin(mosi, miso, scl, cs);
}

void FXLS8471Q::RegWrite( int reg, int *d, int len)
{
    int ob[2];
    int c = 0;
    ob[1] = 0;
    ob[0] = reg + 128;
    _spi.cs(false);
    _spi.write(ob[0]);
    _spi.write(ob[1]);
    while(c < len) 
        _spi.write(d[c++]);
    _spi.cs(true);
}


void FXLS8471Q::RegRead( int reg, int *d, int len)
{
    int ob[2];
    int c = 0;
    ob[0] = reg;
    ob[1] = 0;
    _spi.cs(false);
    _spi.write(ob[0]);
    _spi.write(ob[1]);
    while(c < len) 
        d[c++] = _spi.read(0xff);
    _spi.cs(true);
}

char FXLS8471Q::getWhoAmI(void)
{
    static int d;
    RegRead( FXLS8471Q_WHOAMI, &d, 1);
    return((char) d);
}


void FXLS8471Q::begin(void)
{
    int databyte;
    _spi.begin();
    // write 0000 0000 = 0x00 to accelerometer control register 1 to place FXLS8471Q into
    // standby
    // [7-1] = 0000 000
    // [0]: active=0
    databyte = 0x00;
    RegWrite(FXLS8471Q_CTRL_REG1, &databyte, 1);

    // write 0000 0001= 0x01 to XYZ_DATA_CFG register
    // [7]: reserved
    // [6]: reserved
    // [5]: reserved
    // [4]: hpf_out=0
    // [3]: reserved
    // [2]: reserved
    // [1-0]: fs=00 for accelerometer range of +/-2g with 0.244mg/LSB
    databyte = 0x00;
    RegWrite(FXLS8471Q_XYZ_DATA_CFG, &databyte, 1);

    // write 0001 0101b = 0x15 to accelerometer control register 1
    // [7-6]: aslp_rate=00
    // [5-3]: dr=010 for 200Hz data rate
    // [2]: lnoise=1 for low noise mode
    // [1]: f_read=0 for normal 16 bit reads
    // [0]: active=1 to take the part out of standby and enable sampling
    databyte = 0x15;
    RegWrite(FXLS8471Q_CTRL_REG1, &databyte, 1);
}

void FXLS8471Q::ReadXYZ(float * fdata)
{
    int raw[7];
    int16_t ix, iy, iz;
    RegRead(FXLS8471Q_STATUS, raw, 7);

    ix = ((raw[1] * 256) + raw[2]);// / 4;
    iy = ((raw[3] * 256) + raw[4]);// / 4;
    iz = ((raw[5] * 256) + raw[6]);// / 4;
    fdata[0] = ((float) ix) / 16384.0;
    fdata[1] = ((float) iy) / 16384.0;
    fdata[2] = ((float) iz) / 16384.0;
}

void FXLS8471Q::ReadXYZraw(int16_t * d)
{
    int res[6];
    int16_t acc;
    RegRead(FXLS8471Q_OUT_X_MSB, res, 6);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;
    d[0] = acc;
    acc = (res[2] << 6) | (res[3] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;
    d[1] = acc;
    acc = (res[4] << 6) | (res[5] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;
    d[2] = acc;
}

/*                          
*   Function for Arduino    *
*/

// void FXLS8471Q::_spi_write(int16_t * d)
// {
//     SPI.transfer(d);
// }

// void FXLS8471Q::_spi_cs(bool digitalOut)
// {
//     static PinName slaveSelectPin;
//     if (digitalOut)
//         digitalWrite(slaveSelectPin, HIGH);
//     else
//         digitalWrite(slaveSelectPin, LOW);
// }