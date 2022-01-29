/*******************************************************
               ģ����׵�ȱ�
							 
*         19����2 �º� 2019314230		

*  LED1501(������)��     P3.2						 
*  ��������(K17~K20)��   P3.4~P3.7;
*  �������ʾ(��λ����)��λ��(P0),����(P1);
*  EEPROM(24C02)��       SCL(P2.6),SDA(P2.7);
*  A/Dת��(TLC549)��     CLK(P2.0),DO(P2.1),CS(P2.2);
*  ʵʱʱ��(DS1302)��    RST(P2.3),SCLK(P2.4),I/O(P2.5);
*******************************************************/
#include <reg52.h>        //Ĭ��ͷ�ļ�
#include "DS1302.h"       //DS1302����ͷ�ļ�
#include "TLC549.h"       //TLC549����ͷ�ļ�
#include "key.h"          //��������ͷ�ļ�
#include "EEPROM.h"       //EEPROM����ͷ�ļ�
#include "T0T1.h"         //��ʱ��T0T1����ͷ�ļ�
#include "display.h"      //�����ܼ��������ʾ����ͷ�ļ�

unsigned int ELEFEE = 58;      //���,Ĭ��Ϊ0.52Ԫ/��
unsigned int ELECON = 0;  //�õ���
unsigned int FEE = 0;     //�ܵ��
unsigned char time[8];    //DS1302��ȡʱ�仺������

/**********������**********/
void main()
{
  EA = 1;             //�����ж�
	configtime0(1);     //����T0��ʱ����ʱ1ms
	configtime1(2);     //����T1��ʱ����ʱ2ms
	DS1302init();       //DS1302��ʼ��
	EEPROMinit();       //EEPROM��ʼ��
	P3 = P3 | 0XF0;     //�����ĸ�������ƽ
	while(1)
	{
		keyscan();    //����ɨ��
		ADscan();     //ADɨ��
		DS1302burstread(time); //DS1302burstģʽ��ʱ��
	}
}
/**********T0��ʱ���жϺ���**********/
void interruptt0() interrupt 1
{
	TH0 = TH0R;     //T0��ʱ�����¶�ʱ1ms
	TL0 = TL0R;
	startdisplay();
	timedisplay();
	enddisplay();
	changedisplay();
	display();
}
/**********����ԼӺ���(ÿ1s��0.1��)**********/
void ELECONadd()
{
	static unsigned int bit_1s = 0;
	if( (bit_start == 1) || (bit_time == 1))
	{
		bit_1s++;
		if(bit_1s == 500)
		{
			bit_1s = 0;
			ELECON++;
		}
	}
}
/**********T1��ʱ���жϺ���**********/
void interruptt1() interrupt 3
{
	TH1 = TH1R;    //T1��ʱ�����¶�ʱ2ms
	TL1 = TL1R;
	keybuff();
	ELECONadd();
	ADrefresh();
}