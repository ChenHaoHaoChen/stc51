#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit key4 = P2^7;
unsigned char code number[]={   //����number������0-9��Ӧ��16���Ʊ���
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char keyfinally = 1;   //keyfinallyΪ������ϰ�������ֵ
void main()
{
	unsigned char backup = 1;     //backupΪ������һ������״̬����������
	unsigned char i = 0;          //�������ʾ����
	ENLED = 0;                    //��74138������
	ADDR3 = 1;
	ADDR2 = 0;                    //�����Ϊ�����
	ADDR1 = 0;
	ADDR0 = 0;
	TMOD = 0x01;                  //TO��ʱ������Ϊ1ģʽ
	TH0 = 0xf8;                   //��ʱ2ms
	TL0 = 0xcd;
	TR0 = 1;                      //��ʼ��ʱ 
	EA = 1;                       //�򿪶�ʱT0�ж�
	ET0 = 1;
	P2 = 0xf7;//���ǰ�����Ч��keyIn1-4�������1(�ߵ�ƽ)��
	          //keyOut1-4����ֻ����һ�����0����������Ϊ1��˭Ϊ0����Ӧ��һ�а�������Ч
	P0 = number[i];               //��ʾ���λ�����
	while(1)
	{
		if(keyfinally != backup)
		{
			if(backup == 0)
			{
				i++;
				if(i == 10)
				{i = 0;}
				P0 = number[i];
			}
			backup = keyfinally;
		}
	}
}
void interrupttime0() interrupt 1  //1Ϊx��x*8+3=������ַ
{
	static unsigned char keybuffer = 0xff;//���ڼ������8��״̬��ÿ��״̬2ms���Ƿ�ȫΪ1��0
	TH0 = 0xf8;
	TL0 = 0xcd;
	keybuffer = (keybuffer << 1) | key4;
	if(keybuffer == 0x00)
	{
		keyfinally = 0;                //����8��״̬ȫΪ0ʱ��Ĭ�ϰ���Ϊ0(����)
	}
	else if(keybuffer == 0xff)
	{
		keyfinally = 1;                //����8��״̬ȫΪ1ʱ��Ĭ�ϰ���Ϊ1(����)
	}
	else
	{}
}