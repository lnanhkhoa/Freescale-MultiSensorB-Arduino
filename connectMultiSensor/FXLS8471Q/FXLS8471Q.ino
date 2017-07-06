
#include "FXLS8471Q.h"

FXLS8471Q _FXLS8471Q(13,12,11,10);

void setup()
{
	Serial.begin(38400);
	Serial.println("Start");

	_FXLS8471Q.begin();
	// byte a;
	// a = _FXLS8471Q.getWhoAmI();
	Serial.println(_FXLS8471Q.getWhoAmI(),HEX);
	

	Serial.println("--- Data x, y, z (unit g) ---");

	// Serial.println("--- Raw Data x, y, z ---");

}


void loop()
{
	float data[3];
	// int data[3];

	while(1){
		_FXLS8471Q.ReadXYZ(data);
		// _FXLS8471Q.ReadXYZraw(data);
		Serial.print(data[0]);
	  	Serial.print(",");
		Serial.print(data[1]);
	  	Serial.print(",");
	  	Serial.println(data[2]);
	  	delay(100);
	    // statement
	}


	Serial.println("Stop");
}