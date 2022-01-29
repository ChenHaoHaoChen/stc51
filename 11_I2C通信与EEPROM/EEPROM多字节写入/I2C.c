/********************
I2C通信配置函数
********************/
#include <reg52.h>
#include <intrins.h>
#define I2Cdelay() {_nop_();_nop_();_nop_();_nop_();}
sbit I2C_SCL = P3^7;//用单片机I/O口模拟I2C通信，（STC89C52没有专门I2C通信寄存器）
sbit I2C_SDA = P3^6;
/*I2C通信起始信号函数*/
void I2Cstart()
{
	I2C_SCL = 1;
	I2C_SDA = 1;
	I2Cdelay();
	I2C_SDA = 0;
	I2Cdelay();
	I2C_SCL = 0;
}
/*I2C通信写数据函数*/
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
/*I2C通信停止函数*/
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
/*I2C通信读操作函数，并发送非应答位*/
unsigned char I2CreadNACK()
{
	unsigned char dat;
	unsigned char mask;
	I2C_SDA = 1;             //读取状态前释放SDA
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
/*I2C通信读操作函数，并发送应答位*/
unsigned char I2CreadACK()
{
	unsigned char dat;
	unsigned char mask;
	I2C_SDA = 1;             //读取状态前释放SDA
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












