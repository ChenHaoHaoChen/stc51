#ifndef _I2C_H
#define _I2C_H
extern void I2Cstart();                 //I2C通信起始信号
extern void I2Cstop();                  //I2C通信终止信号
extern unsigned char I2CreadACK();      //I2C读并发送应答位
extern unsigned char I2CreadNACK();     //I2C读并发送非应答位
extern bit I2Cwrite(unsigned char dat); //I2C写
#endif