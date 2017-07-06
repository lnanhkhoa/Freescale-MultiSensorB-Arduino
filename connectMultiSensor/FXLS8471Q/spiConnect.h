
#ifndef  SPI_ARDUINO_H 
#define  SPI_ARDUINO_H 


typedef byte PinName;


class spi_arduino
{
public:
	PinName _mosi;
    PinName _miso;
    PinName _scl;
    PinName _cs;
    void begin(void);
	void write(int16_t * d);

	void setPin(PinName mosi, PinName miso, PinName scl, PinName cs);

	int read(int16_t * d);

	void cs(bool statePinCs);

// private:

};

#endif
