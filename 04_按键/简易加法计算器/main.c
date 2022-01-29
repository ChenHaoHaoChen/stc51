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
unsigned char code number[]={ //�������ʾ��������      
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char keyfinally[4][4]={ //16�������İ�����ֵ
	{1,1,1,1},
  {1,1,1,1},
	{1,1,1,1},
  {1,1,1,1}
  };
unsigned char ledbuffer[]={ //6���������ʾ��������
	0xff,0xff,0xff,0xff,0xff,0xff
  };
unsigned char code keycodemap[4][4]={ //����ӳ���
	{0x00,0xec,0xea,0x0a},
  {0x01,0x02,0x03,0x0b},
	{0x04,0x05,0x06,0x0c},
  {0x07,0x08,0x09,0x0d}
	};
void keyaction(unsigned char keycode); //������������
void shownumber(unsigned int r);
void keyscan();
void ledscan();
void main()
{
	unsigned char backup[4][4]={ //������һ������״ֵ̬
	{1,1,1,1},
  {1,1,1,1},
	{1,1,1,1},
  {1,1,1,1}
  };
	unsigned char i,j;
  ADDR3 = 1; //��74138������
  ENLED = 0;
	
	TMOD = 0X01; //�򿪶�ʱ������ʱ1ms
	TH0 = 0XFC; 
	TL0 = 0X67;
	TR0 = 1;
	
	EA = 1; //�򿪶�ʱ��0�ж�
	ET0 = 1;
	   
	while(1)
	{
		for(i=0;i<4;i++) //��ⰴ��״̬
		{
			for(j=0;j<4;j++)
			{
				if(keyfinally[i][j] != backup[i][j])
				{
					if(backup[i][j] == 0)
					{
						keyaction(keycodemap[i][j]);
					}
					backup[i][j] = keyfinally[i][j];
				}
			}
		}
	}
}
void keyaction(unsigned char keycode)
{
	static unsigned int result = 0;
	static unsigned int temp = 0;
	if((keycode >= 0x00) && (keycode <= 0x09)) //����������Ϊ0-9
	{
		temp = temp*10 + keycode;
		shownumber(temp);          
	}
	else if(keycode == 0x0a)//���������¼�
	{
		result = result + temp; 
		temp = 0;                  
	}
	else if(keycode == 0xea)//����������enter
	{
		result = result + temp;
		shownumber(result);
	}
	else if(keycode == 0xec)//����������esc
	{
		result = 0;
		temp = 0;
		shownumber(result);
	}
	else
	{}
}
void shownumber(unsigned int r)
{
	unsigned char i,j;
	for(i=0;i<6;i++)  //�������λ���䵽ledbuffer������
	{
		ledbuffer[i] = number[r%10];
		r = r/10;
	}
	for(j=5;j>0;j--) //��λ����
	{
		if(ledbuffer[j] == 0xc0)
		{
			ledbuffer[j] = 0xff;
		}
		else
		{
			break;
		}
	}
}
void interrupttime0() interrupt 1 //ÿ1ms�����ж�
{
	TH0 = 0XFC;
	TL0 = 0X67;
	keyscan(); //����ÿ1msɨ��
	ledscan(); //�����ÿ1msɨ��
}
void keyscan()
{
	static unsigned char keybuffer[4][4]={
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
	{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
	};
	static unsigned char i = 0;
	static unsigned char j = 0;
	keybuffer[i][0] = keybuffer[i][0] << 1 |KEYIN1;
	keybuffer[i][1] = keybuffer[i][1] << 1 |KEYIN2;
	keybuffer[i][2] = keybuffer[i][2] << 1 |KEYIN3;
	keybuffer[i][3] = keybuffer[i][3] << 1 |KEYIN4;
	for(j=0;j<4;j++)
	{
		if( (keybuffer[i][j] &0x0f) == 0x00 )
		{
			keyfinally[i][j] = 0;
		}
		else if( (keybuffer[i][j] | 0xf0) == 0xff )
		{
			keyfinally[i][j] = 1;
		}
		else
		{}
	}
	switch(i)
	{
		case 0:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 0;i++;break;
		case 1:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 0;KEYOUT1 = 1;i++;break;
		case 2:KEYOUT4 = 1;KEYOUT3 = 0;KEYOUT2 = 1;KEYOUT1 = 1;i++;break;
		case 3:KEYOUT4 = 0;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 1;i=0;break;
		default:break;
	}
}
void ledscan()
{
	static unsigned char i = 0;
	P0 = 0xff;
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i=0;break;
	}
}  