#include <reg52.h>

sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main()
{
	unsigned int cnt=0;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	
	LED = 0;
	
	TMOD = 0x01;       //TMOD����Ϊ0x01��Ϊ1ģʽ
	TH0 = 0xdc;        //TH0��ֵΪ0xdc����0xffΪ10ms
	TL0 = 0x00;
	TR0 = 1;           //TRO��1����ʱ����ʼ��ʱ
	
	while(1)
	{
		if (TF0 == 1)    //��TF0=1������ʱ���������
		{
			TH0 = 0xdc;    //��λ
			TL0 = 0x00;    //��λ
			TF0 = 0;       //��λ
			cnt++;         //cnt�Լ�
			if (cnt == 600)//��cnt=600������ʱ600*10ms=1s
			{
				cnt = 0;     //cnt��λ
				LED=~LED;    //��6s��LED�����룬�Դ˷��� 
			}
		}
	}
}