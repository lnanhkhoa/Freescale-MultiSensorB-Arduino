
#ifndef  SPI_ARDUINO_H 
#define  SPI_ARDUINO_H 


typedef byte PinName;


class spi_arduino
{
public: 
	void write(int16_t * d);

	void read(void);

	void cs(bool statePinCs);

private:

}

#endif
