#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned char code lednumber[]={     //�������ʾ����    
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char ledbuffer[6]={         //�������ʾ������
	0XFF,0XFF,0XFF,0XFF,0XFF,0XFF
 };
unsigned char TH0R = 0;              //��ʱ��T0����ֵ
unsigned char TL0R = 0;
unsigned long number;                //numberΪ���Զ˴��ݹ�����ʮ�������ֽ�
void configbuad(unsigned int buad);  //��������
void configtime0(unsigned char ms); 
void main()
{
	ENLED = 0;                         //��74138������
	ADDR3 = 1;
	EA = 1;                            //���ж���ʹ��
	configtime0(1);                    //��ʱ��T0��ʱ1ms
	configbuad(9600);                  //���ò�����Ϊ9600
	while(1)
	{                                  //�������ֽ����������ʮ����ֹ��ʾ����
		ledbuffer[0] = lednumber[number & 0X0F]; 
		ledbuffer[1] = lednumber[number >> 4];
	}
}
/*���ö�ʱ��T0��ʱ1ms*/
void configtime0(unsigned char ms)
{
	unsigned long temp;
	TMOD = TMOD & 0XF0;                //���ö�ʱ��T0Ϊ1ģʽ
	TMOD = TMOD | 0X01;
	temp = 65536-(11059200/12*ms/1000);//���㶨ʱ1ms���
	TH0R = (unsigned char)(temp >> 8); //���㶨ʱ1ms�߰�λ�͵Ͱ�λ
	TL0R = (unsigned char)temp;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;                           //��ʼ��ʱ
	ET0 = 1;                           //�򿪶�ʱ��T0�ж�
}
/*����ͨ�Ų�����*/
void configbuad(unsigned int buad)
{
	SCON = 0X50;                       //���ô���Ϊģʽ1
	TMOD = TMOD & 0X0F;                //���ö�ʱ��T2Ϊģʽ2
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12/32)/buad; //���㲨����
	TL1 = TH1;
	TR1 = 1;                           //��ʱ��T1��ʼ��ʱ
	ES = 1;                            //��UART�����ж�
}
/*����UART�����ж�*/
void interruptUART() interrupt 4
{
	if(RI)                             //Ϊ����״̬
	{
		RI = 0;
		SBUF = SBUF;                     //�ڴ��ڵ���������Ҳ��ʾ
		number = SBUF;                  
	}
	else if(TI)                        //Ϊ����״̬
	{
		TI = 0;
	}
}
/*ÿ1ms���붨ʱ��T0�ж�*/
void interrupttime0() interrupt 1
{
	static unsigned char i = 0; 
	TH0 = TH0R;                        //�������ö�ʱʱ��
	TL0 = TL0R;
	P0 = 0XFF;                         //�������������
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[0];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[1];i=0;break;
		default:break;
	}
}