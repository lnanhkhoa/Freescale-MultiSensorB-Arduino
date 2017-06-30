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
    I2C(PinName sda, PinName scl, const char *name = NULL);

    /** Set the frequency of the I2C interface
     *
     *  @param hz The bus frequency in hertz
     */
    void frequency(int hz);

    /** Read from an I2C slave
     *
     *  Performs a complete read transaction. The bottom bit of
     *  the address is forced to 1 to indicate a read.
     *
     *  @param address 8-bit I2C slave address [ addr | 1 ]
     *  @param data Pointer to the byte-array to read data in to
     *  @param length Number of bytes to read
     *  @param repeated Repeated start, true - don't send stop at end
     *
     *  @returns
     *       0 on success (ack),
     *   non-0 on failure (nack)
     */
    int read(int address, char *data, int length, bool repeated = false);

    /** Read a single byte from the I2C bus
     *
     *  @param ack indicates if the byte is to be acknowledged (1 = acknowledge)
     *
     *  @returns
     *    the byte read
     */
    int write(int address, const char *data, int length, bool repeated = false);

    /** Write single byte out on the I2C bus
     *
     *  @param data data to write out on bus
     *
     *  @returns
     *    '1' if an ACK was received,
     *    '0' otherwise
     */


    void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    
    void start(void);

    /** Creates a stop condition on the I2C bus
     */
    void stop(void);

protected:

    void aquire();

    //I2CName     _i2c;
    static I2C  *_owner;
    int         _hz;

};

#endif //I2C_I2C_H
