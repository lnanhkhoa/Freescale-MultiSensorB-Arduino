
#ifndef  SPI_H  
#include <SPI.h>
#endif
//#include <Arduino.h>

#define FXLS8471Q_STATUS 0x00  
#define FXLS8471Q_WHOAMI 0x0D  
#define FXLS8471Q_XYZ_DATA_CFG 0x0E  
#define FXLS8471Q_CTRL_REG1 0x2A

short XValue, YValue, ZValue;
short slaveSelectPin = 10;



byte FXLS8471Q_getWhoAmI(void
){
    int d;
    d=SPIreadOneRegister(slaveSelectPin, FXLS8471Q_WHOAMI);
    return((char) d);
}

byte SPIreadOneRegister(int slaveSelectPin, byte regAddress){
  byte regValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  //SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);
  SPI.transfer(0x00);
  regValue = SPI.transfer(0xff);
  digitalWrite(slaveSelectPin, HIGH);

  return regValue;
}

void RegRead(int regAddress, int *d, int len){

  digitalWrite(10, LOW);
  int c = 0;
  //SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);
  SPI.transfer(0x00);
  while(c < len) d[c++] = SPI.transfer(0xff);
  digitalWrite(10, HIGH);

};

void RegWrite( int reg, int *d, int len)
{
    int ob[2];
    int c = 0;
    ob[1] = 0;
    ob[0] = reg + 128;
    digitalWrite(slaveSelectPin, LOW);
    // _spi.write(ob[0]);
    // _spi.write(ob[1]);
    SPI.transfer(ob[0]);
    SPI.transfer(ob[1]);
    while(c < len)
      SPI.transfer(d[c++]);
    digitalWrite(slaveSelectPin, HIGH);
}

void SPIwriteOneRegister(int slaveSelectPin, byte regAddress, byte regValue){
  digitalWrite(slaveSelectPin, LOW);
  //SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);
  SPI.transfer(regValue);
  digitalWrite(slaveSelectPin, HIGH);
}

void FXLS8471Q___begin(short chipSelectPin) {  
    // reset sensor  
    SPIwriteOneRegister(10,0x2B, 0x40);                        // set RST in CTRL_REG2 register  
    // RegWrite(FXLS8471Q_CTRL_REG1, &databyte, 1);
    delay(100);
    // set range to 4g  
    SPIwriteOneRegister(10,FXLS8471Q_XYZ_DATA_CFG, 0x01); // Write to FXLS8471Q_XYZ_DATA_CFG  
     
    // set data rate to 200 Hz and enable sampling  
    SPIwriteOneRegister(10,FXLS8471Q_CTRL_REG1, 0x15); //  
}
void FXLS8471Q_readXYZData(short &XData, short &YData, short &ZData){  
      
      digitalWrite(slaveSelectPin, LOW);  
      delay(10);  
      SPI.transfer(0x01);  // send read instruction (RW=0, address=0x01)  
      SPI.transfer(0x00);  // send second byte  
       
      XData = SPI.transfer(0x00) << 8;      //Most significant bit(MSB) first, then LSB  
      XData = XData + SPI.transfer(0x00);  
      XData = XData >> 2;  
  
      YData = SPI.transfer(0x00) << 8;  
      YData = YData + SPI.transfer(0x00);  
      YData = YData >> 2;  
       
      ZData = SPI.transfer(0x00) << 8;  
      ZData = ZData + SPI.transfer(0x00);  
      ZData = ZData >> 2;  
      digitalWrite(slaveSelectPin, HIGH);  
}

void FXLS8471Q_begin(void)
{
    int databyte;
    // write 0000 0000 = 0x00 to accelerometer control register 1 to place FXLS8471Q into
    // standby
    // [7-1] = 0000 000
    // [0]: active=0
    databyte = 0x00;
    RegWrite(FXLS8471Q_CTRL_REG1, &databyte, 1);

    // write 0000 0001 = 0x01 to XYZ_DATA_CFG register
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
    // [1]: f_read=0 for normal 16-bit reads
    // [0]: active=1 to take the part out of standby and enable sampling
    databyte = 0x15;
    RegWrite(FXLS8471Q_CTRL_REG1, &databyte, 1);
}

void FXLS8471Q_ReadXYZ(float * fdata)
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

void setup(){  
  Serial.begin(38400);
  // slaveSelectPin = chipSelectPin;  
  pinMode(slaveSelectPin, OUTPUT);  
  SPI.begin();  
  SPI.setClockDivider(SPI_CLOCK_DIV16);   // 16 MHz / 16 = 1 MHz  
  SPI.setDataMode(SPI_MODE0);    //CPHA = CPOL = 0    MODE = 0  
  SPI.setBitOrder(MSBFIRST);  
  delay(1000);
  FXLS8471Q_begin();                   // Setup SPI protocol, issue device soft reset  
}  
void loop(){

  // read all three axis in burst to ensure all measurements correspond to same sample time  
  //Serial.print(FXLS8471Q_getWhoAmI(),HEX);
  // int ob[2];
  // ob[1] = 0;
  // ob[0] = 0x2A + 0x80;
  // Serial.print(ob[0],HEX);
  // Serial.print(",");
  // Serial.println(ob[1],HEX);
  // FXLS8471Q_readXYZData(XValue, YValue, ZValue);   
  // Serial.print("XVALUE=");  
  // Serial.print(XValue);      
  // Serial.print("\tYVALUE=");  
  // Serial.print(YValue);      
  // Serial.print("\tZVALUE=");  
  // Serial.print(ZValue);  
  // Serial.println();       
  //delay(1000);                // Arbitrary delay to make serial monitor easier to observe  

  float fdata[3];
  FXLS8471Q_ReadXYZ(fdata);
  Serial.print(fdata[0]);
  Serial.print(",");
  Serial.print(fdata[1]);
  Serial.print(",");
  Serial.println(fdata[2]);

  // while(1){
  //     // statement
  // }
}  
