/*****************************
电度表三大功能及显示程序
*****************************/
#include <reg52.h>
#include "key.h"
#include "main.h"
#include "EEPROM.h"
#include "LCD1602.h"

unsigned char strstart[16]; //启动状态显示数组
unsigned char strtime[16];  //时间状态显示数组
unsigned char strend[16];   //结费状态显示数组
unsigned char strchange[16];//修改电费状态显示数组

/**********启动状态显示**********/
void startdisplay()
{
	static unsigned char LCD1602buff[2];
	if(bit_start == 1 && bit_extra == 1)
	{
		L1 = 0;  //启动指示灯
		if(ELECON/1000%10 == 0)  //用电量高位灭零                                         
		{
			LCD1602buff[0] = ' ';	
		}
		else
		{
			LCD1602buff[0] = ELECON/1000%10 + '0';
		}
		if((ELECON/100%10 == 0) && (ELECON/1000%10 == 0))
		{
			LCD1602buff[1] = ' ';	
		}
		else
		{
			LCD1602buff[1] = ELECON/100%10 + '0';
		}
		
		LCD1602show(0,0,"DanJia:    Used:");  //LCD1602第1行显示"DanJia:    Used:"
		
		strstart[0] = ELEFEE/100%10 + '0';
		strstart[1] = '.';
		strstart[2] = ELEFEE/10%10 + '0';
		strstart[3] = ELEFEE%10 + '0';
		strstart[4] = 'Y';
		strstart[5] = '/';
		strstart[6] = 'D';
		strstart[7] = ' ';
		strstart[8] = ' ';
		strstart[9] = ' ';
		strstart[10] = LCD1602buff[0];
		strstart[11] = LCD1602buff[1];
		strstart[12] = ELECON/10%10 + '0';
		strstart[13] =  '.';
		strstart[14] = ELECON%10 + '0';
		strstart[15] = 'D';
		strstart[16] = '\0';
		LCD1602show(0,1,strstart);  //LCD1602第2行电费及用电量显示
		
		ELECONaccident();
	}
}
/**********时钟状态显示**********/
void timedisplay()
{
	if(bit_time == 1 && bit_extra == 1)
	{
		L1 = 0;  //启动指示灯
		strtime[0] = ' ';
		strtime[1] = ' ';
		strtime[2] = ' ';
		strtime[3] = '2';
		strtime[4] = '0';
		strtime[5] = (time[6] >> 4) + '0';
		strtime[6] = (time[6] & 0X0F) + '0';
		strtime[7] = '-';
		strtime[8] = (time[4] >> 4) + '0';
		strtime[9] = (time[4] & 0X0F) + '0';
		strtime[10] = '-';
		strtime[11] = (time[3] >> 4) + '0';
		strtime[12] = (time[3] & 0X0F) + '0';
		strtime[13] = ' ';
		strtime[14] = ' ' ;
		strtime[15] = ' ';
		strtime[15] = '\0';
    LCD1602show(0,0,strtime);  //LCD1602第1行年月日显示
		
		strtime[0] = ' ';
		strtime[1] = ' ';
		strtime[2] = ' ';
		strtime[3] = ' ';
		strtime[4] = (time[2] >> 4) + '0';
		strtime[5] = (time[2] & 0X0F) + '0';
		strtime[6] = ':';
		strtime[7] = (time[1] >> 4) + '0';
		strtime[8] = (time[1] & 0X0F) + '0';
		strtime[9] = ':';
		strtime[10] = (time[0] >> 4) + '0';
		strtime[11] = (time[0] & 0X0F) + '0';
		strtime[12] = ' ';
		strtime[13] = ' ';
		strtime[14] = ' ';
		strtime[15] = ' ';
		strtime[16] = '\0';
		LCD1602show(0,1,strtime);  //LCD1602第2行时分秒显示
		
		ELECONaccident();
	}
}
/**********电费结算显示**********/
void enddisplay()
{
	static unsigned char LCD1602buff[3];
	if(bit_end == 1 && bit_extra == 1)
	{
		L1 = 1;  //关闭指示灯
		LCD1602clearscreen();
		FEE = ELEFEE * ELECON;
		if(ELECON/100%10 == 0)  //用电量高位灭零
		{
			LCD1602buff[0] = ' ';
		}
		else
		{
			LCD1602buff[0] = ELECON/100%10 + '0';
		}
		
		if(FEE/10000%10 == 0)   //电费总价高位灭零                                        
		{
			LCD1602buff[1] = ' ';	
		}
		else
		{
			LCD1602buff[1] = FEE/10000%10 + '0';
		}
		
		LCD1602show(0,0,"XiaoHao:ZongJia:");  //LCD1602第1行显示"XiaoHao:ZongJia:"
		
		strend[0] = LCD1602buff[0];
		strend[1] = ELECON/10%10 + '0';
		strend[2] = '.';
		strend[3] = ELECON%10 + '0';
		strend[4] = 'D';
		strend[5] = ' ';
		strend[6] = ' ';
		strend[7] = ' ';
		strend[8] = ' ';
		strend[9] = ' ';
		strend[10] = LCD1602buff[1];
		strend[11] = FEE/1000%10 + '0';
		strend[12] = '.';
		strend[13] = FEE/100%10 + '0';
		strend[14] = FEE/10%10 + '0';
		strend[15] = 'Y';
		strend[16] = '\0';
		LCD1602show(0,1,strend);  //LCD1602第二行显示用电量及总价
		ELECON = 0;    //用电量清零
		bit_end = 0;   //bit_end标志位置0，下次不刷新，否则电费结算只显示1ms，即全为0
 	}
}
/**********修改电费显示**********/
void changedisplay()
{
	if(ELECON == 0 && bit_change == 1)
	{
		L1 = 1;  //关闭指示灯
		bit_extra = 0;
		LCD1602show(0,0,"XiuGaiDianFei:");  //LCD1602第1行显示"XiuGaiDianFei:"
		strchange[0] = ELEFEE/100%10 + '0';
		strchange[1] = '.';
		strchange[2] = ELEFEE/10%10 + '0';
		strchange[3] = ELEFEE%10 + '0';
		strchange[4] = 'Y';
		strchange[5] = '\0';
		LCD1602show(0,1,strchange);
	}
	else if(ELECON == 0 && bit_change == 0 && bit_extra == 0)
	{
		EEPROMwritebyte(0X04,ELEFEE);  //保存修改后电费至EEPROM中
		bit_extra = 1;
	}
}