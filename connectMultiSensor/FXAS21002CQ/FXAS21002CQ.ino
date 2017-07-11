

#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif

#ifndef WIRE_H
#define WIRE_H
#include <Wire.h>
#endif

#include "FXAS21002CQ.h"


int16_t accelCount[3];  // Stores the 12-bit signed value
float ax, ay, az;       // Stores the real accel value in g's
uint32_t count = 0;
bool sleepMode = false;

float gRes, gBias[3] = {0, 0, 0}; // scale resolutions per LSB for the sensors

FXAS21002CQ _FXAS21002CQ(5,4);



void setup()
{
    // pinMode(ledPin, OUTPUT);
    // digitalWrite(ledPin, LOW);

    Serial.begin(38400);
    Serial.println("START");
    _FXAS21002CQ.begin();
    // Read the WHO_AM_I register, this is a good test of communication


    char c = _FXAS21002CQ.getWhoAmI();
    if (c == _FXAS21002CQ.WhoAmIValue()) // WHO_AM_I should always be 0x4A
    { 
        Serial.println("FXAS21002CQ 3-axis 14-bit gyroscope 25 mdeg/s LSB.");
        Serial.print("WHO_AM_I should always be 0xD7. ");
        Serial.print("So, I am 0x");  
        Serial.println(c, HEX);

        _FXAS21002CQ.Reset(); // Start by resetting sensor device to default settings
        _FXAS21002CQ.calibrate(gBias);
        _FXAS21002CQ.init();  // init the accelerometer if communication is OK
        Serial.println("FXAS21002CQ is online...");
    //     // _MMA8652.config();
        delay(2000);
    }
    else
    {
        Serial.print("I Am 0x");  
        Serial.print(c, HEX);  
        Serial.println(". I Should be 0xD1!"); 
        Serial.println("Could not connect to FXAS21002CQ");
        while(1) ; // Loop forever if communication doesn't happen
    }
}

void loop()
{
    float data[3];
    // int16_t data[3];
    // One can use the interrupt pins to detect a data ready condition; here we just check the STATUS register for a data ready bit
    //Serial.println(_MAG3110.status());
    if( _FXAS21002CQ.status() & 0x08)
    {
        _FXAS21002CQ.ReadXYZ(data);
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



