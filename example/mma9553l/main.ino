#include "Wire.h"
#include "dvMMA9553.h"


void setup(){
	Wire.begin();
	Serial.begin(9600);
	Serial.println("START");
	digitalWrite(A0,HIGH);
	digitalWrite(A1,HIGH);
	pedometer_init();
}

void loop(){
	pedometer_main();
}






