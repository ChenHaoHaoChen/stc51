/*****************************
��ȱ������ܼ���ʾ����
*****************************/
#include <reg52.h>
#include "key.h"
#include "main.h"
#include "EEPROM.h"
#include "LCD1602.h"

unsigned char strstart[16]; //����״̬��ʾ����
unsigned char strtime[16];  //ʱ��״̬��ʾ����
unsigned char strend[16];   //���״̬��ʾ����
unsigned char strchange[16];//�޸ĵ��״̬��ʾ����

/**********����״̬��ʾ**********/
void startdisplay()
{
	static unsigned char LCD1602buff[2];
	if(bit_start == 1 && bit_extra == 1)
	{
		L1 = 0;  //����ָʾ��
		if(ELECON/1000%10 == 0)  //�õ�����λ����                                         
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
		
		LCD1602show(0,0,"DanJia:    Used:");  //LCD1602��1����ʾ"DanJia:    Used:"
		
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
		LCD1602show(0,1,strstart);  //LCD1602��2�е�Ѽ��õ�����ʾ
		
		ELECONaccident();
	}
}
/**********ʱ��״̬��ʾ**********/
void timedisplay()
{
	if(bit_time == 1 && bit_extra == 1)
	{
		L1 = 0;  //����ָʾ��
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
    LCD1602show(0,0,strtime);  //LCD1602��1����������ʾ
		
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
		LCD1602show(0,1,strtime);  //LCD1602��2��ʱ������ʾ
		
		ELECONaccident();
	}
}
/**********��ѽ�����ʾ**********/
void enddisplay()
{
	static unsigned char LCD1602buff[3];
	if(bit_end == 1 && bit_extra == 1)
	{
		L1 = 1;  //�ر�ָʾ��
		LCD1602clearscreen();
		FEE = ELEFEE * ELECON;
		if(ELECON/100%10 == 0)  //�õ�����λ����
		{
			LCD1602buff[0] = ' ';
		}
		else
		{
			LCD1602buff[0] = ELECON/100%10 + '0';
		}
		
		if(FEE/10000%10 == 0)   //����ܼ۸�λ����                                        
		{
			LCD1602buff[1] = ' ';	
		}
		else
		{
			LCD1602buff[1] = FEE/10000%10 + '0';
		}
		
		LCD1602show(0,0,"XiaoHao:ZongJia:");  //LCD1602��1����ʾ"XiaoHao:ZongJia:"
		
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
		LCD1602show(0,1,strend);  //LCD1602�ڶ�����ʾ�õ������ܼ�
		ELECON = 0;    //�õ�������
		bit_end = 0;   //bit_end��־λ��0���´β�ˢ�£������ѽ���ֻ��ʾ1ms����ȫΪ0
 	}
}
/**********�޸ĵ����ʾ**********/
void changedisplay()
{
	if(ELECON == 0 && bit_change == 1)
	{
		L1 = 1;  //�ر�ָʾ��
		bit_extra = 0;
		LCD1602show(0,0,"XiuGaiDianFei:");  //LCD1602��1����ʾ"XiuGaiDianFei:"
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
		EEPROMwritebyte(0X04,ELEFEE);  //�����޸ĺ�����EEPROM��
		bit_extra = 1;
	}
}