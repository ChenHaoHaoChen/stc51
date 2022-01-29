/**************************
      DS1302通信程序
**************************/
#include <reg52.h>
sbit SCE = P2^3;
sbit SCK = P2^4;
sbit SIO = P2^5;
unsigned char DS1302singleread(unsigned char adr);
void DS1302singlewrite(unsigned char adr,unsigned char date);
void DS1302burstwrite(unsigned char *date);
void DS1302burstread(unsigned char *date);
void DS1302bytewrite(unsigned char date);
unsigned char DS1302byteread();

unsigned char code timeinit[] = {   //2021年11月29日09时14分00秒，星期一
	0X00,0X14,0X09,0X29,0X11,0X01,0X21
};

/**********DS1302初始化**********/
void DS1302init()
{
	unsigned char date;
	SCE = 0;  //使能引脚默认低电平
	SCK = 0;  //时钟引脚默认低电平
	date = DS1302singleread(0x81);  //读秒寄存器
	if((date & 0x80) != 0)          //若秒寄存器最高位为1，则时钟停止工作
	{                             
		DS1302singlewrite(0X8E,0X00); //去除写保护位
		DS1302burstwrite(timeinit);   //burst模式写时间
	}
}
/**********DS1302读操作**********/
unsigned char DS1302singleread(unsigned char adr)
{
	unsigned char date;
	SCE = 1;                //使能片选信号
	DS1302bytewrite(adr);   //单字节写入要读取数据的地址
	date = DS1302byteread();//单字节读
	SCE = 0;                //除能片选信号
	return date;
}
/**********DS1302写操作**********/
void DS1302singlewrite(unsigned char adr,unsigned char date)
{
	SCE = 1;              //使能片选信号
	DS1302bytewrite(adr); //单字节写入要写入数据的地址
	DS1302bytewrite(date);//单字节写入数据
	SCE = 0;              //除能片选信号
}
/**********DS1302burst写操作**********/
void DS1302burstwrite(unsigned char *date)
{
	unsigned char i;
	SCE = 1;              //使能片选信号
	DS1302bytewrite(0XBE);//设置为burst模式
	for(i=0;i<8;i++)
	{
		DS1302bytewrite(date[i]);//一次性写入八组寄存器数据
	}
	SCE = 0;              //除能片选信号
}
/**********DS1302burst读操作**********/
void DS1302burstread(unsigned char *date)
{
	unsigned char i;
	SCE = 1;              //使能片选信号
	DS1302bytewrite(0XBF);//设置为burst模式
	for(i=0;i<8;i++)
	{
		date[i] = DS1302byteread();//一次性读取八组寄存器数据
	}
	SCE = 0;              //除能片选信号
}
/**********DS1302单字节写操作**********/
void DS1302bytewrite(unsigned char date)
{
	unsigned char i;
	for(i=0X01; i!=0; i=i<<1)
	{
		if((i&date) != 0)
		{
			SIO = 1;
		}
		else
		{
			SIO = 0;
		}
		SCK = 1;
		SCK = 0;
	}
	SIO = 1;
}
/**********DS1302单字节操作**********/
unsigned char DS1302byteread()
{
	unsigned char i;
	unsigned char date = 0;
	for(i=0X01;i!=0;i=i<<1)
	{
		if(SIO != 0)
		{
			date = date | i;
		}
		SCK = 1;
		SCK = 0;
	}
	return date;
}