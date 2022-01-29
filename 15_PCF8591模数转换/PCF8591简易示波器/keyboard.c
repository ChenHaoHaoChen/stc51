/**************************
�������ú���
**************************/
#include <reg52.h>
sbit KEYOUT1 = P2^3;
sbit KEYOUT2 = P2^2;
sbit KEYOUT3 = P2^1;
sbit KEYOUT4 = P2^0;
sbit KEYIN1 = P2^4;
sbit KEYIN2 = P2^5;
sbit KEYIN3 = P2^6;
sbit KEYIN4 = P2^7;
unsigned char keyfinally[4][4]={         //������ֵ
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
  };
unsigned char code keycodemap[4][4]={    //����ӳ���
	{1,2,3,10},
	{4,5,6,12},
	{7,8,9,11},
	{0,14,15,13}
  };
extern void keyaction(unsigned char key);//�ⲿ��������
/*������������*/
void keydriver()
{
	static unsigned char backup[4][4]={    //������һ��״ֵ̬
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
  };
	unsigned char i,j;
	for(i=0;i<4;i++)                       //ѭ����ⰴ��״̬
	{
		for(j=0;j<4;j++)
		{
			if(keyfinally[i][j] != backup[i][j])
			{
				if(backup[i][j] != 0)            //����������Ч��backup[i][j] == 0Ϊ���������⣩
				{
					keyaction(keycodemap[i][j]);
				}
				backup[i][j] = keyfinally[i][j]; //��������ǰ״̬���Ƹ�backup���飬�����´αȽ�
			}
		}
	}
}
/*����ɨ�躯�������ж���ÿ1msɨ��һ��*/
void keyscan()
{
	unsigned char i;
	static unsigned char keyout = 0;
	static unsigned char keybuffer[4][4]={ //����״̬������
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF}
	};
	keybuffer[keyout][0] = (keybuffer[keyout][0] << 1) | KEYIN1;
	keybuffer[keyout][1] = (keybuffer[keyout][1] << 1) | KEYIN2;
	keybuffer[keyout][2] = (keybuffer[keyout][2] << 1) | KEYIN3;
	keybuffer[keyout][3] = (keybuffer[keyout][3] << 1) | KEYIN4;
	for(i=0;i<4;i++)                     
	{
		if( (keybuffer[keyout][i] & 0X0F) == 0X00 )//������״̬����Ϊ1111 0000ʱ��
		{                                          //�����Ĵ�ɨ��ֵΪ0����4*4ms���ǰ���״̬�϶���������
			keyfinally[keyout][i] = 0;
		}
		else if( (keybuffer[keyout][i] & 0X0F) == 0x0F )
		{                                          //������״̬����Ϊ0000 1111ʱ��
			keyfinally[keyout][i] = 1;               //�����Ĵ�ɨ��ֵΪ1����4*4ms���ǰ���״̬�϶���������
		}
	}
	keyout++;
	if(keyout == 4)
	{
		keyout = 0;
	}
	switch(keyout)                               //ͬһʱ��ֻ������һ��KEYOUT=0��ͨ��ѭ��ɨ�轨�����󰴼�
	{                                            //ɨ������16ms
		case 0:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 0;break;
		case 1:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 0;KEYOUT1 = 1;break;
		case 2:KEYOUT4 = 1;KEYOUT3 = 0;KEYOUT2 = 1;KEYOUT1 = 1;break;
		case 3:KEYOUT4 = 0;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 1;break;
		default:break;
	}	
}