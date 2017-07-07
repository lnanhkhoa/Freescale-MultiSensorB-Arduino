

#ifndef WIRE_H
#define WIRE_H
#include <Wire.h>
#endif

#include "FXAS21000.h"


int16_t accelCount[3];  // Stores the 12-bit signed value
float ax, ay, az;       // Stores the real accel value in g's
uint32_t count = 0;
bool sleepMode = false;

float gRes, gBias[3] = {0, 0, 0}; // scale resolutions per LSB for the sensors

FXAS21000 _FXAS21000(5,4);



void setup()
{
    // pinMode(ledPin, OUTPUT);
    // digitalWrite(ledPin, LOW);

    Serial.begin(38400);
    Serial.println("OK");
    _FXAS21000.begin();
    // _MMA8652.Reset();
    // Read the WHO_AM_I register, this is a good test of communication


    char c = _FXAS21000.getWhoAmI();
    if (c == _FXAS21000.WhoAmIValue()) // WHO_AM_I should always be 0x4A
    { 
        Serial.println("MMA3110 3-axis 16-bit magnetometer 1 mGauss LSB.");
        Serial.print("WHO_AM_I should always be 0xC4. ");
        Serial.print("So, I am 0x");  
        Serial.println(c, HEX);

        _FXAS21000.Reset(); // Start by resetting sensor device to default settings
        _FXAS21000.calibrate(gBias);
        _FXAS21000.init();  // init the accelerometer if communication is OK
        Serial.println("MMA3110 is online...");
    //     // _MMA8652.config();
        delay(2000);
    }
    else
    {
        Serial.print("I Am 0x");  
        Serial.print(c, HEX);  
        Serial.println(". I Should be 0xC4!"); 
        Serial.println("Could not connect to MAG3110");
        while(1) ; // Loop forever if communication doesn't happen
    }
}

void loop()
{
    float data[3];
    // int16_t data[3];
    // One can use the interrupt pins to detect a data ready condition; here we just check the STATUS register for a data ready bit
    //Serial.println(_MAG3110.status());
    if( _FXAS21000.status() & 0x08)
    {
        _FXAS21000.ReadXYZ(data);
        // _MAG3110.ReadXYZraw(data);
        Serial.print(data[0]);
        Serial.print(",");
        Serial.print(data[1]);
        Serial.print(",");
        Serial.println(data[2]);
        // delay(50);
    }
    // while(1){
    //     // statement
    // }
}



