#include <reg52.h>
unsigned long beats;
void cntbeats(unsigned long angle);
void main()
{
	TMOD = 0X01;        //���ö�ʱ��Ϊ1ģʽ
	TH0 = 0XF8;         //��ʱ2ms
	TL0 = 0XCD;
	TR0 = 1;            //��ʼ��ʱ
	EA = 1;             //�����ж�
	ET0 = 1;            //�򿪶�ʱ��T0�ж�
	cntbeats(360*2+180);
	while(1);
}
void cntbeats(unsigned long angle)//������ĺ���
{
	EA = 0; //Ϊ�˱���������ʱ�����жϣ�����ǰ���жϹرգ�������ٴ�
	beats = (angle*64*64)/360;
	EA = 1;
}
void interrupttime0() interrupt 1
{
	unsigned char code beat[8]={
	0x0e,0x0c,0x0d,0x09,0x0b,0x03,0x07,0x06
  };
	static unsigned char temp;
	static unsigned char i = 0;
	TH0 = 0XF8;//��ʱ�����¶�ʱ2ms             
	TL0 = 0XCD;
	if(beats != 0)
	{
		temp = P1;            //tempΪ�м����
	  temp = temp & 0XF0;   //��temp����λ���㣬����Ӱ�����λ
	  temp = temp | beat[i];//��P1����λ��ֵ������Ӱ�����λ
		P1 = temp;            
		i++;
		i = 0x07 & i;         //��i=8ʱ��i=0
		beats--;
	}
	else
	{
		P1 = 0X0F | P1;       //P1����λ���㣬����λ����
	}
}