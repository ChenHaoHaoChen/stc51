/**************************
 ����ɨ�衢��������������
**************************/
#include <reg52.h>
#include "main.h"
#include "LCD1602.h"

bit bit_start = 0; //��ȱ���״̬�������ʾ��־λ
bit bit_time = 0;  //��ȱ�ʱ��״̬�������ʾ��־λ
bit bit_end = 0;   //��ȱ���״̬�������ʾ��־λ
bit bit_change = 0;//��ȱ��޸�״̬�������ʾ��־λ
bit bit_extra = 1; 
unsigned char keyfinally[4] = {  //������������ֵ(1Ϊ����0Ϊ����)
	1,1,1,1
};
unsigned char keyback[4] = {     //������������ʱ��ֵ(1Ϊ����0Ϊ����)
	1,1,1,1
};

void keyaction(unsigned char i);

/**********����ɨ�躯��**********/
void keyscan()
{
	static unsigned char i = 0;
	if(keyfinally[i] != keyback[i])
	{
		if(keyback[i] == 0)
		{
			keyaction(i);
		}
		keyback[i] = keyfinally[i];
	}
	i++;
	if(i == 4)  //ѭ��ɨ���ĸ�����
	{
		i = 0;
	}
}
/**********������������**********/
void keyaction(unsigned char i)
{
	if(i == 0)      //����0������״̬�������ʾ
	{
		bit_start = 1;
		bit_time = 0;
		bit_end = 0;
		LCD1602clearscreen();
	}
	else if(i == 1) //����1��ʱ��״̬�������ʾ
	{
		bit_start = 0;
		bit_time = 1;
		bit_end = 0;
		if(bit_change == 1 && bit_extra == 0) //��Ѽ�һ
		{
			ELEFEE++;
			bit_start = 0;
			bit_time = 0;
			bit_end = 0;
		}
		LCD1602clearscreen();
	}
	else if(i == 2) //����2�����״̬�������ʾ
	{
		bit_start = 0;
		bit_time = 0;
		bit_end = 1;
		if(bit_change == 1 && bit_extra == 0) //��Ѽ�һ
		{
			ELEFEE--;
			bit_start = 0;
			bit_time = 0;
			bit_end = 0;
		}
		LCD1602clearscreen();
	}
	else if(i == 3) //����3���޸ĵ���������ʾ
	{
		bit_start = 0;
		bit_time = 0;
		bit_end = 0;
		bit_change = ~bit_change;
		LCD1602clearscreen();
	}
}
/**********������������**********/
void keybuff()
{
	static unsigned char i;
	static unsigned char keybuffer[] = {
	0XFF,0XFF,0XFF,0XFF
	};
	keybuffer[0] = keybuffer[0] << 1 | KEY1;
	keybuffer[1] = keybuffer[1] << 1 | KEY2;
	keybuffer[2] = keybuffer[2] << 1 | KEY3;
	keybuffer[3] = keybuffer[3] << 1 | KEY4;
  for(i=0;i<4;i++) 
	{
		if(keybuffer[i] == 0XFF)
		{
			keyfinally[i] = 1;
		}
		else if(keybuffer[i] == 0X00)
		{
			keyfinally[i] = 0;
		}
	}
}