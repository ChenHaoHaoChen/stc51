#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit PWMOUT = P0^0;
unsigned char highTH0 = 0;  //每一周期高电平定时时间
unsigned char highTL0 = 0;
unsigned char lowTH0 = 0;   //每一周期低电平定时时间
unsigned char lowTL0 = 0;
unsigned char TH1R = 0;     //定义每多少ms进入中断1
unsigned char TL1R = 0;
unsigned long count = 0;    //定义一个周期所需计数值
void configpwm(unsigned char fr,unsigned char dr); //函数声明
void configtime(unsigned int ms);
void adjustpwmdr(unsigned dr);

void main()
{
	ENLED = 0;                //打开74138译码器
	ADDR3 = 1;
	ADDR2 = 1;                //打开六个小灯端口
	ADDR1 = 1;
	ADDR0 = 0;
	EA = 1;                   //打开中断使能
	TMOD = 0X01;              //设置定时器为1模式
	configpwm(100,5);         //配置  
	configtime(50);
	while(1);
}

void configpwm(unsigned char fr,unsigned char dr) //配置PWM，fr为频率，dr为占空比
{
	unsigned int high,low;
	count = (11059200/12)/fr;
	high = (count*dr)/100;
	low = count - high;
	high = 65536 - high;
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);
	highTL0 = (unsigned char) high;
	lowTH0 = (unsigned char)(low >> 8);
	lowTL0 = (unsigned char) low;
	TH0 = highTH0;
	TL0 = highTL0;
	TR0 = 1;
	ET0 = 1;
	PWMOUT = 1;
}

void configtime(unsigned int ms)                 //配置并启动T1,功能是：每ms毫秒刷新一次
{
	unsigned long temp;
	temp =65536 - (11059200/12)*(ms/1000);
	TH1R = (unsigned char)(temp >> 8);
	TL1R = (unsigned char) temp;
	TH1 = TH1R;
	TL1 = TL1R;
	TR1= 1;
	ET1 = 1;
}

void interrupttime0() interrupt 1               //进入一次中断0改变一次输出的高低电平
{
	if(PWMOUT == 1)
	{
		PWMOUT = 0;
		TH0 = lowTH0; 
		TL0 = lowTL0;
	}
	else
	{
		PWMOUT = 1;
		TH0 = highTH0;
		TL0 = highTL0;
	}
}

void interrupttiime1() interrupt 3              //进入一次中断1，调整一次占空比
{
	static unsigned char table[]={
		5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95
	};
	static unsigned char dir = 0;
	static unsigned char index = 0;
	TH1 = TH1R;
	TL1 = TL1R;
	adjustpwmdr(table[index]);
	if(dir == 0)
	{
		index++;
		if(index == 18)
		{
			dir = 1;
		}
	}
	else
	{
		index--;
		if(index == 0)
		{
			dir = 0;
		}
	}
}

void adjustpwmdr(unsigned dr)               //调整占空比函数
{
	unsigned int high,low;
	high = (count * dr)/100;
	low = count - high;
	high = 65536 - high;
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);
	highTL0 = (unsigned char) high;
	lowTH0 = (unsigned char)(low >> 8);
	lowTL0 = (unsigned char) low;
}