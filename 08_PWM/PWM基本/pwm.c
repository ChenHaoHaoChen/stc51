#include <reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit PWMOUT = P0^0;

unsigned char highTH0; //PWM����ߵ�ƽ�߰�λ
unsigned char highTL0; //PWM����ߵ�ƽ�ذ�λ
unsigned char lowTH0;  //PWM����͵�ƽ�߰�λ
unsigned char lowTL0;  //PWM����͵�ƽ�Ͱ�λ

void configpwm(unsigned int fr,unsigned char dr); //��������
void closepwm();

void main()
{
	unsigned int i;
	unsigned int j;
	ENLED = 0;           //��74138������
	ADDR3 = 1;
	ADDR2 = 1;           //ѡ��LED�˿�
	ADDR1 = 1;
	ADDR0 = 0;
	TMOD = 0X01;         //���ö�ʱ��Ϊ1ģʽ
	EA = 1;              //���ж�ʹ��
	while(1)
	{
		for(i=99;i>=1;i--)
		{
			configpwm(100,i);
			for(j=0;j<30000;j++);
			closepwm();
		}
		
	}
}

void configpwm(unsigned int fr,unsigned char dr) //����PWM
{
	unsigned long temp;
	unsigned int high,low;
  temp = (11059200/12)/fr;                       //����100HZ����10ms���趨ʱ����
	high = (temp*dr)/100;                          //����ռ�ձȣ�����10ms�иߵ�ƽ�ļ�������
	low = temp - high;                             //����͵�ƽ�ļ�������
	high = 65536 - high;                           //�������������������������
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);          //�ߵ�ƽ�߰�λ�������
	highTL0 = (unsigned char) high;                //�ߵ�ƽ�Ͱ�λ�������
	lowTH0 = (unsigned char)(low >> 8);            //�͵�ƽ�߰�λ�������
	lowTL0 = (unsigned char) low;                  //�͵�ƽ�Ͱ�λ�������
	TR0 = 1;                                       //�򿪶�ʱ��T0
	ET0 = 1;                                       //�򿪶�ʱ��T0�ж�
	PWMOUT = 1;
}
void closepwm()                                  //�ر�PWM����
{
	highTH0 = 0;
  highTL0 = 0;
  lowTH0 = 0;
  lowTL0 = 0;
	TR0 = 0;
	ET0 = 0;
}
void interrupttime0() interrupt 1                //��ʱ��T0�ж�
{
	if(PWMOUT == 1)
	{
		PWMOUT = 0;
		TH0 = lowTH0;
		TL0 = lowTL0;
	}
	else
	{
		PWMOUT = 1;
		TH0 = highTH0;
		TL0 = highTL0;
	}
}


