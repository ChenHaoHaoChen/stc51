#include <reg52.h>
void configbuad(unsigned int buad);//��������
void main()
{
	EA = 1;            //�����ж�
	ES = 1;            //��UART�����ж�
  SCON = 0X50;       //���ô���ͨ��Ϊ1ģʽ
	configbuad(9600);  //���ò�����Ϊ9600
	while(1);
}
void configbuad(unsigned int buad)
{
	TMOD = TMOD & 0X0F;//���ö�ʱ��1Ϊģʽ2�����Զ�1��װģʽ
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12)/32/buad;
	TL1 = TH0;
	TR1 = 1;           //��ʱ��1��ʼ��ʱ
}
void interruptUART() interrupt 4
{
	if(TI)             //RIΪ�����жϱ�־������Ҫ�������
	{
		TI = 0;
	}
	else if(RI)        //TIΪ�����жϱ�־������Ҫ�������
	{
		RI = 0;
		SBUF = SBUF + 1; //SBUFΪ���ܵ�������
	}
}