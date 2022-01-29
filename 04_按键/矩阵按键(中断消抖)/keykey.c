#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEYOUT4 = P2^0;
sbit KEYOUT3 = P2^1;
sbit KEYOUT2 = P2^2;
sbit KEYOUT1 = P2^3;
sbit KEYIN1 = P2^4;
sbit KEYIN2 = P2^5;
sbit KEYIN3 = P2^6;
sbit KEYIN4 = P2^7;
unsigned char code number[]={               //�������ʾ���飺0-F
		0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 
		0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e 
	};
unsigned char keyfinally[4][4]={            //����֮�󣬰���������ֵ
	{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
  };
void main()
{
	unsigned char i,j;                        //˫��ѭ������
	unsigned char backup[4][4]={              //������һ��״̬��ֵ
		{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};
	ENLED = 0;                                //��74138������
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	TMOD = 0x01;                              //���ö�ʱ����1ģʽ
	TH0 = 0xfc;                               //��ʱ1ms
	TL0 = 0x66;
	TR0 = 1;                                  //�򿪶�ʱ��
	EA = 1;                                   //�򿪶�ʱ��TO�ж�
	ET0 = 1;
	P0 = number[0];                           //Ĭ����ʾnumber[0],�����������ʾ0
	while(1)
	{
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(keyfinally[i][j] != backup[i][j])//������ֵ����һ��״ֵ̬��ͬʱ�����ж�
				{
					if(backup[i][j] == 0)             //��������ʱִ�ж���
					{
						P0 = number[i*4+j];             //�������ʾ
					}
					backup[i][j] = keyfinally[i][j];  //������һ��״ֵ̬
				}
			}
		}
	}
}
void interrupttime0() interrupt 1
{
	static unsigned char keybuffer[4][4]={    //���󰴼�ɨ��������壬Ĭ��Ϊ0xff����1111 1111
		{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
	};
	static unsigned char keyout = 0;         //���󰴼�ɨ���������
	static unsigned char i;
	TH0 = 0xfc;                              //��ʱ�����¶�ʱ1ms
	TL0 = 0x66;
  keybuffer[keyout][0] = (keybuffer[keyout][0] << 1) | KEYIN1;
	keybuffer[keyout][1] = (keybuffer[keyout][1] << 1) | KEYIN2;
	keybuffer[keyout][2] = (keybuffer[keyout][2] << 1) | KEYIN3;
	keybuffer[keyout][3] = (keybuffer[keyout][3] << 1) | KEYIN4;
	for(i=0;i<4;i++)
	{
		if((keybuffer[keyout][i]&0x0f) == 0x00)//������������λ0ʱ���϶���������
		{
			keyfinally[keyout][i] = 0;
		}
		else if((keybuffer[keyout][i]&0x0f) == 0x0f)//������������λ1ʱ���϶���������
		{
			keyfinally[keyout][i] = 1;       
		}
		else
		{}
	}
	keyout++;
	if(keyout == 4)
	{
		keyout = 0;
	}
	switch(keyout)
	{
		case 0:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 0;break;
		case 1:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 0;KEYOUT1 = 1;break;
		case 2:KEYOUT4 = 1;KEYOUT3 = 0;KEYOUT2 = 1;KEYOUT1 = 1;break;
		case 3:KEYOUT4 = 0;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 1;break;
	}
}