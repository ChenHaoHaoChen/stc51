#ifndef _I2C_H
#define _I2C_H
extern void I2Cstart();                 //I2Cͨ����ʼ�ź�
extern void I2Cstop();                  //I2Cͨ����ֹ�ź�
extern unsigned char I2CreadACK();      //I2C��������Ӧ��λ
extern unsigned char I2CreadNACK();     //I2C�������ͷ�Ӧ��λ
extern bit I2Cwrite(unsigned char dat); //I2Cд
#endif