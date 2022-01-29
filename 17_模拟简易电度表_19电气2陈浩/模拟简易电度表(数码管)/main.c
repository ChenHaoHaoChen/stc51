/*******************************************************
               模拟简易电度表
							 
*         19电气2 陈浩 2019314230		

*  LED1501(照明灯)：     P3.2						 
*  按键控制(K17~K20)：   P3.4~P3.7;
*  数码管显示(八位共阳)：位码(P0),段码(P1);
*  EEPROM(24C02)：       SCL(P2.6),SDA(P2.7);
*  A/D转换(TLC549)：     CLK(P2.0),DO(P2.1),CS(P2.2);
*  实时时钟(DS1302)：    RST(P2.3),SCLK(P2.4),I/O(P2.5);
*******************************************************/
#include <reg52.h>        //默认头文件
#include "DS1302.h"       //DS1302配置头文件
#include "TLC549.h"       //TLC549配置头文件
#include "key.h"          //按键配置头文件
#include "EEPROM.h"       //EEPROM配置头文件
#include "T0T1.h"         //定时器T0T1配置头文件
#include "display.h"      //三大功能及数码管显示配置头文件

unsigned int ELEFEE = 58;      //电费,默认为0.52元/度
unsigned int ELECON = 0;  //用电量
unsigned int FEE = 0;     //总电费
unsigned char time[8];    //DS1302读取时间缓冲数组

/**********主函数**********/
void main()
{
  EA = 1;             //开总中断
	configtime0(1);     //配置T0定时器定时1ms
	configtime1(2);     //配置T1定时器定时2ms
	DS1302init();       //DS1302初始化
	EEPROMinit();       //EEPROM初始化
	P3 = P3 | 0XF0;     //拉高四个按键电平
	while(1)
	{
		keyscan();    //按键扫描
		ADscan();     //AD扫描
		DS1302burstread(time); //DS1302burst模式读时间
	}
}
/**********T0定时器中断函数**********/
void interruptt0() interrupt 1
{
	TH0 = TH0R;     //T0定时器重新定时1ms
	TL0 = TL0R;
	startdisplay();
	timedisplay();
	enddisplay();
	changedisplay();
	display();
}
/**********电费自加函数(每1s加0.1度)**********/
void ELECONadd()
{
	static unsigned int bit_1s = 0;
	if( (bit_start == 1) || (bit_time == 1))
	{
		bit_1s++;
		if(bit_1s == 500)
		{
			bit_1s = 0;
			ELECON++;
		}
	}
}
/**********T1定时器中断函数**********/
void interruptt1() interrupt 3
{
	TH1 = TH1R;    //T1定时器重新定时2ms
	TL1 = TL1R;
	keybuff();
	ELECONadd();
	ADrefresh();
}