/**************************
    定时器T0T1定时程序
**************************/
#include <reg52.h>
unsigned char TH0R,TL0R;          //T0高低八位重载值
unsigned char TH1R,TL1R;          //T1高低八位重载值

/**********T0定时器配置函数**********/
void configtime0(unsigned char ms)
{
	unsigned int temp;
	temp = 65536 - 1000*ms;
	TH0R = (unsigned char)(temp >> 8);
	TL0R = (unsigned char)(temp);
	TH0 = TH0R;
	TL0 = TL0R;
	TMOD = 0X01 | TMOD;
	TR0 = 1;
	ET0 = 1;
}
/**********T1定时器配置函数**********/
void configtime1(unsigned char ms)
{
	unsigned int temp;
	temp = 65536 - 1000*ms;
	TH1R = (unsigned char)(temp >> 8);
	TL1R = (unsigned char)(temp);
	TH1 = TH0R;
	TL1 = TL0R;
	TMOD = 0X10 | TMOD;
	TR1 = 1;
	ET1 = 1;
}