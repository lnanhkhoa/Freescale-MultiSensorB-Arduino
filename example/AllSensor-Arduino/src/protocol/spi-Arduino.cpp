
#ifndef  SPI_H 
#define  SPI_H 
#include <SPI.h>
#endif

#include "spi-Arduino.h"


void spi_arduino::write(int16_t * d){
	SPI.transfer(d);
}


void spi_arduino::read(void){

}

void spi_arduino::cs(bool statePinCs){
	
	static PinName slaveSelectPin;
    if (statePinCs)
        digitalWrite(slaveSelectPin, HIGH);
    else
        digitalWrite(slaveSelectPin, LOW);
}
