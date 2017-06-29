
#include "MAG3110.h"
#include "mbed.h"
#include "MotionSensor.h"

/******************************************************************************
 * Constructors
 ******************************************************************************/
MAG3110::MAG3110(PinName sda, PinName scl): m_i2c(sda, scl), 
    m_addr(0x1d)
{
    char cmd[2];

    cmd[0] = MAG_CTRL_REG2;
    cmd[1] = 0x80;
    m_i2c.write(m_addr, cmd, 2);

}

void MAG3110::enable(void) {
    uint8_t data[2];
    readRegs( MAG_CTRL_REG1, &data[1], 1);
    data[1] |= 0x01;
    data[0] = MAG_CTRL_REG1;
    writeRegs(data, 2);
}


void MAG3110::disable(void) {
    uint8_t data[2];
    readRegs( MAG_CTRL_REG1, &data[1], 1);
    data[1] &= 0xFE;
    data[0] = MAG_CTRL_REG1;
    writeRegs(data, 2);
}


void MAG3110::readRegs(int addr, uint8_t * data, int len)
{
    char cmd[1];

    cmd[0] = addr;
    m_i2c.write( m_addr, cmd, 1, true);
    m_i2c.read( m_addr, (char *) data, len);
    return;
}


void MAG3110::writeRegs(uint8_t * data, int len) {
    m_i2c.write(m_addr, (char *)data, len);
}


uint32_t MAG3110::whoAmI() {
    uint8_t who_am_i = 0;
    readRegs(MAG_WHOAMI, &who_am_i, 1);
    return (uint32_t) who_am_i;
}

uint32_t MAG3110::dataReady(void) {
    uint8_t stat = 0;
    readRegs(MAG_DR_STATUS, &stat, 1);
    return (uint32_t) stat;
}

uint32_t MAG3110::sampleRate(uint32_t f) {
    return(50); // for now sample rate is fixed at 50Hz
}


void MAG3110::getX(float * x) {
    *x = (float(getMagAxis(MAG_OUT_X_MSB)) * 0.1f);
}

void MAG3110::getY(float * y) {
    *y = (float(getMagAxis(MAG_OUT_Y_MSB)) * 0.1f);
}

void MAG3110::getZ(float * z) {
    *z = (float(getMagAxis(MAG_OUT_Z_MSB)) * 0.1f);
}

void MAG3110::getX(int16_t * d) {
    *d = getMagAxis(MAG_OUT_X_MSB);
}

void MAG3110::getY(int16_t * d) {
    *d = getMagAxis(MAG_OUT_Y_MSB);
}

void MAG3110::getZ(int16_t * d) {
    *d = getMagAxis(MAG_OUT_Z_MSB);
}

int16_t MAG3110::getMagAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 8) | res[1];

    return acc;
}


void MAG3110::getAxis(MotionSensorDataUnits &data) {
    int16_t t[3];
    uint8_t res[6];

    readRegs(MAG_OUT_X_MSB, res, 6);
    t[0] = (res[0] << 8) | res[1];
    t[1] = (res[2] << 8) | res[3];
    t[2] = (res[4] << 8) | res[5];
    data.x = ((float) t[0]) * 0.1f;
    data.y = ((float) t[1]) * 0.1f;
    data.z = ((float) t[2]) * 0.1f;
}


void MAG3110::getAxis(MotionSensorDataCounts &data) {

    uint8_t res[6];
    readRegs(MAG_OUT_X_MSB, res, 6);

    data.x = (res[0] << 8) | res[1];
    data.y = (res[2] << 8) | res[3];
    data.z = (res[4] << 8) | res[5];
}
/*

// read a register per, pass first reg value, reading 2 bytes increments register
// Reads MSB first then LSB
int MAG3110::readVal(char regAddr)
{
    char cmd[2];
    int16_t t;
    cmd[0] = regAddr;
    if(_i2c.write(m_addr, cmd, 1)) {
        printf("MAG3110 write error\r\n");
        _i2c.stop();
        _i2c.start();
        }

    cmd[0] = 0x00;
    cmd[1] = 0x00;
    _i2c.read(m_addr, cmd, 2);
    t = (cmd[0] * 256) + (unsigned short) cmd[1];
    return ((int) t); //concatenate the MSB and LSB
}


float MAG3110::getHeading()
{
    int xVal = readVal(MAG_OUT_X_MSB);
    int yVal = readVal(MAG_OUT_Y_MSB);
    return (atan2((double)(yVal - _avgY),(double)(xVal - _avgX)))*180/PI;
}

void MAG3110::getValues(int *xVal, int *yVal, int *zVal)
{
    *xVal = readVal(MAG_OUT_X_MSB);
    *yVal = readVal(MAG_OUT_Y_MSB);
    *zVal = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::ReadXYZ(float * mag)
{
    int x, y, z;
    x = readVal(MAG_OUT_X_MSB);
    y = readVal(MAG_OUT_Y_MSB);
    z = readVal(MAG_OUT_Z_MSB);
    mag[0] = (float) x / 10.0;
    mag[1] = (float) y / 10.0;
    mag[2] = (float) z / 10.0;
    
}

void MAG3110::ReadXYZraw(int16_t * mag_raw)
{
    mag_raw[0] = readVal(MAG_OUT_X_MSB);
    mag_raw[1] = readVal(MAG_OUT_Y_MSB);
    mag_raw[2] = readVal(MAG_OUT_Z_MSB);
}

void MAG3110::setCalibration(int minX, int maxX, int minY, int maxY )
{
    _avgX=(maxX+minX)/2;
    _avgY=(maxY+minY)/2;
}
*/




