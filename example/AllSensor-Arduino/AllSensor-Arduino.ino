
// //#include "MAG3110.h"
// //#include "FXOS8700Q.h"
// #include "MMA8652.h"
// // #include "FXLS8471Q.h"
// // #include "FXAS21000.h"

// // Requires a Freescale-compatible platform. Comment if you're getting weird results for the alt or any other sensor. Also check the interrupt params below.
// #define FSL_COMPATIBLE
// #if defined(FSL_COMPATIBLE)
//     #define MPL3115A2_I2C_ADDRESS (0x60<<1)
//     #include "MPL3115A2.h" 
    
//     MPL3115A2 alt(D14, D15, MPL3115A2_I2C_ADDRESS, D4, D3);
// #endif

// FXLS8471Q acc1(D11, D12, D13, D10);
// MMA8652   acc2(D14, D15);
// FXOS8700Q_acc combo_acc(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
// FXOS8700Q_mag combo_mag(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
// MAG3110   mag2(D14, D15);
// FXAS21000 gyro(D14, D15);


// Serial pc(USBTX, USBRX);

// ****************************************************************************************
// //****************************************************************************************
// ///***************************************************************************************
//     float print_AltimiterValue( unsigned char *dt)
//     {
//         unsigned short altm;
//         float faltm;

//         //
//         // dt[0] = Bits 12-19 of 20-bit real-time Altitude sample. (b7-b0)
//         // dt[1] = Bits 4-11 of 20-bit real-time Altitude sample. (b7-b0)
//         // dt[2] = Bits 0-3 of 20-bit real-time Altitude sample (b7-b4)
//         //
//         altm = (dt[0]<<8) | dt[1];
//         //
//         if ( dt[0] > 0x7F) {
//             altm = ~altm + 1;
//             faltm = (float)altm * -1.0f;
//         } else {
//             faltm = (float)altm * 1.0f;
//         }
//         //
//         faltm = faltm+((float)(dt[2]>>4) * 0.0625f);
//         return faltm;
//     }

//     float print_TemperatureValue( unsigned char *dt)
//     {
//         unsigned short temp;
//         float ftemp;

//         //
//         // dt[0] = Bits 4-11 of 16-bit real-time temperature sample. (b7-b0)
//         // dt[1] = Bits 0-3 of 16-bit real-time temperature sample. (b7-b4)
//         //
//         temp = dt[0];
//         //
//         if ( dt[0] > 0x7F) {
//             temp = ~temp + 1;
//             ftemp = (float)temp * -1.0f;
//         } else {
//             ftemp = (float)temp * 1.0f;
//         }
//         //
//         ftemp = ftemp+((float)(dt[1]>>4) * 0.0625f);
//         return ftemp;

//     }



// int main()
// {
//     float acc_data[3], gyro_data[3];
//     MotionSensorDataUnits adata;
//     MotionSensorDataUnits mdata;
//     int16_t acc_raw[3];

//     printf("\r\nStarting\r\n\r\n");

//     combo_acc.enable();
//     combo_mag.enable();
//     mag2.enable();
//     printf("FXLS8471 Acc   = %X\r\n", acc1.getWhoAmI());
//     printf("MMA8652 Acc    = %X\r\n", acc2.getWhoAmI());
//     printf("FXOS8700 Combo = %X\r\n", combo_acc.whoAmI());
//     printf("MAG3110 Mag    = %X\r\n", mag2.whoAmI());
//     printf("FXAS21000 Gyro = %X\r\n", gyro.getWhoAmI());
    
//     #if defined(FSL_COMPATIBLE)
//         unsigned char alt_data[8];
//         alt.Altimeter_Mode();
//         printf("MPL3115A2 Alt  = %X\r\n", alt.getDeviceID());  // May only be used on a Freescale platform, comment out for others
//     #endif
    
//     wait(3);
    
//     while(1) {
//         acc1.ReadXYZ(acc_data);
//         acc1.ReadXYZraw(acc_raw);
//         printf("FXLS8471 Acc:   X:%6.3f Y:%6.3f Z:%6.3f (Raw X:%4d Y:%4d Z:%4d)\r\n", acc_data[0], acc_data[1], acc_data[2], acc_raw[0], acc_raw[1], acc_raw[2]);

//         acc2.ReadXYZ(acc_data);
//         acc2.ReadXYZraw(acc_raw);
//         printf("MMA8652 Acc:    X:%6.3f Y:%6.3f Z:%6.3f (Raw X:%4d Y:%4d Z:%4d)\r\n", acc_data[0], acc_data[1], acc_data[2], acc_raw[0], acc_raw[1], acc_raw[2]);

//         combo_acc.getAxis(adata);
//         printf("FXOS8700 Acc:   X:%6.3f Y:%6.3f Z:%6.3f\r\n", adata.x, adata.y, adata.z);
        
//         combo_mag.getAxis(mdata);
//         printf("FXOS8700 Mag:   X:%6.2f Y:%6.2f Z:%6.2f\r\n", mdata.x, mdata.y, mdata.z);

//         mag2.getAxis(mdata);
//         printf("MAG3110 Mag:    X:%6.2f Y:%6.2f Z:%6.2f\r\n", mdata.x, mdata.y, mdata.z);

//         gyro.ReadXYZ(gyro_data);
//         printf("FXAS21000 Gyro: X:%6.2f Y:%6.2f Z:%6.2f\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
        
//         #if defined(FSL_COMPATIBLE)
//             alt.getAllDataRaw(&alt_data[0]);  // May only be used on a Freescale platform, comment out for others
//             printf("MPL3115A2 Alt:  %5.1f\r\n", print_AltimiterValue(&alt_data[0]));  // May only be used on a Freescale platform, comment out for others
//             printf("MPL3115A2 Temp: %3.1f\r\n", print_TemperatureValue(&alt_data[3]));  // May only be used on a Freescale platform, comment out for others
//         #endif

//         printf("\r\n");
        
//         wait(1);
//     }
// }

#include "Wire.h"
#include "i2c.h"
#include "source/FXLS8471.h"



/*******
*
*   Connect Arduino
*
********/

#include "connectArduino/arduino-FXLS8471Q.h"


I2C _i2c(2,3);

void MMA8652_begin(void)
{
    char data[2];
    // write 0000 0000 = 0x00 to accelerometer control register 1 to place MMA8652 into
    // standby
    // [7-1] = 0000 000
    // [0]: active=0
    data[0] = MMA8652_CTRL_REG1;
    data[1] = 0x00;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);
    
    // write 0000 0001= 0x01 to XYZ_DATA_CFG register
    // [7]: reserved
    // [6]: reserved
    // [5]: reserved
    // [4]: hpf_out=0
    // [3]: reserved
    // [2]: reserved
    // [1-0]: fs=00 for accelerometer range of +/-2g range with 0.244mg/LSB
    data[0] = MMA8652_XYZ_DATA_CFG;
    data[1] = 0x00;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);

    // write 0000 1101 = 0x0D to accelerometer control register 1
    // [7-6]: aslp_rate=00
    // [5-3]: dr=100 for 50Hz data rate
    // [2]: 0
    // [1]: 0
    // [0]: active=1 to take the part out of standby and enable sampling
    data[0] = MMA8652_CTRL_REG1;
    data[1] = 0x21;
    _i2c.write( MMA8652_SLAVE_ADDR, data, 2);
}

byte MMA8652_RegRead( char reg, int len)
{
    byte data;
    char i2c_address = FXAS21000_SLAVE_ADDR;
    Wire.beginTransmission(i2c_address);         // Initialize the Tx buffer
    Wire.write(reg);                  // Put slave register address in Tx buffer
    Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
    Wire.requestFrom(i2c_address, 1);  // Read one uint8_t from slave register address 
    data = Wire.read();                      // Fill Rx buffer with result
    return data;           
}

byte  MMA8652_getWhoAmI(void)

{
    static byte data;
    data = MMA8652_RegRead(FXAS21000_WHOAMI, 1);
    return data;
}









void setup(){
    Serial.println("START");

    byte data;
    Serial.begin(38400);
    _i2c.start();
    

    //FXLS8471();
    //FXLS8471Q.setup();

    data = MMA8652_getWhoAmI();
    Serial.println(data,HEX);

}

void loop(){

    //FXLS8471Q.main();
    Serial.println("end");

    while(1){
        // statement
    }
}




