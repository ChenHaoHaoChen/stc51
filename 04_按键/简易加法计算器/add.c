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
unsigned char code ledchar[]={            //�����������ʾת���ַ�
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 
  0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e 
	};
unsigned char ledbuff[6]={                //�����������ʾ������
	0xff,0xff,0xff,0xff,0xff,0xff
  };
unsigned char code keycodemap[4][4]={     //����ӳ������������޹���
	{0x31,0x32,0x33,0x26},                  //1��2��3��+
	{0x34,0x35,0x36,0x25},                  //4��5��6��
	{0x37,0x38,0x39,0x28},                  //7��8��9
	{0x30,0x1b,0x0d,0x27}                   //0��esc��=
	};
unsigned char keysta[4][4]={              //16����������״̬
	{1,1,1,1},{1,1,1,1},
  {1,1,1,1},{1,1,1,1}
  };

void keydriver();                         //��������
void keyaction(unsigned char keycode);
void shownumber(unsigned int num);
void ledscan();
void keyscan();
void main()              
{
	ENLED = 0;                              //��74138������
	ADDR3 = 1;
	TMOD = 0x01;                            //��1ģʽ�򿪶�ʱ��������ʱ1ms����ʼ��ʱ
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	EA = 1;                                 //�򿪶�ʱT0�ж�
	ET0 = 1;
	ledbuff[0] = ledchar[0];                //���λ�����Ĭ����ʾ0
	while(1)
	{
		keydriver();
	}
}
/*������⺯��*/
void keydriver()
{
	unsigned char i,j;
	static unsigned char backup[4][4]={
	{1,1,1,1},{1,1,1,1},
  {1,1,1,1},{1,1,1,1}
  };
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if(keysta[i][j] != backup[i][j])
			{
				if(backup[i][j] != 0)
				{
					keyaction(keycodemap[i][j]);
				}
				backup[i][j] = keysta[i][j];
			}
		}
	}
}
/*����ִ�к���������⵽��Ӧ��������ʱ��ִ�а�����Ӧ����*/
void keyaction(unsigned char keycode)
{
	static unsigned long addend = 0;
	static unsigned long result = 0;
	if((keycode >= 0x30) && (keycode <= 0x39))  //����⵽����0-9������ʱ
	{
		addend = (addend*10) + (keycode - 0x30);  //ȷ����λ����������������
		shownumber(addend);
	}
	else if(keycode == 0x26)                    //����⵽k4����+������
	{
		result += addend;                         //result = addend;Ҳ�У�������������Ӽ���
		addend = 0;
		shownumber(result);
	}
	else if(keycode == 0x0d)                    //����⵽k15����=������
	{
		result += addend;
		addend = 0;
		shownumber(result);
	}		
	else if(keycode == 0x1b)                    //����⵽k14����esc������
	{
		addend = 0;
		result = 0;
    shownumber(addend);		
	}
}
/*�������ʾ����*/
void shownumber(unsigned long num)
{
	unsigned char i;
	unsigned char buf[6];
	for(i=0;i<6;i++)                            //��ʵ��addend�ֽ�ɸ���ʮ���ٵ�λ
	{
		buf[i] = num % 10;
		num = num / 10;
	}
	for(i=5;i>0;i--)                            //��λ���㣬���Ϊ���������ֲ�����
	{
		if(buf[i] == 0)
		{
			ledbuff[i] = 0xff;
		}
		else 
		{
			break;
		}
	}
	for( ;i>=0;i--)                            //�нӸ�λ�����е�i������λ0�����ʣ����Ч������ʾ�������
	{
		ledbuff[i] = ledchar[buf[i]];
	}
}
/*��ʱ��T0�жϺ�����ÿ1ms����һ��*/																																							 
void interrupttime0() interrupt 1
{
	TH0 = 0xfc;                                //��ʱ�����¶�ʱ
	TL0 = 0x67;
	ledscan();
	keyscan();
}
/*�����ˢ�º���*/
void ledscan()
{
	static unsigned char i = 0;
	P0 = 0xff;                                 //��������ܹ�Ӱ
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;i++;P0 = ledbuff[0];break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;i++;P0 = ledbuff[1];break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;i++;P0 = ledbuff[2];break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;i++;P0 = ledbuff[3];break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;i++;P0 = ledbuff[4];break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;i=0;P0 = ledbuff[5];break;
		default:break;
	}
}
/*����ˢ����������*/
void keyscan()
{
	unsigned char i;
	static unsigned char keyout = 0;
	static unsigned char keybuf[4][4]={       //���尴����������
		{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},{0xff,0xff,0xff,0xff}
	};
	keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEYIN1; //ÿ4ms����λһ��
	keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEYIN2;
	keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEYIN3;
	keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEYIN4;
	for(i=0;i<4;i++)
	{
		if((keybuf[keyout][i]&0x0f) == 0x00)                 //�����������ĸ�0���͵�ƽ��ʱ���϶���������
		{
			keysta[keyout][i] = 0;
		}
		else if((keybuf[keyout][i]&0x0f) == 0x0f)            //�����������ĸ�1���ߵ�ƽ��ʱ���϶���������
		{
			keysta[keyout][i] = 1;       
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
		default:break;
	}
}
	
	
	
	
	
	
	
	
	
	
	
	
	