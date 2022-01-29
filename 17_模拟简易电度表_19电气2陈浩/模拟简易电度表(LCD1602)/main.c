/*******************************************************
*            模拟简易电度表(LCD1602显示)
							 
*            19电气2   陈浩   2019314230

*  L1(指示灯)：          P1.3;
*  LED1501(照明灯)：     P1.4;
*  按键控制(K17~K20)：   P3.4~P3.7;
*  EEPROM(AT24C02)：     SCL(P2.6),SDA(P2.7);
*  A/D转换(TLC549)：     CLK(P2.0),DO(P2.1),CS(P2.2);
*  实时时钟(DS1302)：    RST(P2.3),SCLK(P2.4),I/O(P2.5);
*  LCD1602显示：         DB(P0),RS(P1.0),RW(P1.1),EN(P1.2);
*******************************************************/
#include <reg52.h>           //默认头文件
#include "key.h"             //按键配置头文件
#include "DS1302.h"          //DS1302配置头文件
#include "EEPROM.h"          //EEPROM配置头文件
#include "TLC549.h"          //TLC549配置头文件
#include "LCD1602.h"         //LCD1602配置头文件
#include "T0T1.h"            //定时器T0T1配置头文件
#include "display.h"         //三大功能及数码管显示配置头文件

unsigned int ELEFEE;         //电费,默认为0.52元/度
unsigned int ELECON = 0;     //用电量
unsigned int FEE = 0;        //总电费
unsigned char time[8];       //DS1302读取时间暂存数组
bit flag_100ms = 0;          //100ms刷新标志位

/**********主函数**********/
void main()
{
  EA = 1;                    //开总中断
	configtime0(1);            //配置T0定时器定时1ms
	configtime1(2);            //配置T1定时器定时2ms
	DS1302init();              //DS1302初始化
	EEPROMinit();              //EEPROM初始化
	LCD1602init();             //LCD1602初始化
	LCD1602show(3,0,"MoNiJianYi");  //开机显示字符页面
	LCD1602show(3,1,"DianDuBiao");
	P3 = P3 | 0XF0;            //按键初始化：拉高四个按键电平
	while(1)
	{
		keyscan();               //按键扫描
		ADscan();                //AD扫描
		if(flag_100ms == 1)      //执行周期为100ms
		{
			flag_100ms = 0;
			DS1302burstread(time); //DS1302burst模式读时间
			startdisplay();        //开始计费显示
			timedisplay();         //时间状态显示
			enddisplay();          //结算电费显示
			changedisplay();       //修改电费显示
		}
	}
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
/**********T0定时器中断函数**********/
void interruptt0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;    //T0定时器重新定时1ms
	TL0 = TL0R;
	ADrefresh();   //AD采集电压
	cnt++;
	if(cnt == 100)
	{
		flag_100ms = 1;
		cnt = 0;
	}
}
/**********T1定时器中断函数**********/
void interruptt1() interrupt 3
{
	TH1 = TH1R;    //T1定时器重新定时2ms
	TL1 = TL1R;
	keybuff();     //按键中断消抖
	ELECONadd();   //电费自加函数0.1度/秒
}

