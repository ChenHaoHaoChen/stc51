/******************
D/A是数模转换，
设置输出电压，即将设置好的电压刻度值转换为电压，通过AOUT口输出
******************/
#include <reg52.h>
unsigned char TH0R;
unsigned char TL0R;

extern void keydriver();
extern void keyscan();
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();

void configtime0(unsigned char ms);

void main()
{
	EA = 1;
	configtime0(1);//定时器T0定时1ms
	while(1)
	{
		keydriver();
	}
}
/*设置DA输出函数*/
void setDAout(unsigned char val)
{
	I2Cstart();
	I2Cwrite(0X90);
	I2Cwrite(0X40);
	I2Cwrite(val);
	I2Cstop();
}
/*按键动作函数*/
void keyaction(unsigned char keycode)
{
	static unsigned char val = 0;
	if(keycode == 10)
	{
		if(val <= 25)
		{
			val++;
			setDAout(val*255/25);
		}
	}
	else if(keycode == 11)
	{
		if(val >= 0)
		{
			val--;
			setDAout(val*255/25);
		}
	}
}
/*定时器T0配置函数*/
void configtime0(unsigned char ms)
{
	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	TH0R = (unsigned char)(tmp >> 8);
	TL0R = (unsigned char)(tmp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*定时器T0中断*/
void interrupttime0() interrupt 1
{
	TH0 = TH0R;
	TL0 = TL0R;
	keyscan();
}
