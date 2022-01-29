#include <reg52.h>

sbit ADDR0 = P1^0; 
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char code Nixietube[]={             //����������������RAM�У�51ֻ��512�ֽ�RAM������ʹ��code
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, //code�����������Flash��
		0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e  //�����Flash�б������ɸ���
	};

void main()
{
	unsigned char cnt=0;
	unsigned char sec=0;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	
	TMOD = 0x01;
	TH0 = 0xb8;
	TL0 = 0x00;
	TR0 = 1;
	while(1)
	{
		if (TF0 == 1)
		{
			TH0 = 0xb8;
			TL0 = 0x00;
			TF0 = 0;
			cnt++;
			if (cnt == 50)
			{
				cnt = 0;
				P0 = Nixietube[sec];
				sec++;
				if (sec == 16)
				{
					sec = 0;
				}
			}
		}
	}	
}