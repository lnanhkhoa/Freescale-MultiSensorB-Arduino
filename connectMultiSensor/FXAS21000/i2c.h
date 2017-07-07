/* Microcontroller Library - I2C
 * Copyright (c) 2017 Arduino Limited. All rights reserved.
 * Company: Memsitech
 * Author: lnanhkhoa
 */ 
//
// Created by User on 6/29/2017.
//

#ifndef ARDUINO_I2C_H
#define ARDUINO_I2C_H

/* Microcontroller Library - I2C
 * Copyright (c) 2017 Arduino Limited. All rights reserved.
 * Company: Memsitech
 * Author: lnanhkhoa
 */

typedef int  PinName;


class I2C
{
public:

    /** Create an I2C Master interface, connected to the specified pins
     *
     *  @param sda I2C data line pin
     *  @param scl I2C clock line pin
     */
    I2C(PinName sda, PinName scl);

    void read(int address, char * data, uint8_t length);

    /** Read a single byte from the I2C bus
     *
     *  @param ack indicates if the byte is to be acknowledged (1 = acknowledge)
     *
     *  @returns
     *    the byte read
     */
    void write(int address,  char cmd, uint8_t length, bool repeated = false);

    /** Write single byte out on the I2C bus
     *
     *  @param cmd cmd to write out on bus
     *
     *  @returns
     *    '1' if an ACK was received,
     *    '0' otherwise
     */
    
    void start(void);

    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);

    uint8_t readByte(uint8_t address, uint8_t subAddress);

    void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);



protected:

    void aquire();
};

#endif //I2C_I2C_H
