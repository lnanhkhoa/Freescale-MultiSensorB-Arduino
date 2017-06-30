



# Freescale Multi Sensor B

^ - FXLS8471Q Accelerometer SPI
	#define FXLS8471Q_WHOAMI 0x0D
	#define FXLS8471Q_WHOAMI_VAL 0x6A

^ - MMA8652Q Accelerometer I2C
	#define MMA8652_SLAVE_ADDR 0x3A
	#define MMA8652_WHOAMI 0x0D
	#define MMA8652_WHOAMI_VAL 0x4A

^ - FXOS8700Q 6-axis accelerometer - magnetometer
	#define FXOS8700Q_WHOAMI 0x0D
	#define FXOS8700Q_WHOAMI_VAL 0xC7

^ - MAG3110 3-axis magnetometer
	#define MAG_WHOAMI  0x07
	#define MAG_3110_WHO_AM_I_VALUE 0xC4

^ - MPL3115A2 presure
	#define REG_WHO_AM_I        0x0C 
	#define MPL3115A2_WHOAMI_VAL 0xD1

^ - MMA9553L pedometer 
	#define MMA9553_Slave_Addr  0x4C

^ - FXAS21000 gyroscope
	#define FXAS21000_WHOAMI 0x0C
	#define FXAS21000_WHOAMI_VAL 0xD1