

#ifndef MMA8652FC_H
#define MMA8652FC_H


// MMA8652 Slave Address
#define MMA8652_SLAVE_ADDR       0x1D

// MMA8652 internal register addresses
#define MMA8652_STATUS           0x00
#define MMA8652_F_STATUS         0x00
#define MMA8652_OUT_X_MSB        0x01    
#define MMA8652_OUT_X_LSB        0x02
#define MMA8652_OUT_Y_MSB        0x03
#define MMA8652_OUT_Y_LSB        0x04
#define MMA8652_OUT_Z_MSB        0x05
#define MMA8652_OUT_Z_LSB        0x06
#define MMA8652_F_SETUP          0x09
#define MMA8652_TRIG_CFG         0x0A
#define MMA8652_SYSMOD           0x0B
#define MMA8652_INT_SOURCE       0x0C
#define MMA8652_WHOAMI           0x0D   
#define MMA8652_XYZ_DATA_CFG     0x0E
#define MMA8652_HP_FILTER_CUTOFF 0x0F
#define MMA8652_PL_STATUS        0x10
#define MMA8652_PL_CFG           0x11
#define MMA8652_PL_COUNT         0x12
#define MMA8652_PL_BF_ZCOMP      0x13
#define MMA8652_P_L_THS_REG      0x14
#define MMA8652_FF_MT_CFG        0x15
#define MMA8652_FF_MT_SRC        0x16
#define MMA8652_FF_MT_THS        0x17
#define MMA8652_FF_MT_COUNT      0x18
#define MMA8652_TRANSIENT_CFG    0x1D
#define MMA8652_TRANSIENT_SRC    0x1E
#define MMA8652_TRANSIENT_THS    0x1F
#define MMA8652_TRANSIENT_COUNT  0x20
#define MMA8652_PULSE_CFG        0x21
#define MMA8652_PULSE_SRC        0x22
#define MMA8652_PULSE_THSX       0x23
#define MMA8652_PULSE_THSY       0x24
#define MMA8652_PULSE_THSZ       0x25
#define MMA8652_PULSE_TMLT       0x26
#define MMA8652_PULSE_LTCY       0x27
#define MMA8652_PULSE_WIND       0x28
#define MMA8652_ASLP_COUNT       0x29
#define MMA8652_CTRL_REG1        0x2A
#define MMA8652_CTRL_REG2        0x2B
#define MMA8652_CTRL_REG3        0x2C
#define MMA8652_CTRL_REG4        0x2D
#define MMA8652_CTRL_REG5        0x2E
#define MMA8652_OFF_X            0x2F
#define MMA8652_OFF_Y            0x30
#define MMA8652_OFF_Z            0x31
#define MMA8652_WHOAMI_VAL       0x4A


enum accelFSR {
    AFS_2g = 0,
    AFS_4g,
    AFS_8g
};

enum accelODR {
    AODR_800HZ = 0, // 200 Hz
    AODR_400HZ,
    AODR_200HZ,
    AODR_100HZ,
    AODR_50HZ,
    AODR_12_5HZ, // 12.5 Hz, etc.
    AODR_6_25HZ,
    AODR_1_56HZ
};

typedef int PinName;

/**
 * MMA8652 accelerometer on I2C
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
    
    void Reset(void);

    void init(void);

    uint8_t status(void);

    void readAccelData(int16_t * destination);

    void calibrate(void);

    void Offsets(void);

    void Standby(void);

    void Active(void);

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


    char WhoAmIValue(void);


    void begin(void);


    void config(void);

    float getAres(void);

    
private:
    /**
     * Set the device in active mode
     */

    void RegRead( char reg, char * d, int len);

};

#endif
