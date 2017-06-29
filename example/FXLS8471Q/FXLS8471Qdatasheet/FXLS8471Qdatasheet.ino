//#include "FXLS8471Q.h"
#include "Wire.h"
#include <Math.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_PCD8544.h>

// FXLS8471Q internal register addresses

#define FXLS8471Q_STATUS 0x00
#define FXLS8471Q_WHOAMI 0x0D
#define FXLS8471Q_XYZ_DATA_CFG 0x0E
#define FXLS8471Q_CTRL_REG1 0x2A
#define FXLS8471Q_WHOAMI_VAL 0xC7
#define FXLS8471Q_READ_LEN 7
#define FXLS8471Q_OUT_X_MSB 0x01
#define FXLS8471Q_ADDRESS 0x1E

#define I2C_ERROR 0
#define I2C_OK 1

int ledPin = 13;


typedef struct
{
int16_t x;
int16_t y;
int16_t z;
} SRAWDATA;

SRAWDATA Accel;



void setup()
{
  Wire.begin();
  Serial.begin(38400);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  // Read the WHO_AM_I register, this is a good test of communication
  byte c = readByte(FXLS8471Q_ADDRESS, FXLS8471Q_WHOAMI);  // Read WHO_AM_I register
  Serial.print(c,HEX);
  if (c == FXLS8471Q_WHOAMI_VAL) // WHO_AM_I should always be 0x4A
  {  
    int databyte;
	
    databyte = 0x00;
    writeByte(FXLS8471Q_ADDRESS, FXLS8471Q_CTRL_REG1, databyte);

    databyte = 0x00;
    writeByte(FXLS8471Q_ADDRESS, FXLS8471Q_XYZ_DATA_CFG, databyte);

    databyte = 0x15;
    writeByte(FXLS8471Q_ADDRESS, FXLS8471Q_CTRL_REG1, databyte);
	
    // Serial.println("FXLS8471Q is online...");
  }
  else
  {
    Serial.print("Could not connect to FXLS8471Q: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }
}
void loop()
{  
    int16_t ix, iy, iz;
	float fdata[2],accel_x,accel_y,accel_z;
	
	if(ReadAccel(&Accel)){
		ix=Accel.x;
		iy=Accel.y;
		iz=Accel.z;
		
		accel_x = ((float) ix) / 16384.0;
		accel_y = ((float) iy) / 16384.0;
		accel_z = ((float) iz) / 16384.0;
		
		float RADIANS_TO_DEGREES = 180/3.14159;
		//float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
		float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2))) *
		RADIANS_TO_DEGREES ;
		float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2))) *
		RADIANS_TO_DEGREES;
		float accel_angle_z = atan(sqrt(pow(accel_x,2) + pow(accel_y,2))/accel_z) *
		RADIANS_TO_DEGREES;
//		float accel_angle_z = 0;
		
		
		Serial.print(F("DEL:"));
		Serial.print(27, DEC);
		Serial.print(F("#FIL:"));
		Serial.print(accel_angle_x,2);
		Serial.print(F(","));
		Serial.print(accel_angle_y,2);
		Serial.print(F(","));
		Serial.print(accel_angle_z,2);
		Serial.println(F(""));
		delay(5);
	}
	else
		while(1);
}




// function configures FXLS8471Q accelerometer sensor
static int FXLS8471Q_start(){
	uint8_t databyte;
	// write 0000 0000 = 0x00 to accelerometer control register 1 to place FXLS8471Q into
	// standby
	// [7-1] = 0000 000
	// [0]: active=0
	// databyte = 0x00;
	// if (s_i2c_write_regs(aFP, FXLS8471Q_SLAVE_ADDR, FXLS8471Q_CTRL_REG1, &databyte, (uint8_t) 1) != 1){
		// return (I2C_ERROR);
	// }
	// write 0000 0001= 0x01 to XYZ_DATA_CFG register
	// [7]: reserved
	// [6]: reserved
	// [5]: reserved
	// [4]: hpf_out=0
	// [3]: reserved
	// [2]: reserved
	// [1-0]: fs=01 for accelerometer range of +/-4g with 0.488mg/LSB
	// databyte = 0x01;
	// if (s_i2c_write_regs(aFP, FXLS8471Q_SLAVE_ADDR, FXLS8471Q_XYZ_DATA_CFG, &databyte, (uint8_t) 1) != 1){
		// return (I2C_ERROR);
	// }
	// write 0001 0101b = 0x15 to accelerometer control register 1
	// [7-6]: aslp_rate=00
	// [5-3]: dr=010 for 200Hz data rate 
	// [2]: lnoise=1 for low noise mode
	// [1]: f_read=0 for normal 16 bit reads
	// [0]: active=1 to take the part out of standby and enable sampling
	// databyte = 0x15; 
	// if (s_i2c_write_regs(aFP, FXLS8471Q_SLAVE_ADDR, FXLS8471Q_CTRL_REG1, &databyte, (uint8_t) 1) != 1){
		// return (I2C_ERROR);
	// }
	// normal return
	return (I2C_OK);
}



// read status and the three channels of accelerometer data from 
// FXLS8471Q (7 bytes)
int16_t ReadAccel(SRAWDATA *pAccelData)
{
	uint8_t Buffer[FXLS8471Q_READ_LEN]; // read buffer
	// read FXLS8471Q_READ_LEN=7 bytes (status byte and the three channels of data)
	if (readBytes(FXLS8471Q_ADDRESS, FXLS8471Q_STATUS, FXLS8471Q_READ_LEN, Buffer))
	{
		// Serial.println("OK");
	// copy the 14 bit accelerometer byte data into 16 bit words
	pAccelData->x = ((Buffer[1] << 8) | Buffer[2])>> 2;
	pAccelData->y = ((Buffer[3] << 8) | Buffer[4])>> 2;
	pAccelData->z = ((Buffer[5] << 8) | Buffer[6])>> 2;
	}
	else
	{
	// return with error
	return (I2C_ERROR);
	}
	// normal return
	return (I2C_OK);
}






void writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

uint8_t readByte(uint8_t address, uint8_t subAddress)
{
	uint8_t data; // `data` will store the register data	 
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
	Wire.requestFrom(address, (uint8_t) 1);  // Read one uint8_t from slave register address 
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}


int readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
        Wire.requestFrom(address, count);  // Read bytes from slave register address 
	while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
	return (I2C_OK);
}
