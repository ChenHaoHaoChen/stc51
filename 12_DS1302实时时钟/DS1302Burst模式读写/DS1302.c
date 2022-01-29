/*********************
DS1302配置函数
*********************/
#include <reg52.h>
sbit DS1302_CE = P1^7;
sbit DS1302_SCK = P3^5;
sbit DS1302_SIO = P3^4;
unsigned char code date[7]={           //设置时间 2020-8-27 11：00：00
		0X00,0X14,0X11,0X28,0X08,0X06,0X21
	};
/*DS1302写字节函数*/
void DS1302writebyte(unsigned char dat)
{
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if((mask&dat) != 0) //先准备好SIO口数据，再拉高拉低时钟SCK口
		{
			DS1302_SIO = 1;      
		}
		else
		{
			DS1302_SIO = 0;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
}
/*DS1302写字节函数*/
unsigned char DS1302readbyte()
{
	unsigned char dat = 0;
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if(DS1302_SIO != 0)
		{
			dat = dat | mask;//先读取SIO值，再拉高拉低时钟SCK口
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
	return dat;
}
/*DS1302 单读 写函数*/
void DS1302singlewrite(unsigned char addsn,unsigned char dat)
{
	DS1302_CE = 1;         //打开使能端
	DS1302writebyte((addsn<<1) | 0X80);//写入地址
	DS1302writebyte(dat);  //写入数据
	DS1302_CE = 0;         //关闭使能端
}
/*DS1302 单独 读函数*/
unsigned char DS1302singleread(unsigned char addsn)
{
	unsigned char dat;
	DS1302_CE = 1;         //打开使能端
	DS1302writebyte((addsn<<1) | 0X81);//写入地址
	dat = DS1302readbyte();//读取数据
	DS1302_CE = 0;         //关闭使能端
	return dat;
}
/*DS1302burst模式写函数*/
void DS1302burstwrite(unsigned char *p)
{
	unsigned char i;
	DS1302_CE = 1;
	DS1302writebyte(0XBE);
	for(i=0;i<8;i++)      //burst模式一次可以写八个寄存器
	{
		DS1302writebyte(p[i]);
	}
	DS1302_CE = 0;
}
/*DS1302burst模式读函数*/
void DS1302burstread(unsigned char *p)
{
	unsigned char i;
	DS1302_CE = 1;
	DS1302writebyte(0XBF);
	for(i=0;i<8;i++)      //burst模式一次可以读八个寄存器
	{
		p[i] = DS1302readbyte();
	}
	DS1302_CE = 0;
}
/*DS1302初始化函数*/
void DS1302init()
{
	DS1302_CE = 0;       //51MCU默认输出高电平，由软件拉低使能端（CE）和时钟端（SCK）电平
	DS1302_SCK = 0;
	DS1302singlewrite(7,0X00);//写保护位值0，即允许写数据
	DS1302burstwrite(date);//采用burst模式写入设置好的时间
}
