/********************
I2Cͨ�����ú���
********************/
#include <reg52.h>
#include <intrins.h>
#define I2Cdelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^7;//�õ�Ƭ��I/O��ģ��I2Cͨ�ţ���STC89C52û��ר��I2Cͨ�żĴ�����
sbit I2C_SDA = P3^6;
/*I2Cͨ����ʼ�źź���*/
void I2Cstart()
{
	I2C_SCL = 1;
	I2C_SDA = 1;
	I2Cdelay();
	I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 0;
}
/*I2Cͨ��д���ݺ���*/
unsigned char I2Cwrite(unsigned char dat)
{
	unsigned char ack;
	unsigned char mask;
	for(mask=0X80;mask!=0;mask=mask>>1)
	{
		if((dat & mask) == 0)
		{
			I2C_SDA = 0;
		}
		else
		{
			I2C_SDA = 1;
		}
		I2Cdelay();
		I2C_SCL = 1;
		I2Cdelay();
		I2C_SCL = 0;
	}
	I2C_SDA = 1;
	I2Cdelay();
	I2C_SCL = 1;
	ack = I2C_SDA;
	I2Cdelay();
	I2C_SCL = 0;
	
	return (ack);
}
/*I2Cͨ��ֹͣ����*/
void I2Cstop()
{
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 1;
	I2Cdelay();
	I2C_SDA= 1;
	I2Cdelay();
}
/*I2Cͨ�Ŷ����������������ͷ�Ӧ��λ*/
unsigned char I2CreadNACK()
{
	unsigned char dat;
	unsigned char mask;
	I2C_SDA = 1;             //��ȡ״̬ǰ�ͷ�SDA
	for(mask=0X80;mask!=0;mask=mask>>1)
	{
		I2Cdelay();
		I2C_SCL = 1;
		if(I2C_SDA ==0)
		{
			dat = dat & (~mask);
		}
		else
		{
			dat = dat | mask;
		}
		I2Cdelay();
		I2C_SCL = 0;
	}
	I2C_SDA = 1;
	I2Cdelay();
	I2C_SCL = 1;
	I2Cdelay();
	I2C_SCL = 0;
	
	return dat;
}
/*I2Cͨ�Ŷ�����������������Ӧ��λ*/
unsigned char I2CreadACK()
{
	unsigned char dat;
	unsigned char mask;
	I2C_SDA = 1;             //��ȡ״̬ǰ�ͷ�SDA
	for(mask=0X80;mask!=0;mask=mask>>1)
	{
		I2Cdelay();
		I2C_SCL = 1;
		if(I2C_SDA == 0)
		{
			dat = dat & (~mask);
		}
		else
		{
			dat = dat | mask;
		}
		I2Cdelay();
		I2C_SCL = 0;
	}
	I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 1;
	I2Cdelay();
	I2C_SCL = 0;
	
	return dat;
}












