
#ifndef  SPI_H 
#define  SPI_H 
#include <SPI.h>
#endif

#include "spiConnect.h"


void spi_arduino::begin(void){
	SPI.begin();  
	SPI.setClockDivider(SPI_CLOCK_DIV16);   // 16 MHz / 16 = 1 MHz  
	SPI.setDataMode(SPI_MODE0);    //CPHA = CPOL = 0    MODE = 0  
	SPI.setBitOrder(MSBFIRST);  
	delay(1000);
}


void spi_arduino::write(int16_t * d){
	SPI.transfer(d);
}


int spi_arduino::read(int16_t * d){
	static int data;
	data = SPI.transfer(d);
	return(data);
}

void spi_arduino::setPin(PinName mosi, PinName miso, PinName scl, PinName cs){
	this->_mosi = mosi;
    this->_miso = miso;
    this->_scl = scl;
    this->_cs = cs;
}

void spi_arduino::cs(bool statePinCs){
    if (statePinCs)
        digitalWrite(10, HIGH);
    else
        digitalWrite(10, LOW);
}