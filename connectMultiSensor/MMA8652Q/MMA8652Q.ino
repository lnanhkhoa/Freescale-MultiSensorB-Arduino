

#ifndef WIRE_H
#define WIRE_H
#include <Wire.h>
#endif

#include "MMA8652FC.h"
// int ledPin = 13;


// typedef struct
// {
// int16_t x;
// int16_t y;
// int16_t z;
// } SRAWDATA;

// SRAWDATA Accel;

int16_t accelCount[3];  // Stores the 12-bit signed value
float ax, ay, az;       // Stores the real accel value in g's
uint32_t count = 0;
bool sleepMode = false;



MMA8652 _MMA8652(5,4);



void setup()
{
    // pinMode(ledPin, OUTPUT);
    // digitalWrite(ledPin, LOW);

    Serial.begin(38400);
    _MMA8652.begin();
    // _MMA8652.Reset();
    // Read the WHO_AM_I register, this is a good test of communication


    uint8_t c = _MMA8652.getWhoAmI();

    if (c == _MMA8652.WhoAmIValue()) // WHO_AM_I should always be 0x4A
    { 
        Serial.println("MMA8652FC 3-axis 12-bit accelerometer 2 mg LSB.");
        Serial.print("WHO_AM_I should always be 0x4A. ");
        Serial.print("So, I am 0x");  
        Serial.println(c, HEX);

        // _MMA8652.Reset(); // Start by resetting sensor device to default settings
        _MMA8652.calibrate();
        _MMA8652.init();  // init the accelerometer if communication is OK
        Serial.println("MMA8652FC is online...");
        // _MMA8652.config();
        delay(2000);
    }
    else
    {
        Serial.print("I Am 0x");  
        Serial.print(c, HEX);  
        Serial.println(". I Should be 0x4A!"); 
        Serial.println("Could not connect to MMA8652FC");
        while(1) ; // Loop forever if communication doesn't happen
    }
}


void loop1()
{
    float data[3];
    // int data[3];

    while(true){
        _MMA8652.ReadXYZ(data);
        // _MMA8652.ReadXYZraw(data);
        Serial.print(data[0]);
        Serial.print(",");
        Serial.print(data[1]);
        Serial.print(",");
        Serial.println(data[2]);
        delay(100);
        // statement
    }
}

void loop()
{
    // float data[3];
    int16_t data[3];
    // One can use the interrupt pins to detect a data ready condition; here we just check the STATUS register for a data ready bit
    //Serial.println(_MMA8652.status());
    // if( _MMA8652.status() & 0x08)
    // {
        // _MMA8652.ReadXYZ(data);
        _MMA8652.ReadXYZraw(data);
        Serial.print(data[0]);
        Serial.print(",");
        Serial.print(data[1]);
        Serial.print(",");
        Serial.println(data[2]);
        // delay(50);
    // }
    // while(1){
    //     // statement
    // }
}



