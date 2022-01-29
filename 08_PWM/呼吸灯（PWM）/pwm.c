#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit PWMOUT = P0^0;
unsigned char highTH0 = 0;  //ÿһ���ڸߵ�ƽ��ʱʱ��
unsigned char highTL0 = 0;
unsigned char lowTH0 = 0;   //ÿһ���ڵ͵�ƽ��ʱʱ��
unsigned char lowTL0 = 0;
unsigned char TH1R = 0;     //����ÿ����ms�����ж�1
unsigned char TL1R = 0;
unsigned long count = 0;    //����һ�������������ֵ
void configpwm(unsigned char fr,unsigned char dr); //��������
void configtime(unsigned int ms);
void adjustpwmdr(unsigned dr);

void main()
{
	ENLED = 0;                //��74138������
	ADDR3 = 1;
	ADDR2 = 1;                //������С�ƶ˿�
	ADDR1 = 1;
	ADDR0 = 0;
	EA = 1;                   //���ж�ʹ��
	TMOD = 0X01;              //���ö�ʱ��Ϊ1ģʽ
	configpwm(100,5);         //����  
	configtime(50);
	while(1);
}

void configpwm(unsigned char fr,unsigned char dr) //����PWM��frΪƵ�ʣ�drΪռ�ձ�
{
	unsigned int high,low;
	count = (11059200/12)/fr;
	high = (count*dr)/100;
	low = count - high;
	high = 65536 - high;
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);
	highTL0 = (unsigned char) high;
	lowTH0 = (unsigned char)(low >> 8);
	lowTL0 = (unsigned char) low;
	TH0 = highTH0;
	TL0 = highTL0;
	TR0 = 1;
	ET0 = 1;
	PWMOUT = 1;
}

void configtime(unsigned int ms)                 //���ò�����T1,�����ǣ�ÿms����ˢ��һ��
{
	unsigned long temp;
	temp =65536 - (11059200/12)*(ms/1000);
	TH1R = (unsigned char)(temp >> 8);
	TL1R = (unsigned char) temp;
	TH1 = TH1R;
	TL1 = TL1R;
	TR1= 1;
	ET1 = 1;
}

void interrupttime0() interrupt 1               //����һ���ж�0�ı�һ������ĸߵ͵�ƽ
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

void interrupttiime1() interrupt 3              //����һ���ж�1������һ��ռ�ձ�
{
	static unsigned char table[]={
		5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95
	};
	static unsigned char dir = 0;
	static unsigned char index = 0;
	TH1 = TH1R;
	TL1 = TL1R;
	adjustpwmdr(table[index]);
	if(dir == 0)
	{
		index++;
		if(index == 18)
		{
			dir = 1;
		}
	}
	else
	{
		index--;
		if(index == 0)
		{
			dir = 0;
		}
	}
}

void adjustpwmdr(unsigned dr)               //����ռ�ձȺ���
{
	unsigned int high,low;
	high = (count * dr)/100;
	low = count - high;
	high = 65536 - high;
	low = 65536 - low;
	highTH0 = (unsigned char)(high >> 8);
	highTL0 = (unsigned char) high;
	lowTH0 = (unsigned char)(low >> 8);
	lowTL0 = (unsigned char) low;
}