/**************************
    ��ʱ��T0T1��ʱ����
**************************/
#include <reg52.h>
unsigned char TH0R,TL0R;  //T0�ߵͰ�λ����ֵ
unsigned char TH1R,TL1R;  //T1�ߵͰ�λ����ֵ

/**********T0��ʱ�����ú���**********/
void configtime0(unsigned char ms)
{
	unsigned int temp;
	temp = 65536 - 1000*ms;
	TH0R = (unsigned char)(temp >> 8); 
	TL0R = (unsigned char)(temp);
	TH0 = TH0R;         //��ʱ1ms
	TL0 = TL0R;
	TMOD = 0X01 | TMOD; //T0����Ϊ1ģʽ
	TR0 = 1;            //T0��ʼ��ʱ
	ET0 = 1;            //��T0�ж�
}
/**********T1��ʱ�����ú���**********/
void configtime1(unsigned char ms)
{
	unsigned int temp;
	temp = 65536 - 1000*ms;
	TH1R = (unsigned char)(temp >> 8);
	TL1R = (unsigned char)(temp);
	TH1 = TH0R;         //��ʱ2ms
	TL1 = TL0R;
	TMOD = 0X10 | TMOD; //T1����Ϊ1ģʽ
	TR1 = 1;            //T1��ʼ��ʱ
	ET1 = 1;            //��T1�ж�
}