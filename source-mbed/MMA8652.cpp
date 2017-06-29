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

#include "MMA8652.h"
#define UINT14_MAX        16383
  
MMA8652::MMA8652(PinName sda, PinName scl) : _i2c(sda, scl) {
 
    begin();
}


MMA8652::~MMA8652() { }

              
void MMA8652::RegRead( char reg, char * d, int len)
{
    char cmd[1];
    cmd[0] = reg;
char i2c_addr = MMA8652_SLAVE_ADDR;
_i2c.write( i2c_addr, cmd, 1, true);
_i2c.read ( i2c_addr, d, len);
}

void MMA8652::begin(void)
{
    char data[2];
    // write 0000 0000 = 0x00 to accelerometer control register 1 to place MMA8652 into
    // standby
    // [7-1] = 0000 000
    // [0]: active=0
    data[0] = MMA8652_CTRL_REG1;
    data[1] = 0x00;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);
    
    // write 0000 0001= 0x01 to XYZ_DATA_CFG register
    // [7]: reserved
    // [6]: reserved
    // [5]: reserved
    // [4]: hpf_out=0
    // [3]: reserved
    // [2]: reserved
    // [1-0]: fs=00 for accelerometer range of +/-2g range with 0.244mg/LSB
    data[0] = MMA8652_XYZ_DATA_CFG;
    data[1] = 0x00;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);

    // write 0000 1101 = 0x0D to accelerometer control register 1
    // [7-6]: aslp_rate=00
    // [5-3]: dr=100 for 50Hz data rate
    // [2]: 0
    // [1]: 0
    // [0]: active=1 to take the part out of standby and enable sampling
    data[0] = MMA8652_CTRL_REG1;
    data[1] = 0x21;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);
}

char  MMA8652::getWhoAmI(void)
{
    static char d;
    RegRead( /*MMA8652_WHOAMI*/ 0x0d, &d, 1);
    return(d);
}

void MMA8652::ReadXYZ(float * a)
{
    char d[7];
    int16_t t[6];

    RegRead( MMA8652_STATUS, d, 7);
    t[0] = ((d[1] * 256) + ((unsigned short) d[2]));
    t[1] = ((d[3] * 256) + ((unsigned short) d[4]));
    t[2] = ((d[5] * 256) + ((unsigned short) d[6]));

    a[0] = (float) t[0] / 16384.0;
    a[1] = (float) t[1] / 16384.0;
    a[2] = (float) t[2] / 16384.0;
  
}


void MMA8652::ReadXYZraw(int16_t * d)
{
char res[6];
int16_t acc;
    RegRead( MMA8652_OUT_X_MSB, res, 6);

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
  
   