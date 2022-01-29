/******************
D/A����ģת����
���������ѹ���������úõĵ�ѹ�̶�ֵת��Ϊ��ѹ��ͨ��AOUT�����
******************/
#include <reg52.h>
unsigned char TH0R;
unsigned char TL0R;

extern void keydriver();
extern void keyscan();
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();

void configtime0(unsigned char ms);

void main()
{
	EA = 1;
	configtime0(1);//��ʱ��T0��ʱ1ms
	while(1)
	{
		keydriver();
	}
}
/*����DA�������*/
void setDAout(unsigned char val)
{
	I2Cstart();
	I2Cwrite(0X90);
	I2Cwrite(0X40);
	I2Cwrite(val);
	I2Cstop();
}
/*������������*/
void keyaction(unsigned char keycode)
{
	static unsigned char val = 0;
	if(keycode == 10)
	{
		if(val <= 25)
		{
			val++;
			setDAout(val*255/25);
		}
	}
	else if(keycode == 11)
	{
		if(val >= 0)
		{
			val--;
			setDAout(val*255/25);
		}
	}
}
/*��ʱ��T0���ú���*/
void configtime0(unsigned char ms)
{
	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	TH0R = (unsigned char)(tmp >> 8);
	TL0R = (unsigned char)(tmp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*��ʱ��T0�ж�*/
void interrupttime0() interrupt 1
{
	TH0 = TH0R;
	TL0 = TL0R;
	keyscan();
}
