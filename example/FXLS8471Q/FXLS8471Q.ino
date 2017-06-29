#include <SPI.h>
#include <Arduino.h>
#define FXLS8471Q_STATUS 0x00  
#define FXLS8471Q_WHOAMI 0x0D  
#define FXLS8471Q_XYZ_DATA_CFG 0x0E  
#define FXLS8471Q_CTRL_REG1 0x2A
short XValue, YValue, ZValue;
short slaveSelectPin = 10;



byte FXLS8471Q_getWhoAmI(void)
{
    int d;
    d=SPIreadOneRegister(10,FXLS8471Q_WHOAMI);
    return((char) d);
}

byte SPIreadOneRegister(int slaveSelectPin, byte regAddress){
  byte regValue = 0;
  
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0B);  // read instruction
  SPI.transfer(regAddress);
  regValue = SPI.transfer(0x00);
  digitalWrite(slaveSelectPin, HIGH);

  return regValue;
}
void SPIwriteOneRegister(int slaveSelectPin, byte regAddress,byte regValue){
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(0x0A);  // write instruction
  SPI.transfer(regAddress);
  SPI.transfer(regValue);
  digitalWrite(slaveSelectPin, HIGH);
}

void FXLS8471Q_begin(short chipSelectPin) {  
    slaveSelectPin = chipSelectPin;  
    pinMode(slaveSelectPin, OUTPUT);  
    SPI.begin();  
    SPI.setClockDivider(SPI_CLOCK_DIV16);   // 16 MHz / 16 = 1 MHz  
    SPI.setDataMode(SPI_MODE0);    //CPHA = CPOL = 0    MODE = 0  
    SPI.setBitOrder(MSBFIRST);  
    delay(1000);  
     
    // reset sensor  
    SPIwriteOneRegister(10,0x2B, 0x40);                        // set RST in CTRL_REG2 register  
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

void setup(){  
  Serial.begin(9600);  
  FXLS8471Q_begin(10);                   // Setup SPI protocol, issue device soft reset  
}  
void loop(){

  // read all three axis in burst to ensure all measurements correspond to same sample time  
  //Serial.print(FXLS8471Q_getWhoAmI(),DEC);
  FXLS8471Q_readXYZData(XValue, YValue, ZValue);   
  Serial.print("XVALUE=");  
  Serial.print(XValue);      
  Serial.print("\tYVALUE=");  
  Serial.print(YValue);      
  Serial.print("\tZVALUE=");  
  Serial.print(ZValue);  
  Serial.println();       
  delay(1000);                // Arbitrary delay to make serial monitor easier to observe  
}  
