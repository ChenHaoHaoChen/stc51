/*****************************
电度表三大功能及数码管显示程序
*****************************/
#include <reg52.h>
#include "key.h"
extern void EEPROMwritebyte(unsigned char address,unsigned char dat);
extern unsigned int ELEFEE;         //电费,默认为0.52元/度
extern unsigned int ELECON;          //用电量
extern unsigned int FEE;             //总电费
extern unsigned char time[8];
extern unsigned char keyfinally[4];  //按键终值(1为弹起，0为按下)
unsigned char displaybuff[8] = {  
	0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF
};
unsigned char code number[20] = {  //共阳数码管显示译码
	0xc0,0xf9,0xa4,0xb0,0x99,        //不显示小数点0~9
	0x92,0x82,0xf8,0x80,0x90, 
	0x40,0x79,0x24,0x30,0x19,        //显示小数点0~9
	0x12,0x02,0x78,0x00,0x10
};
/**********启动状态数码管显示**********/
void startdisplay()
{
	if(bit_start == 1 && bit_extra == 1)
	{
		displaybuff[7] = number[ELEFEE/100%10 + 10];  //电费显示
		displaybuff[6] = number[ELEFEE/10%10];
		displaybuff[5] = number[ELEFEE%10];
		displaybuff[4] = 0XFF;  	                                               
		if(ELECON/1000%10 == 0)                       //用电量显示＋高位灭零                       
		{
			displaybuff[3] = 0XFF;	
		}
		else
		{
			displaybuff[3] = number[ELECON/1000%10];
		}
		if((ELECON/100%10 == 0) && (ELECON/1000%10 == 0))
		{
			displaybuff[2] = 0XFF;	
		}
		else
		{
			displaybuff[2] = number[ELECON/100%10];
		}
		displaybuff[1] = number[ELECON/10%10 + 10];
		displaybuff[0] = number[ELECON%10];
	}
}
/**********时钟状态数码管显示**********/
void timedisplay()
{
	if(bit_time == 1 && bit_extra == 1)
	{
		displaybuff[7] = number[time[2] >> 4];
		displaybuff[6] = number[time[2] & 0X0F];
		displaybuff[5] = 0XBF;
		displaybuff[4] = number[time[1] >> 4];
		displaybuff[3] = number[time[1] & 0X0F];
		displaybuff[2] = 0XBF;
		displaybuff[1] = number[time[0] >> 4];
		displaybuff[0] = number[time[0] & 0X0F];
	}
}
/**********电费结算数码管显示**********/
void enddisplay()
{
	if(bit_end == 1 && bit_extra == 1)
	{
		FEE = ELEFEE * ELECON;
		if(ELECON/100%10 == 0)
		{
			 displaybuff[7] = 0XFF;
		}
		else
		{
			 displaybuff[7] = number[ELECON/100%10];
		}
		displaybuff[6] = number[ELECON/10%10 + 10];
		displaybuff[5] = number[ELECON%10];
		displaybuff[4] = 0XFF;
		if(FEE/10000%10 == 0)
		{
			displaybuff[3] = 0XFF;
		}
		else
		{
			displaybuff[3] = number[FEE/10000%10];
		}
		displaybuff[2] = number[FEE/1000%10 + 10];
		displaybuff[1] = number[FEE/100%10];
		displaybuff[0] = number[FEE/10%10];
		ELECON = 0;    //用电量清零
		bit_end = 0;   //end标志位置0，下次不刷新，否则电费结算只显示1ms，即全为0
	}
}
/**********修改电费数码管显示**********/
void changedisplay()
{
	if(ELECON == 0 && bit_change == 1)
	{
		bit_extra = 0;
		displaybuff[0] = 0XBF;
		displaybuff[1] = 0XBF;
		displaybuff[2] = 0XBF;
		displaybuff[3] = 0XBF;
		displaybuff[4] = 0XFF;
		displaybuff[5] = number[ELEFEE%10];
		displaybuff[6] = number[ELEFEE/10%10];
		displaybuff[7] = number[ELEFEE/100%10 + 10];
	}
	else if(ELECON == 0 && bit_change == 0 && bit_extra == 0)
	{
		EEPROMwritebyte(0X03,ELEFEE);
		bit_extra = 1;
	}
}
/**********数码管显示函数**********/
void display()
{
	static unsigned char i = 0;
	P0 = 0XFF;  //显示消隐
	switch(i)
	{ 
		case 0: P0 = displaybuff[0]; P1 = 0X7F; i++; break;
		case 1: P0 = displaybuff[1]; P1 = 0XBF; i++; break;
		case 2: P0 = displaybuff[2]; P1 = 0XDF; i++; break;
		case 3: P0 = displaybuff[3]; P1 = 0XEF; i++; break;
		case 4: P0 = displaybuff[4]; P1 = 0XF7; i++; break;
		case 5: P0 = displaybuff[5]; P1 = 0XFB; i++; break;
		case 6: P0 = displaybuff[6]; P1 = 0XFD; i++; break;
		case 7: P0 = displaybuff[7]; P1 = 0XFE; i=0; break;
		default: break;
	}
}