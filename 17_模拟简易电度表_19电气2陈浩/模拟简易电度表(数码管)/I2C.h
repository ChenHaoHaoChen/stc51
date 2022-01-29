#ifndef _I2C_H
#define _I2C_H
extern void I2Cstart();
extern void I2Cstop();
extern unsigned char I2CreadACK();
extern unsigned char I2CreadNACK();
extern bit I2Cwrite(unsigned char dat);
#endif