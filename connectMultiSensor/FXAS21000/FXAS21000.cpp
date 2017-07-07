 #include "FXAS21000.h"
 
FXAS21000::FXAS21000(PinName sda, PinName scl) : _i2c(sda, scl) {
 
    begin();
}
              
void FXAS21000::RegRead( char reg, char * d, int len)
{
    char cmd[1];
    cmd[0] = reg;
char i2c_addr = FXAS21000_SLAVE_ADDR;
_i2c.write( i2c_addr, cmd, 1, true);
_i2c.read ( i2c_addr, d, len);
}

void FXAS21000::begin(void)
{
    char data[2];
    // write 0000 1000 = 0x08 to gyro control register 1 to place FXAS21000 into
    // standby
    // [7-1] = 0000 000
    // [0]: active=0
    data[0] = FXAS21000_CTRL_REG1;
    data[1] = 0x08;
    _i2c.write( FXAS21000_SLAVE_ADDR, data, 2);
    
    // write 0001 1011 to CRTL_REG0 register
    data[0] = FXAS21000_CTRL_REG0;
    data[1] = 0x1B;
    _i2c.write( FXAS21000_SLAVE_ADDR, data, 2);

    // write 0000 1001 to gyro control register 1
    data[0] = FXAS21000_CTRL_REG1;
    data[1] = 0x0A;
    _i2c.write( FXAS21000_SLAVE_ADDR, data, 2);
}

char FXAS21000::getWhoAmI(void)
{
    char d;
    RegRead( FXAS21000_WHOAMI, &d, 1);
    return(d);
}

void FXAS21000::ReadXYZ(float * a)
{
    char d[7];
    int16_t t[6];

    RegRead( FXAS21000_STATUS, d, 7);
    t[0] = ((d[1] * 256) + ((unsigned short) d[2]));
    t[1] = ((d[3] * 256) + ((unsigned short) d[4]));
    t[2] = ((d[5] * 256) + ((unsigned short) d[6]));
    //printf("%X\r\n", (int) d[0]);

    a[0] = (float) t[0] * 0.003125;
    a[1] = (float) t[1] * 0.003125;
    a[2] = (float) t[2] * 0.003125;
  
}

void FXAS21000::ReadXYZraw(int16_t * t)
{
    char d[7];

    RegRead(FXAS21000_STATUS, d, 7);
    t[0] = ((d[1] * 256) + ((unsigned short) d[2]));
    t[1] = ((d[3] * 256) + ((unsigned short) d[4]));
    t[2] = ((d[5] * 256) + ((unsigned short) d[6]));
}



///////////////////////////////////////////////////////////////////////////////
// Useful functions to access the FXAS21000 gyroscope
///////////////////////////////////////////////////////////////////////////////

void FXAS21000::getGres() {
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
}

void FXAS21000::readGyroData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  _i2c.readBytes(FXAS21000_ADDRESS, OUT_X_MSB, 6, &rawData[0]);  // Read the six raw data registers into data array
  destination[0] = ((int16_t) rawData[0] << 8 | rawData[1]) >> 2; // signed 14-bit integers
  destination[1] = ((int16_t) rawData[2] << 8 | rawData[3]) >> 2;
  destination[2] = ((int16_t) rawData[4] << 8 | rawData[5]) >> 2;
}

int8_t FXAS21000::readTempData()
{
  return (int8_t) _i2c.readByte(FXAS21000_ADDRESS, TEMP);  // Read the 8-bit 2's complement data register 
}


void FXAS21000::calibrate(float * gBias)
{
  int32_t gyro_bias[3] = {0, 0, 0};
  uint16_t ii, fcount;
  int16_t temp[3];
  
  // Clear all interrupts by reading the data output and STATUS registers
  readGyroData(temp);
  _i2c.readByte(FXAS21000_ADDRESS, STATUS);
  
  Standby();  // Must be in standby to change registers

  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, 0x08);   // select 50 Hz ODR
  fcount = 50;                                     // sample for 1 second
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG0, 0x03);   // select 200 deg/s full scale
  uint16_t gyrosensitivity = 41;                   // 40.96 LSB/deg/s

  Active();  // Set to active to start collecting data
   
  uint8_t rawData[6];  // x/y/z FIFO accel data stored here
  for(ii = 0; ii < fcount; ii++)   // construct count sums for each axis
  {
  _i2c.readBytes(FXAS21000_ADDRESS, OUT_X_MSB, 6, &rawData[0]);  // Read the FIFO data registers into data array
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
void FXAS21000::Reset() 
{
    _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, 0x20); // set reset bit to 1 to assert software reset to zero at end of boot process
    delay(100);
    while(!(_i2c.readByte(FXAS21000_ADDRESS, INT_SRC_FLAG) & 0x08))  { // wait for boot end flag to be set
    }
}



// Initialize the FXAS21000 registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FXAS21000Q
// Feel free to modify any values, these are settings that work well for me.
void FXAS21000::init()
{
  Standby();  // Must be in standby to change registers

  // Set up the full scale range to 200, 400, 800, or 1600 deg/s.
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG0, gyroFSR);      // write FSR

  // Setup the 3 data rate bits, 4:2
  if(gyroODR < 8) { // data rate can only be 0 to 7
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, gyroODR << 2); 
  }

  // Disable FIFO, route FIFO and rate threshold interrupts to INT2, enable data ready interrupt, route to INT1
  // Active HIGH, push-pull output driver on interrupts
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG2,  0x0E);
  
  // Set up rate threshold detection; at max rate threshold = FSR; rate threshold = THS*FSR/128
  _i2c.writeByte(FXAS21000_ADDRESS, RT_CFG, 0x07);         // enable rate threshold detection on all axes
  _i2c.writeByte(FXAS21000_ADDRESS, RT_THS, 0x00 | 0x0D);  // unsigned 7-bit THS, set to one-tenth FSR; set clearing debounce counter
  _i2c.writeByte(FXAS21000_ADDRESS, RT_COUNT, 0x04);       // set to 4 (can set up to 255)
        
  Active();  // Set to active to start reading
}


// Sets the FXAS21000 to standby mode.
// It must be in standby to change most register settings
void FXAS21000::Standby()
{
  byte c = _i2c.readByte(FXAS21000_ADDRESS, CTRL_REG1);
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
}

// Sets the FXAS21000 to active mode.
// Needs to be in this mode to output data
void FXAS21000::Ready()
{
  byte c = _i2c.readByte(FXAS21000_ADDRESS, CTRL_REG1);
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
  _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, c |   0x01);   // Set bit 0 to 1, ready mode; no data acquisition yet
}

// Sets the FXAS21000 to active mode.
// Needs to be in this mode to output data
void FXAS21000::Active()
{
  byte c = _i2c.readByte(FXAS21000_ADDRESS, CTRL_REG1);
 _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, c & ~(0x03));  // Clear bits 0 and 1; standby mode
 _i2c.writeByte(FXAS21000_ADDRESS, CTRL_REG1, c |   0x02);   // Set bit 1 to 1, active mode; data acquisition enabled
}