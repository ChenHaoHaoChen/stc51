//���ڵ����Ӱ�Լ�LED��Ӱ�������ж�0���ж�1�ֱ���1ms��4msˢ�£�
//���ж�0ˢ��ʱ����ж�1���Ƶĵ������Ӱ��
//���ж�1ˢ��ʱ����ж�0���Ƶ�����ܺ�LED����Ӱ��
//���Ľ���2021.7.23
#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned int cnt = 0;
unsigned long sec = 0;
unsigned char code number[]={        //�������ʾ����
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char buffer[]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff //�������ʾ������
  };
unsigned char image[]={              //������ʾ����
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xC3,0xE7,0xE7,0xE7,0xE7,0xE7,0xC3,
  0xFF,0x99,0x00,0x00,0x00,0x81,0xC3,0xE7,
	0xFF,0x99,0x99,0x99,0x99,0x99,0x81,0xC3,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
 };
void main()
{
	static unsigned char j1 = 0;       //j����LED��ʾ��λ����                       
	EA = 1;                            //��T0��T1�ж�
	ET0 = 1;
	ET1 = 1;
	
	TMOD = 0x01;
	TH0 = 0xfc;                        //T0��ʱ������ʱ1ms
	TL0 = 0x67;
	TR0 = 1;
	
	TH1 = 0xf1;                        //T1��ʱ������ʱ4ms
	TL1 = 0x9a;
	TR1 = 1;
	
	ENLED = 0;                         //74138�͵�ƽʹ�ܶ˴�
	
	while(1)
	{
		if(cnt == 1000)                  //��1ms��������cnt=1000,��1sʱ����buffer�����������1-6��ŵ�����
		{
			cnt = 0;
			sec++;
			buffer[0] = number[sec%10];
			buffer[1] = number[sec/10%10];
			buffer[2] = number[sec/100%10];
			buffer[3] = number[sec/1000%10];
			buffer[4] = number[sec/10000%10];
			buffer[5] = number[sec/100000%10];
			buffer[6] = ~(0x01<<j1);      //led��ʾ����
			j1++;
			if(j1 == 8)
			{
				j1 = 0;
			}
		}
	}
}

void interrupttime0() interrupt 1  //�ж�0������ʾ����ܺ�LED��1msˢ��һ��
{
	static unsigned char i1 = 0;     //��̬�������ν��븳ֵ0���´ν��벻�ٸ�ֵ0����ֱ������
	ADDR3 = 1;
	TH0 = 0xfc;
	TL0 = 0x67;
	P0 = 0xff;
	cnt++;
	switch(i1)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = buffer[0];i1++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = buffer[1];i1++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = buffer[2];i1++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = buffer[3];i1++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = buffer[4];i1++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = buffer[5];i1++;break;
		case 6:ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;P0 = buffer[6];i1=0;break;
	}
}

void interrupttime1() interrupt 3  //�ж�1������ʾ����4msˢ��һ��
{
	static unsigned char i2 = 0;     //��̬�������ν��븳ֵ0���´ν��벻�ٸ�ֵ0����ֱ������
	static unsigned char j2 = 0;
	static unsigned char tms = 0;
	ADDR3 = 0;
	TH1 = 0xf1;
	TL1 = 0x9a;
	P0 = 0xff;
	switch(i2)
	{
		case 0 :ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = image[j2+0];i2++;break;
		case 1 :ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = image[j2+1];i2++;break;
		case 2 :ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = image[j2+2];i2++;break;
		case 3 :ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = image[j2+3];i2++;break;
		case 4 :ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = image[j2+4];i2++;break;
		case 5 :ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = image[j2+5];i2++;break;
	  case 6 :ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;P0 = image[j2+6];i2++;break;
		case 7 :ADDR2 = 1;ADDR1 = 1;ADDR0 = 1;P0 = image[j2+7];i2=0;break;
	}
	tms++;
	if(tms == 50)
	{
		j2++;
		if(j2 == 32)
		{
			j2 = 0;
		}
	}
}
	


