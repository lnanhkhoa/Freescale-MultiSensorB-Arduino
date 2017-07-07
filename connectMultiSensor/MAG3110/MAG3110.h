/*
 * MAG3110 Sensor Library for mbed
 * TODO: Add proper header
 */

#ifndef MAG3110_H
#define MAG3110_H

#define PI 3.14159265359

#define MAG3110_ADDRESS 0x0E

// define registers
#define MAG_DR_STATUS 0x00
#define MAG_OUT_X_MSB 0x01
#define MAG_OUT_X_LSB 0x02
#define MAG_OUT_Y_MSB 0x03
#define MAG_OUT_Y_LSB 0x04
#define MAG_OUT_Z_MSB 0x05
#define MAG_OUT_Z_LSB 0x06
#define MAG_WHOAMI    0x07
#define MAG_SYSMOD    0x08
#define MAG_OFF_X_MSB 0x09
#define MAG_OFF_X_LSB 0x0A
#define MAG_OFF_Y_MSB 0x0B
#define MAG_OFF_Y_LSB 0x0C
#define MAG_OFF_Z_MSB 0x0D
#define MAG_OFF_Z_LSB 0x0E
#define MAG_DIE_TEMP  0x0F
#define MAG_CTRL_REG1 0x10
#define MAG_CTRL_REG2 0x11

// what should WHO_AM_I return?
#define MAG3110_WHO_AM_I_VALUE 0xC4


// Fields in registers
// CTRL_REG1: dr2,dr1,dr0  os1,os0  fr tm ac

// Sampling rate from 80Hz down to 0.625Hz
#define MAG_3110_SAMPLE80 0
#define MAG_3110_SAMPLE40 0x20
#define MAG_3110_SAMPLE20 0x40
#define MAG_3110_SAMPLE10 0x60
#define MAG_3110_SAMPLE5 0x80
#define MAG_3110_SAMPLE2_5 0xA0
#define MAG_3110_SAMPLE1_25 0xC0
#define MAG_3110_SAMPLE0_625 0xE0

// How many samples to average (lowers data rate)
#define MAG_3110_OVERSAMPLE1 0
#define MAG_3110_OVERSAMPLE2 0x08
#define MAG_3110_OVERSAMPLE3 0x10
#define MAG_3110_OVERSAMPLE4 0x18

// read only 1 byte per axis
#define MAG_3110_FASTREAD 0x04
// do one measurement (even if in standby mode)
#define MAG_3110_TRIGGER 0x02
// put in active mode
#define MAG_3110_ACTIVE 0x01

// CTRL_REG2: AUTO_MRST_EN  _ RAW MAG_RST _ _ _ _ _
// reset sensor after each reading
#define MAG_3110_AUTO_MRST_EN 0x80
// don't subtract user offsets
#define MAG_3110_RAW 0x20
// reset magnetic sensor after too-large field
#define MAG_3110_MAG_RST 0x10

// DR_STATUS Register ZYXOW ZOW YOW XOW ZYXDR ZDR YDR XDR
#define MAG_3110_ZYXDR  0x08

typedef int PinName;


enum magODR {
  mODR_80Hz_16os = 0,  // 80 Hz data output rate with 16 times oversampling (1280 Hz is max ADC rate)
  mODR_40Hz_32os,
  mODR_20Hz_64os,
  mODR_10Hz_128os,
  mODR_40Hz_16os,  
  mODR_20Hz_32os,
  mODR_10Hz_64os,
  mODR_5Hz_128os,
  mODR_20Hz_16os,  
  mODR_10Hz_32os,
  mODR_5Hz_64os,
  mODR_2_5Hz_128os,
  mODR_10Hz_16os,  
  mODR_5Hz_32os,
  mODR_2_5Hz_64os,
  mODR_1_25Hz_128os,
  mODR_5Hz_16os,  
  mODR_2_5Hz_32os,
  mODR_1_25Hz_64os,
  mODR_0_63Hz_128os,
  mODR_2_5Hz_16os,  
  mODR_1_25Hz_32os,
  mODR_0_63Hz_64os,
  mODR_0_31Hz_128os,
  mODR_1_25Hz_16os,  
  mODR_0_63Hz_32os,
  mODR_0_31Hz_64os,
  mODR_0_16Hz_128os,
  mODR_0_63Hz_16os,  
  mODR_0_31Hz_32os,
  mODR_0_11Hz_64os,
  mODR_0_08Hz_128os
};


/**
 * MAG3110 Class to read X/Y/Z data from the magentometer
 *
 */
class MAG3110 
{
public:
    /**
     * Main constructor
     * @param sda SDA pin
     * @param sdl SCL pin
     * @param addr addr of the I2C peripherfal
     */
    MAG3110(PinName sda, PinName scl);

    ~MAG3110();

    void begin(void);
    
    void Reset(void);

    uint8_t status(void);

    uint8_t getWhoAmI(void);

    uint32_t dataReady(void);

    void ReadXYZraw(int16_t * destination);

    void ReadXYZ(float * a);

    int8_t readTempData();
      
    // Set up sensor software reset

    // Allow user compensation of acceleration errors
    void Offsets();

    // Initialize the MAG3110 registers 
    // See the many application notes for more info on setting all of these registers:
    // http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MAG3110
    // Feel free to modify any values, these are settings that work well for me.
    void init();

    // Sets the MAG3110 to standby mode.
    // It must be in standby to change most register settings
    void Standby();

    // Sets the MAG3110 to active mode.
    // Needs to be in this mode to output data
    void Active();

    char WhoAmIValue(void);

    void writeRegs(uint8_t * data, int len);
    void readRegs(int cmdl, uint8_t * data, int len);
};
#endif


