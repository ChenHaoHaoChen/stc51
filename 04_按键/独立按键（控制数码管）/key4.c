#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY = P2^7;                //����һ������
unsigned char code number[]={   //����number������0-9��Ӧ��16���Ʊ���
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
void main()
{
	unsigned char i = 0;          
	bit backup = 1;               //backupΪ��������һ��״̬
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	P2 = 0XF7;
	P0 = number[i];
	while(1)
	{
		if(KEY != backup)           //��������ǰ״̬����һ��״̬��ͬʱ����
		{
			if(backup == 0)           //��������һ��״̬Ϊ0ʱ����
			{
			  i++;
			  if(i == 10)             //���жϺ��Ƿ����10�������С��
			  {
				  i = 0;
			  }
				P0 = number[i];         //����С��
			}
			backup = KEY;             //����жϺ󣬽�������ǰ״̬��ֵ��backup
		}
	}	
}