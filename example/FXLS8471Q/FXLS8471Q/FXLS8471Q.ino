//#include "FXLS8471Q.h"
#include "Wire.h"  
// #include <Adafruit_GFX.h>
// #include <Adafruit_PCD8544.h>

// FXLS8471Q internal register addresses

#define FXLS8471Q_STATUS 0x00
#define FXLS8471Q_WHOAMI 0x0D
#define FXLS8471Q_XYZ_DATA_CFG 0x0E
#define FXLS8471Q_CTRL_REG1 0x2A
#define FXLS8471Q_WHOAMI_VAL 0x6A
#define FXLS8471Q_READ_LEN 7
#define FXLS8471Q_OUT_X_MSB 0x01
#define FXLS8471Q_ADDRESS 0x1D



// Pin definitions
int ledPin  = 13;  // Pro Mini led
int16_t magCount[3];  // Stores the 12-bit signed value
float mx, my, mz;       // Stores the real accel value in g's
int8_t tempCount;
float temperature;
uint32_t count = 0;


void setup()
{
  Wire.begin();
  Serial.begin(38400);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

// Set up for data display
  // display.setTextSize(1); // Set text size to normal, 2 is twice normal etc.
  // display.setTextColor(BLACK); // Set pixel color; 1 on the monochrome screen
  // display.clearDisplay();   // clears the screen and buffer
  
  // Read the WHO_AM_I register, this is a good test of communication
  byte c = readByte(FXLS8471Q_ADDRESS, FXLS8471Q_WHOAMI);  // Read WHO_AM_I register
  Serial.print(c,HEX);
  // display.clearDisplay();
  // display.setCursor(0,0); display.print("MAG3110");  
  // display.setCursor(0,10); display.print("I Am");
  // display.setCursor(0, 20); display.print("Ox");display.print(c, HEX);  
  // display.setCursor(0, 30); display.print("I Should be"); 
  // display.setCursor(0, 40); display.print("Ox");display.print(0xC4, HEX);  
  // display.display();
  // delay(1000);
  if (c == 0x4A) // WHO_AM_I should always be 0x4A
  {  
    MAG3110Reset();  // Start by resetting sensor device to default settings
    MAG3110Offsets(); // Apply user offsets
    initMAG3110();   // init the accelerometer if communication is OK
    Serial.println("FXLS8471Q is online...");
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
    int raw[7];
    int16_t ix, iy, iz;
    readBytes(FXLS8471Q_STATUS, 0x07, raw);

    ix = ((raw[1] * 256) + raw[2]);// / 4;
    iy = ((raw[3] * 256) + raw[4]);// / 4;
    iz = ((raw[5] * 256) + raw[6]);// / 4;
    fdata[0] = ((float) ix) / 16384.0;
    fdata[1] = ((float) iy) / 16384.0;
    fdata[2] = ((float) iz) / 16384.0;
  
<<<<<<< HEAD
=======
  
  
  
  
  
  
  
  
>>>>>>> a185910ac7361e14bbfe5a84ce7cd2ea63a1c005
  // One can use the interrupt pins to detect a data ready condition; here we just check the STATUS register for a data ready bit
  if(readByte(FXLS8471Q_ADDRESS, DR_STATUS) & 0x08)  { // When this bit set, all axes have new data
 
    readMagData(magCount);               // Read the x/y/z adc values
    mx = (float)magCount[0]*10./32768.;  // get actual Gauss value 
    my = (float)magCount[1]*10./32768.;   
    mz = (float)magCount[2]*10./32768.;  
    

    tempCount = readTempData();  // Read the x/y/z adc values
    temperature = (float) tempCount + 26.; // Temperature in degrees Centigrade
  }   
    uint32_t deltat = millis() - count;
   if (deltat > 500) { // update LCD once per half-second independent of read rate
 
    // Print out values
    Serial.print("x-magnetic field = "); Serial.print(1000.*mx); Serial.print(" mG");   
    Serial.print("y-magnetic field = "); Serial.print(1000.*my); Serial.print(" mG");   
    Serial.print("z-magnetic field = "); Serial.print(1000.*mz); Serial.println(" mG");  
    
    count = millis();
    digitalWrite(ledPin, !digitalRead(ledPin));
   }
}




<<<<<<< HEAD
=======
// Set up sensor software reset
void MAG3110Reset() 
{
writeByte(FXLS8471Q_ADDRESS, CTRL_REG2, 0x10); // set reset bit to 1 to assert software reset to zero at end of boot process
}

// Allow user compensation of acceleration errors
void MAG3110Offsets()
{
   MAG3110Standby();  // Must be in standby to change registers
   
   writeByte(FXLS8471Q_ADDRESS, OFF_X_MSB, 0x00); // X-axis compensation; this is 0 mg
   writeByte(FXLS8471Q_ADDRESS, OFF_X_LSB, 0x00); // X-axis compensation; this is 0 mg
   writeByte(FXLS8471Q_ADDRESS, OFF_Y_MSB, 0x00); // X-axis compensation; this is 0 mg
   writeByte(FXLS8471Q_ADDRESS, OFF_Y_LSB, 0x00); // X-axis compensation; this is 0 mg
   writeByte(FXLS8471Q_ADDRESS, OFF_Z_MSB, 0x00); // X-axis compensation; this is 0 mg
   writeByte(FXLS8471Q_ADDRESS, OFF_Z_LSB, 0x00); // X-axis compensation; this is 0 mg
   
   MAG3110Active();  // Set to active to start reading
}

// Initialize the MAG3110 registers 
// See the many application notes for more info on setting all of these registers:
// http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MAG3110
// Feel free to modify any values, these are settings that work well for me.
void initMAG3110()
{
  MAG3110Standby();  // Must be in standby to change registers

  // Set up the magnetometer sample rate and oversample ratio
    writeByte(FXLS8471Q_ADDRESS, CTRL_REG1, magODR << 3);  
  // Enable automatic magnetic sensor resets
    writeByte(FXLS8471Q_ADDRESS, CTRL_REG2, 0x80);  // set normal mode, correct with user offset registers

  MAG3110Active();  // Set to active to start reading
}

// Sets the MAG3110 to standby mode.
// It must be in standby to change most register settings
void MAG3110Standby()
{
  byte c = readByte(FXLS8471Q_ADDRESS, CTRL_REG1);
  writeByte(FXLS8471Q_ADDRESS, CTRL_REG1, c & ~(0x01));
}

// Sets the MAG3110 to active mode.
// Needs to be in this mode to output data
void MAG3110Active()
{
  byte c = readByte(FXLS8471Q_ADDRESS, CTRL_REG1);
  writeByte(FXLS8471Q_ADDRESS, CTRL_REG1, c | 0x01);  
}


>>>>>>> a185910ac7361e14bbfe5a84ce7cd2ea63a1c005



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

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
        Wire.requestFrom(address, count);  // Read bytes from slave register address 
	while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer
}
