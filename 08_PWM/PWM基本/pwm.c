#include <reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit PWMOUT = P0^0;

unsigned char highTH0; //PWM输出高电平高八位
unsigned char highTL0; //PWM输出高电平地八位
unsigned char lowTH0;  //PWM输出低电平高八位
unsigned char lowTL0;  //PWM输出低电平低八位

void configpwm(unsigned int fr,unsigned char dr); //函数声明
void closepwm();

void main()
{
	unsigned int i;
	unsigned int j;
	ENLED = 0;           //打开74138译码器
	ADDR3 = 1;
	ADDR2 = 1;           //选择LED端口
	ADDR1 = 1;
	ADDR0 = 0;
	TMOD = 0X01;         //配置定时器为1模式
	EA = 1;              //打开中断使能
	while(1)
	{
		for(i=99;i>=1;i--)
		{
			configpwm(100,i);
			for(j=0;j<30000;j++);
			closepwm();
		}
		
	}
}

void configpwm(unsigned int fr,unsigned char dr) //配置PWM
{
	unsigned long temp;
	unsigned int high,low;
  temp = (11059200/12)/fr;                       //计算100HZ，即10ms所需定时个数
	high = (temp*dr)/100;                          //根据占空比，计算10ms中高电平的计数个数
	low = temp - high;                             //计算低电平的计数个数
	high = 65536 - high;                           //由所需计数个数来计算计数起点
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);          //高电平高八位计数起点
	highTL0 = (unsigned char) high;                //高电平低八位计数起点
	lowTH0 = (unsigned char)(low >> 8);            //低电平高八位计数起点
	lowTL0 = (unsigned char) low;                  //低电平低八位计数起点
	TR0 = 1;                                       //打开定时器T0
	ET0 = 1;                                       //打开定时器T0中断
	PWMOUT = 1;
}
void closepwm()                                  //关闭PWM函数
{
	highTH0 = 0;
  highTL0 = 0;
  lowTH0 = 0;
  lowTL0 = 0;
	TR0 = 0;
	ET0 = 0;
}
void interrupttime0() interrupt 1                //定时器T0中断
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


