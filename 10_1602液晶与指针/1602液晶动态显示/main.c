#include <reg52.h>
#define LCD1602_DB P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E = P1^5;

bit flag500ms = 0;
unsigned char TH0R;
unsigned char TL0R;
	unsigned char code str1[]={
		"                CHH And ZYJ                "
	};
	unsigned char code str2[]={
		"                2021 /8/ 18                "
	};

void configtime(unsigned int ms);
void LCD1602init();
void LCD1602writecmd(unsigned char cmd);
void LCD1602readstate();
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);
void LCD1602setcoor(unsigned char x,unsigned char y);
void LCD1602writedata(unsigned char dat);

void main()
{
	unsigned char index = 0;
	EA = 1;
	LCD1602init();
	configtime(10);
	while(1)
	{
		if(flag500ms)   //每500ms刷新一次
		{
			flag500ms = 0;
			LCD1602show(0,0,str1+index); //index为刷新指针索引
	    LCD1602show(0,1,str2+index);
			index++;
			if(index == sizeof(str1))
			{
				index = 0;
			}
		}
	}
}

void configtime(unsigned int ms)
{
	unsigned long temp;
	temp = 65536 - (11059200/12*ms/1000);
	TH0R = (unsigned char)(temp >> 8);
	TL0R = (unsigned char) temp;
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}

void interrupttime0() interrupt 1
{
	static unsigned char i = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	i++;
	if(i == 50)
	{
		i = 0;
		flag500ms = 1;
	}
}

void LCD1602init()
{
	LCD1602writecmd(0X38);
	LCD1602writecmd(0X0C);
	LCD1602writecmd(0X06);
	LCD1602writecmd(0X01);
}

void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 0;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

void LCD1602readstate()
{
	unsigned char sta7;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do
	{
		LCD1602_E = 1;
		sta7 = LCD1602_DB;
		LCD1602_E = 0;
	}
	while(sta7 & 0X80);
}

void LCD1602show(unsigned char x,unsigned char y,unsigned char *p)
{
	LCD1602setcoor(x,y);
	while(*p != '\0')
	{
		LCD1602writedata(*p++);
	}
}

void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add;
	if(y == 0)
	{
		add = 0X00 + x;
	}
	else 
	{
		add = 0X40 + x;
	}
	LCD1602writecmd(add | 0X80);
}

void LCD1602writedata(unsigned char dat)
{
	LCD1602readstate();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 0;
	LCD1602_E = 1;
	LCD1602_E = 0;
}