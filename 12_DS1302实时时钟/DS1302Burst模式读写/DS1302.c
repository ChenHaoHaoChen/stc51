/*********************
DS1302���ú���
*********************/
#include <reg52.h>
sbit DS1302_CE = P1^7;
sbit DS1302_SCK = P3^5;
sbit DS1302_SIO = P3^4;
unsigned char code date[7]={           //����ʱ�� 2020-8-27 11��00��00
		0X00,0X14,0X11,0X28,0X08,0X06,0X21
	};
/*DS1302д�ֽں���*/
void DS1302writebyte(unsigned char dat)
{
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if((mask&dat) != 0) //��׼����SIO�����ݣ�����������ʱ��SCK��
		{
			DS1302_SIO = 1;      
		}
		else
		{
			DS1302_SIO = 0;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
}
/*DS1302д�ֽں���*/
unsigned char DS1302readbyte()
{
	unsigned char dat = 0;
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if(DS1302_SIO != 0)
		{
			dat = dat | mask;//�ȶ�ȡSIOֵ������������ʱ��SCK��
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
	return dat;
}
/*DS1302 ���� д����*/
void DS1302singlewrite(unsigned char addsn,unsigned char dat)
{
	DS1302_CE = 1;         //��ʹ�ܶ�
	DS1302writebyte((addsn<<1) | 0X80);//д���ַ
	DS1302writebyte(dat);  //д������
	DS1302_CE = 0;         //�ر�ʹ�ܶ�
}
/*DS1302 ���� ������*/
unsigned char DS1302singleread(unsigned char addsn)
{
	unsigned char dat;
	DS1302_CE = 1;         //��ʹ�ܶ�
	DS1302writebyte((addsn<<1) | 0X81);//д���ַ
	dat = DS1302readbyte();//��ȡ����
	DS1302_CE = 0;         //�ر�ʹ�ܶ�
	return dat;
}
/*DS1302burstģʽд����*/
void DS1302burstwrite(unsigned char *p)
{
	unsigned char i;
	DS1302_CE = 1;
	DS1302writebyte(0XBE);
	for(i=0;i<8;i++)      //burstģʽһ�ο���д�˸��Ĵ���
	{
		DS1302writebyte(p[i]);
	}
	DS1302_CE = 0;
}
/*DS1302burstģʽ������*/
void DS1302burstread(unsigned char *p)
{
	unsigned char i;
	DS1302_CE = 1;
	DS1302writebyte(0XBF);
	for(i=0;i<8;i++)      //burstģʽһ�ο��Զ��˸��Ĵ���
	{
		p[i] = DS1302readbyte();
	}
	DS1302_CE = 0;
}
/*DS1302��ʼ������*/
void DS1302init()
{
	DS1302_CE = 0;       //51MCUĬ������ߵ�ƽ�����������ʹ�ܶˣ�CE����ʱ�Ӷˣ�SCK����ƽ
	DS1302_SCK = 0;
	DS1302singlewrite(7,0X00);//д����λֵ0��������д����
	DS1302burstwrite(date);//����burstģʽд�����úõ�ʱ��
}
