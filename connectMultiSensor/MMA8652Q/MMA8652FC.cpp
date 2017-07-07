
#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif



#ifndef WIRE_H
#define WIRE_H
#include "Wire.h"
#endif


#include "MMA8652FC.h"
#include "i2c.h"

#define UINT14_MAX        16383

// Specify sensor full scale
uint8_t accelFSR = AFS_2g;
uint8_t accelODR = AODR_200HZ;
float aRes;


  
I2C _i2c(0, 0);

MMA8652::MMA8652(PinName sda, PinName scl)
{
    _i2c = decltype(_i2c)(sda, scl);
}


MMA8652::~MMA8652() {

}


// Set up sensor software reset
void MMA8652::Reset() 
{    
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, 0x40); // set reset bit to 1 to assert software reset to zero at end of boot process
}

void MMA8652::readAccelData(int16_t * destination)
{
    uint8_t rawData[6];  // x/y/z accel register data stored here
    _i2c.readBytes(MMA8652_SLAVE_ADDR, MMA8652_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
    destination[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 4;
    destination[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 4;
    destination[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 4;
}



void MMA8652::calibrate(void)
{
    int32_t accel_bias[3] = {0, 0, 0};
    uint16_t ii, fcount;
    int16_t temp[3];
    
    // Clear all interrupts by reading the data output and MMA8652_F_STATUS registers
    readAccelData(temp);
    uint8_t tem = _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_F_STATUS);
    
    Standby();  // Must be in standby to change registers

    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, 0x01);      // select 100 Hz ODR
    fcount = 100;                                     // sample for 1 second
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_XYZ_DATA_CFG, 0x00);   // select 2 g full scale
    uint16_t accelsensitivity = 1024;                 // 1024 LSB/g

    Active();  // Set to active to start collecting data
     
    uint8_t rawData[6];  // x/y/z FIFO accel data stored here
    for(ii = 0; ii < fcount; ii++)   // construct count sums for each axis
    {
    _i2c.readBytes(MMA8652_SLAVE_ADDR, MMA8652_OUT_X_MSB, 6, &rawData[0]);  // Read the FIFO data registers into data array
    temp[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 4;
    temp[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 4;
    temp[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 4;
    
    accel_bias[0] += (int32_t) temp[0];
    accel_bias[1] += (int32_t) temp[1];
    accel_bias[2] += (int32_t) temp[2];
    
    delay(15);  // wait for a new data reading (100 Hz)
    // for (int i = 0; i < 15000; ++i);

    }
 
    accel_bias[0] /= (int32_t) fcount; // get average values
    accel_bias[1] /= (int32_t) fcount;
    accel_bias[2] /= (int32_t) fcount;
    
    if(accel_bias[2] > 0L) {
        accel_bias[2] -= (int32_t) accelsensitivity;
    }  // Remove gravity from the z-axis accelerometer bias calculation
    else {
        accel_bias[2] += (int32_t) accelsensitivity;
    }

    rawData[0] = (-accel_bias[0]/2) & 0xFF; // get average values
    rawData[1] = (-accel_bias[1]/2) & 0xFF; // get average values
    rawData[2] = (-accel_bias[2]/2) & 0xFF; // get average values
    
    Standby();  // Must be in standby to change registers
    
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_X, rawData[0]); // X-axis compensation  
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_Y, rawData[1]); // Y-axis compensation  
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_Z, rawData[2]); // z-axis compensation 

    Active();  // Set to active to start reading
} 

// Allow user compensation of acceleration errors
void MMA8652::Offsets(void)
{
     Standby();  // Must be in standby to change registers
     
     // Factory settings are pretty good; the settings below produce 1 mg error or less at 2 g full scale! For the device at rest on my table 
     // these values partially compensate for the slope of the table and the slope of the sensor in my breadboard. It is a pretty stable setup!
     // For negative values use 2's complement, i.e., -2 mg = 0xFF, etc.
     _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_X, 0x00); // X-axis compensation; this is 0 mg
     _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_Y, 0x00); // Y-axis compensation; this is 0 mg
     _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_OFF_Z, 0x00); // z-axis compensation; this is  0 mg adjustment
     
     Active();  // Set to active to start reading
}

// Initialize the MMA8652 registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8652Q
// Feel free to modify any values, these are settings that work well for me.
void MMA8652::init(void)
{
    Standby();  // Must be in standby to change registers

    // Set up the full scale range to 2, 4, or 8g.
        _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_XYZ_DATA_CFG, accelFSR);  

     // First clear MMA8652_CTRL_REG1
        _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, 0x00);
     // Setup the 3 data rate bits, from 0 to 7
        _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1) & ~(0x38));
        if (accelODR <= 7)
            _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1) | (accelODR << 3));      
        // set resolution
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) & ~(0x03)); // clear bits 0 and 1
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) |  (0x02)); // select normal(00) or high resolution (10) mode
        
    // These settings have to do with setting up the sleep mode and should probably be broken up into a separate function
    // set Auto-WAKE sample frequency when the device is in sleep mode

         _i2c.writeByte(MMA8652_SLAVE_ADDR, 0x29, 0x40 ); // sleep after ~36 seconds of inactivity at 6.25 Hz ODR

         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1) & ~(0xC0)); // clear bits 7 and 8
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1) |  (0xC0)); // select 1.56 Hz sleep mode sample frequency for low power

    // set sleep power mode scheme
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) & ~(0x18)); // clear bits 3 and 4
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) |  (0x18)); // select low power mode
         
    // Enable auto SLEEP
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) & ~(0x04)); // clear bit 2
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG2) |  (0x04)); // enable auto sleep mode

    // set sleep mode interrupt scheme
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3) & ~(0x3C)); // clear bits 3, 4, 5, and 6
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3) |  (0x3C)); // select wake on transient, orientation change, pulse, or freefall/motion detect
         
     // Enable Auto-SLEEP/WAKE interrupt
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4) & ~(0x80)); // clear bit 7
         _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4) |  (0x80)); // select  Auto-SLEEP/WAKE interrupt enable
     
    // Set up portrait/landscape registers - 4 steps:
    // 1. Enable P/L
    // 2. Set the back/front angle trigger points (z-lock)
    // 3. Set the threshold/hysteresis angle
    // 4. Set the debouce rate
    // For more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4068.pdf
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PL_CFG, 0x40);        // 1. Enable P/L
 // _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PL_BF_ZCOMP, 0x44); // 2. 29deg z-lock (don't think this register is actually writable)
 // _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_P_L_THS_REG, 0x84); // 3. 45deg thresh, 14deg hyst (don't think this register is writable either)
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PL_COUNT, 0x50);      // 4. debounce counter at 100ms (at 800 hz)

    /* Set up single and double tap - 5 steps:
     1. Set up single and/or double tap detection on each axis individually.
     2. Set the threshold - minimum required acceleration to cause a tap.
     3. Set the time limit - the maximum time that a tap can be above the threshold
     4. Set the pulse latency - the minimum required time between one pulse and the next
     5. Set the second pulse window - maximum allowed time between end of latency and start of second pulse
     for more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4072.pdf */
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_CFG, 0x7F);  // 1. enable single/double taps on all axes
    // _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_CFS, 0x55);  // 1. single taps only on all axes
    // _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_CFS, 0x6A);  // 1. double taps only on all axes
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_THSX, 0x04);  // 2. x thresh at 0.25g, multiply the value by 0.0625g/LSB to get the threshold
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_THSY, 0x04);  // 2. y thresh at 0.25g, multiply the value by 0.0625g/LSB to get the threshold
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_THSZ, 0x04);  // 2. z thresh at 0.25g, multiply the value by 0.0625g/LSB to get the threshold
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_TMLT, 0x30);  // 3. 2.55s time limit at 100Hz odr, this is very dependent on data rate, see the app note
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_LTCY, 0xA0);  // 4. 5.1s 100Hz odr between taps min, this also depends on the data rate
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_PULSE_WIND, 0xFF);  // 5. 10.2s (max value)  at 100 Hz between taps max

    // Set up motion detection
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_FF_MT_CFG, 0x58); // Set motion flag on x and y axes
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_FF_MT_THS, 0x84); // Clear debounce counter when condition no longer obtains, set threshold to 0.25 g
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_FF_MT_COUNT, 0x8); // Set debounce to 0.08 s at 100 Hz

    // Set up interrupt 1 and 2
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3) & ~(0x02)); // clear bits 0, 1 
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG3) |  (0x02)); // select ACTIVE HIGH, push-pull interrupts
         
 // _i2c.writeByte(MMA8652_SLAVE_ADDR, 0x2C, 0x02);  // Active high, push-pull interrupts

    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4) & ~(0x1D)); // clear bits 0, 3, and 4
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4, _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG4) |  (0x1D)); // DRDY, Freefall/Motion, P/L and tap ints enabled
     
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG5, 0x01);  // DRDY on INT1, P/L and taps on INT2

    Active();  // Set to active to start reading
}

uint8_t MMA8652::status(void){
    return _i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_STATUS);
}

// Sets the MMA8652 to standby mode.
// It must be in standby to change most register settings
void MMA8652::Standby(void)
{
    uint8_t c = _i2c.readByte(MMA8652_SLAVE_ADDR, 0x2A);
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, c & ~(0x01));
}

// Sets the MMA8652 to active mode.
// Needs to be in this mode to output data
void MMA8652::Active(void)
{
    uint8_t c = _i2c.readByte(MMA8652_SLAVE_ADDR, 0x2A);
    _i2c.writeByte(MMA8652_SLAVE_ADDR, MMA8652_CTRL_REG1, c | 0x01);
}


void MMA8652::RegRead( char reg, char * d, int len)
{
    
    char i2c_addr = MMA8652_SLAVE_ADDR;
    _i2c.write( i2c_addr, reg, 1, true);
    _i2c.read ( i2c_addr, d, len);
}

void MMA8652::begin(void)
{
	//Start I2C ( )
    _i2c.start();
}

void MMA8652::config(void)
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

float MMA8652::getAres(void) {
    
    switch (accelFSR)
    {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11). 
    // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
        case AFS_2g:
                    aRes = 2.0/2048.0;
                    break;
        case AFS_4g:
                    aRes = 4.0/2048.0;
                    break;
        case AFS_8g:
                    aRes = 8.0/2048.0;
                    break;

    }
    return aRes;
}

char  MMA8652::getWhoAmI(void)
{
    static char d;
    RegRead( /*MMA8652_WHOAMI*/ 0x0D, &d, 1);
    return(d);
}

char MMA8652::WhoAmIValue(void){
    return(MMA8652_WHOAMI_VAL);
}


void MMA8652::ReadXYZ(float * a)
{
    int16_t accelCount[3];
    if(_i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_STATUS) & 0x08)  // When this bit set, all axes have new data
    {
        ReadXYZraw(accelCount);
        getAres();
        a[0] = (float)accelCount[0]*aRes;
        a[1] = (float)accelCount[1]*aRes;
        a[2] = (float)accelCount[2]*aRes;  
    }
}


void MMA8652::ReadXYZraw(int16_t * destination)
{
    if(_i2c.readByte(MMA8652_SLAVE_ADDR, MMA8652_STATUS) & 0x08)  // When this bit set, all axes have new data
    {
    	uint8_t rawData[6];  // x/y/z accel register data stored here
        _i2c.readBytes(MMA8652_SLAVE_ADDR, MMA8652_OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
        destination[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 4;
        destination[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 4;
        destination[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 4;
    }
}
  
   