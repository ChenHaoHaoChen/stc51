/**************************
 ����ɨ�衢��������������
**************************/
#include <reg52.h>
sbit KEY1 = P3^4;  //K17���������Ƶ�ȱ�����״̬�������ʾ
sbit KEY2 = P3^5;  //K18���������Ƶ�ȱ�ʱ��״̬�������ʾ
sbit KEY3 = P3^6;  //K19���������Ƶ�ȱ��ѽ����������ʾ
sbit KEY4 = P3^7;  //K20����������չ����
bit bit_start = 0;                //��ȱ���״̬�������ʾ��־λ
bit bit_time = 0;                 //��ȱ�ʱ��״̬�������ʾ
bit bit_end = 0;                  //��ȱ���״̬�������ʾ��־λ
bit bit_change = 0;               //��ȱ��޸�״̬�������ʾ��־λ
bit bit_extra = 1;
extern unsigned int ELEFEE;
unsigned char keyfinally[4] = {   //������ֵ(1Ϊ����0Ϊ����)
	1,1,1,1
};
unsigned char keyback[4] = {      //������ʱ��ֵ(1Ϊ����0Ϊ����)
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
	if(i == 4)
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
	}
	else if(i == 3) //����3���޸ĵ���������ʾ
	{
		bit_start = 0;
		bit_time = 0;
		bit_end = 0;
		bit_change = ~bit_change;
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