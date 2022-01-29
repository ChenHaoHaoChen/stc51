/*************************
DS18B20�¶ȴ��������ú���
**************************/
#include <reg52.h>
#include <intrins.h>
sbit DS18B20_DQ = P3^2;
/*10*x us��ʱ*/
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
/*DS18B20��ʼ������*/
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
/*DS18B20дһ�ֽں���*/
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
/*DS18B20��һ�ֽں���*/
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
/*����һ���¶�ת��*/
bit DS18B20start()
{
	bit ack;
	ack = DS18B20getack();
	if(ack == 0)
	{
		DS18B20write(0XCC);//����ROM����
		DS18B20write(0X44);//����һ���¶�ת��
	}
	return ~ack;
}
/*��ȡDS18B20�¶�ת��ֵ*/
bit DS18B20gettemp(int *temp)
{
	bit ack;
	unsigned char LSB,MSB;
	ack = DS18B20getack();
	if(ack == 0)
	{
		DS18B20write(0XCC); //����ROM����
		DS18B20write(0XBE); //���Ͷ�ȡ���ݼĴ�������
		LSB = DS18B20read();//��ȡ���ݼĴ������ֽ�
		MSB = DS18B20read();//��ȡ���ݼĴ������ֽ�
		*temp = ((int)MSB <<8) + LSB;
	}
	return ~ack;
}