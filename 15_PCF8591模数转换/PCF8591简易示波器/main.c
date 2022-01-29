#include <reg52.h>
extern void keydriver();
extern void keyscan();
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();

void configtime0(unsigned char ms);
void configfreq1(unsigned int freq);

unsigned char code sinewave[32] = {      //正弦波波表
    127, 152, 176, 198, 217, 233, 245, 252,
    255, 252, 245, 233, 217, 198, 176, 152,
    127, 102,  78,  56,  37,  21,   9,   2,
      0,   2,   9,  21,  37,  56,  78, 102,
};
unsigned char code trianglewave[32] = {  //三角波波表
      0,  16,  32,  48,  64,  80,  96, 112,
    128, 144, 160, 176, 192, 208, 224, 240,
    255, 240, 224, 208, 192, 176, 160, 144,
    128, 112,  96,  80,  64,  48,  32,  16,
};
unsigned char code sawtoothwave[32] = {  //锯齿波表
      0,   8,  16,  24,  32,  40,  48,  56,
     64,  72,  80,  88,  96, 104, 112, 120,
    128, 136, 144, 152, 160, 168, 176, 184,
    192, 200, 208, 216, 224, 232, 240, 248,
};
unsigned char TH0R;
unsigned char TL0R;
unsigned char TH1R;
unsigned char TL1R;
unsigned char *p;

void main()
{
	EA = 1;
	configtime0(1);
	configfreq1(10);
	p = sinewave;
	while(1)
	{
		keydriver();
	}
}
/*按键动作函数*/
void keyaction(unsigned char keycode)
{
	static unsigned char i = 0;
	if(keycode == 10)
	{
		if(i == 0)
		{
			p = sinewave;
			i = 1;
		}
		else if(i == 1)
		{
			p = trianglewave;
			i = 2;
		}
		else if(i == 2)
		{
			p = sawtoothwave;
			i = 0;
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
/*定时器T1配置波形频率函数*/
void configfreq1(unsigned int freq)
{
	unsigned long tmp;
	tmp = (11059200/12)/(freq*32);
	tmp = 65536 - tmp;
	TH1R = (unsigned char)(tmp >> 8);
	TL1R = (unsigned char)(tmp);
	TMOD = TMOD & 0X0F;
	TMOD = TMOD | 0X10;
	TH1 = TH1R;
	TL1 = TL1R;
	TR1 = 1;
	ET1 = 1;
	PT1 = 1;  //设置定时器T1为最高优先级
}
/*设置DA输出电压刻度函数*/
void setDAout(unsigned char val)
{
	I2Cstart();
	I2Cwrite(0X90);
	I2Cwrite(0X40);
	I2Cwrite(val);
	I2Cstop();
}
/*定时器T0中断*/
void interrupttime0() interrupt 1
{
	TH0 = TH0R;
	TL0 = TL0R;
	keyscan();
}
/*定时器T1中断*/
void interrupttime2() interrupt 3
{
	static unsigned char cnt = 0;
	TH1 = TH1R;
	TL1 = TL1R;
	setDAout(p[cnt]);
	cnt++;
	if(cnt >= 32)
	{
		cnt = 0;
	}
}