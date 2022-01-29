#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

extern unsigned char irflag;        //�ⲿ��������
extern unsigned char ircode[4];
extern void Infraredinit();
void configtime0(unsigned char ms);//��������

unsigned char TH0R;                //��ʱ��T0��λ����ֵ
unsigned char TL0R;                //��ʱ��T0��λ����ֵ
unsigned char ledbuffer[6]={       //�������ʾ������
	0XFF,0XFF,0XFF,0XFF,0XFF,0XFF
  };
unsigned char code number[] = {    //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

void main()
{
	EA = 1;         //�����ж�
	ADDR3 = 1;      //��74HC138ʹ�ܶ�
	ENLED  = 0;
	PT0 = 1;        //����T0��ʱ���ж�Ϊ������ȼ�
	configtime0(1); //��ʱ1ms
	Infraredinit(); //NEC����ͨ�ų�ʼ��
	while(1)
	{
		if(irflag == 1)
		{
			irflag = 0;
			ledbuffer[0] = number[ircode[2] & 0X0F];
			ledbuffer[1] = number[ircode[2] >>4];
			ledbuffer[4] = number[ircode[0] & 0X0F];
			ledbuffer[5] = number[ircode[0] >> 4];
		}
	}
}
/*���ö�ʱ��T0��ʱ1ms*/
void configtime0(unsigned char ms)
{
	unsigned long temp;
	temp = 11059200/12;
	temp = (temp*ms)/1000;
	temp = 65536 - temp;
	TH0R = (unsigned char)(temp >> 8);
	TL0R = (unsigned char)(temp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*�����ɨ�躯��*/
void ledscan()
{
	static unsigned char i = 0;
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i=0;break;
		default:break;
	}
}

void interrupttime0() interrupt 1
{
	TH0 = TH0R;
	TL0 = TL0R;
	ledscan();
}