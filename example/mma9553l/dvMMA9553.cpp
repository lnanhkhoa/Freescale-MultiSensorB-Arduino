/******************************************************************** 
*
*                File dvMMA9553.cpp
*   Name: dvMMA9553.cpp
*   Include file: dvMMA9553.h + dvMMA9553.cpp
*   Used library: arduinoi2c.h
*   For MCU: Arduino Uno R3
*   Purpose:    functions of pedometer MMA9553L for detect motion 
*               include ACTIVITY_UNKNOWN, ACTIVITY_REST, ACTIVITY_WALKING,
                ACTIVITY_JOGGING, ACTIVITY_RUNNING.
*   Include functions:
    void pedometer_main(void);
    void pedometer_init(void);
    void pedometer_reset(void);
    void pedometer_enable(void);
    void pedometer_disable(void);
    void pedometer_active(void);
    void pedometer_write_config(void);
    void pedometer_afe_config(void);
    void pedometer_wakeup(void);
    void pedometer_int0_enable(void);
    void pedometer_cmd_readstatus(void);
    void pedometer_cmd_readconfig(void);
    void dvMMA9553_Read(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len);
    void dvMMA9553_Write(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len);
    unsigned char dvMMA9553_I2C_RecvByte(void);
    void dvMMA9553_I2C_SendByte(unsigned char data);
    void dvMMA9553_I2C_Start(void);
    void dvMMA9553_I2C_Stop(void);
    void dvMMA9553_I2C_Ack(unsigned char ack);
    void dvMMA9553_init(void);
*
*
*
********************************************************************/

#include "Arduino.h"
#include "arduinoi2c.h"
#include "dvMMA9553.h"


/*================
*   Function's Name: delay_wwx
*   Purpose: delay 400 us
*   
*
=================*/
static void delay_wwx(void){
    int n;
    for(n=0; n<400; n++)
        ;
}

/*================
*   Function's Name: pedometer_init
*   Purpose:
*   
*
=================*/

void pedometer_init(void){
  unsigned char Buf[20];  
  int StepCount; //wenxue
  
    pedometer_reset();          // reset pedometer
    pedometer_write_config();       // config
    pedometer_enable();         // enable pedometer
    pedometer_int0_enable();    // enable INT_O pin
    pedometer_active();         // active MMA9553
    pedometer_wakeup();
    
    delay_wwx();
    pedometer_afe_config(); // afe config    wenxue
   delay_wwx();
        
    pedometer_cmd_readstatus();
        
    while(1){
       dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
       Serial.print("Buf[1]== ");// wenxue
       Serial.println(Buf[1]);// wenxue
       if(Buf[1]==0x80){
         dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 16); // 12 bytes status data + 4 bytes frame start 
         break;
        }
     }        
      // wenxue      
      // for(int i=0;i<16;i++){
      //    Serial.print("Buf[");
      //    Serial.print(i);
      //    Serial.print("]= ");
      //    Serial.println(Buf[i]);
      //   }
      StepCount = Buf[6] * 256 + Buf[7];
      Serial.print("StepCount= ");
    Serial.println(StepCount);
}

/*================
*   Function's Name: pedometer_main
*   Purpose:
*   
*
=================*/

void pedometer_main(void){
   unsigned char Buf[20];
   int StepCount,Distance,Calories; //wenxue
   pedometer_cmd_readstatus(); // read  // Ð´ÁË{0x15,0x30,0x00,0x0C}
        
   while(1)
   {
     dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
     // Serial.print("Buf[1]== ");
     // Serial.println(Buf[1]);
     delay_wwx();
     if(Buf[1]==0x80)
      {
        dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 16);  
        break;
       }
    }
    
    // wenxue      
     //   for(int i=0;i<16;i++){
     //     Serial.print("Buf[");
     //     Serial.print(i);
     //     Serial.print("]= ");
     //     Serial.println(Buf[i]);       
     // }
           
       StepCount = Buf[6] * 256 + Buf[7];
       Serial.print("StepCount= ");
       Serial.println(StepCount);

       Distance  = Buf[8] * 256 + Buf[9];
       Serial.print("Distance= ");
       Serial.println(Distance);

       Calories  = Buf[12] * 256 + Buf[13];
       Serial.print("Calories= ");
       Serial.println(Calories);
       Serial.println("");

}

/*================
*   Function's Name: pedometer_cmd_readstatus
*   Purpose:
*   
*
=================*/

void pedometer_cmd_readstatus(void){
    unsigned char Buf[]={0x15,0x30,0x00,0x0C};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4);
}

/*================
*   Function's Name: pedometer_cmd_readconfig
*   Purpose:
*   
*
=================*/

void pedometer_cmd_readconfig(void){
    unsigned char Buf[]={0x15,0x10,0x00,0x10};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
}

/*================
*   Function's Name: pedometer_cmd_readwakeup
*   Purpose:
*   
*
=================*/

void pedometer_cmd_readwakeup(void){
    unsigned char Buf[]={0x12,0x10,0x06,0x01};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
}

/*================
*   Function's Name: pedometer_write_config
*   Purpose:
*   
*
=================*/

void pedometer_write_config(void){
    unsigned char Buf[]={0x15,0x20,0x00,0x10,
                           0x0C,0xE0,
                           0x13,0x20,
                           0x00,0x96,
                           0x60,0x50,
                           0xAF,0x50,
                           0x04,0x03,
                           0x05,0x01,
                           0x00,0x00};

    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 20);
}

/*================
*   Function's Name: pedometer_reset
*   Code hex: 0x17
*   Purpose:
*   
*
=================*/

void pedometer_reset(void){
   unsigned char Buf[]={0x17,0x20,0x01,0x01,0x20};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_enable
*   Purpose:
*   
*
=================*/

void pedometer_enable(void){
    unsigned char Buf[]={0x17,0x20,0x05,0x01,0x00};

    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_disable
*   Purpose:
*   
*
=================*/

void pedometer_disable(void){
   unsigned char Buf[]={0x17,0x20,0x05,0x01,0x20};
   dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_active
*   Purpose:
*   
*
=================*/

void pedometer_active(void){
    unsigned char Buf[]={0x15,0x20,0x06,0x01,0x80};    
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_wakeup
*   Purpose:
*   
*
=================*/

void pedometer_wakeup(void){
    unsigned char Buf[]={0x12,0x20,0x06,0x01,0x00};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_int0_enable
*   Purpose:
*   
*
=================*/

void pedometer_int0_enable(void){
    unsigned char Buf[]={0x18,0x20,0x00,0x01,0xC0};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: dvMMA9553_Read
*   Purpose:
*   
*
=================*/

void dvMMA9553_Read(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len){
  I2c_ReadRegister(deviceAddr,regAddr,data,len);
}

/*================
*   Function's Name: dvMMA9553_Write
*   Purpose:
*   
*
=================*/

void dvMMA9553_Write(unsigned char deviceAddr,unsigned char regAddr, unsigned char *data, unsigned char len){
  I2c_WriteRegister(deviceAddr,regAddr,data,len);
}

/*================
*   Function's Name: pedometer_cmd__readlpfxyz
*   Purpose:
*   
*
=================*/

void pedometer_cmd__readlpfxyz(void){
    unsigned char Buf[]={0x06,0x30,0x18,0x06};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
  
}

/*================
*   Function's Name: pedometer_cmd__readrawxyz
*   Purpose:
*   
*
=================*/

void pedometer_cmd__readrawxyz(void){
    unsigned char Buf[]={0x06,0x30,0x12,0x06};
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 4 );
  
}

/*================
*   Function's Name: pedometer_mainrawxyz
*   Purpose:
*   
*
=================*/

void pedometer_mainrawxyz(void){
    unsigned char Buf[20];
    int x, y, z, i; 
    pedometer_cmd__readrawxyz(); //
    while(1){
       dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 2);
       delay_wwx();
       if(Buf[1]==0x80){
          dvMMA9553_Read(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 10);  
          break;
        }
    }
    // wenxue      
    for(i=0;i<10;i++){
        Serial.print("Buf[");
        Serial.print(i);
        Serial.print("]= ");
        Serial.print(Buf[i]);
    }
    x = Buf[4] * 256 + Buf[5];
    y = Buf[6] * 256 + Buf[7];
    z = Buf[8] * 256 + Buf[9];
   
    Serial.print("x= ");
    Serial.println(x);
    Serial.print("y= ");
    Serial.println(y);
    Serial.print("z= ");
    Serial.println(z);
}

/*================
*   Function's Name: pedometer_afe_config
*   Purpose:
*   
*
=================*/

void pedometer_afe_config(void){
  unsigned char Buf[]={0x06,0x20,0x00,0x01,0x40}; // 2g mode FS=01    1g--16393
   //unsigned char Buf[]={0x06,0x20,0x00,0x01,0x80}; // 4g mode FS=10    4g--8196
   // unsigned char Buf[]={0x06,0x20,0x00,0x01,0x00}; // 8g mode FS=00    8g--4098  
    dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf, 5);
}

/*================
*   Function's Name: pedometer_afe_config_read
*   Purpose:
*   
*
=================*/

void pedometer_afe_config_read(void){
   unsigned char Buf[]={0x06,0x10,0x00,0x01}; 
   dvMMA9553_Write(MMA9553_Slave_Addr, MMA9553_Sub_Addr, Buf,4);
}

/******************************************************************** 
*                Complete source dvMMA9553.h
********************************************************************/