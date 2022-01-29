/*************************
DS18B20温度传感器配置函数
**************************/
#include <reg52.h>
#include <intrins.h>
sbit DS18B20_DQ = P3^2;
/*10*x us延时*/
void delay(unsigned int t)
{
	while(t--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
/*DS18B20初始化函数*/
bit DS18B20getack()
{
	bit ack;
	EA = 0;
	DS18B20_DQ = 0;
	delay(50);
	DS18B20_DQ = 1;
	delay(6);
	ack = DS18B20_DQ;
	while(!DS18B20_DQ);
	EA = 1;
	return ack;
}
/*DS18B20写一字节函数*/
void DS18B20write(unsigned char dat)
{
	unsigned char mask;
	EA = 0;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		DS18B20_DQ = 0;
		_nop_();
		_nop_();
		if((mask&dat) == 0)
		{
			DS18B20_DQ = 0;
		}
		else
		{
			DS18B20_DQ = 1;
		}
		delay(6);
		DS18B20_DQ = 1;
	}
	EA = 1;
}
/*DS18B20读一字节函数*/
unsigned char DS18B20read()
{
	unsigned char dat;
	unsigned char mask;
	EA= 0;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		DS18B20_DQ = 0;
		_nop_();
		_nop_();
		DS18B20_DQ = 1;
		_nop_();
		_nop_();
		if(!DS18B20_DQ)
		{
			dat = dat & (~mask);
		}
		else
		{
			dat = dat | mask;
	  }
		delay(6);
	}
	EA = 1;
	return dat;
}
/*启动一次温度转换*/
bit DS18B20start()
{
	bit ack;
	ack = DS18B20getack();
	if(ack == 0)
	{
		DS18B20write(0XCC);//跳过ROM操作
		DS18B20write(0X44);//启动一次温度转换
	}
	return ~ack;
}
/*读取DS18B20温度转换值*/
bit DS18B20gettemp(int *temp)
{
	bit ack;
	unsigned char LSB,MSB;
	ack = DS18B20getack();
	if(ack == 0)
	{
		DS18B20write(0XCC); //跳过ROM操作
		DS18B20write(0XBE); //发送读取数据寄存器命令
		LSB = DS18B20read();//读取数据寄存器低字节
		MSB = DS18B20read();//读取数据寄存器高字节
		*temp = ((int)MSB <<8) + LSB;
	}
	return ~ack;
}