/**************************
      DS1302ͨ�ų���
**************************/
#include <reg52.h>
sbit SCE = P2^3;
sbit SCK = P2^4;
sbit SIO = P2^5;
unsigned char DS1302singleread(unsigned char adr);
void DS1302singlewrite(unsigned char adr,unsigned char date);
void DS1302burstwrite(unsigned char *date);
void DS1302burstread(unsigned char *date);
void DS1302bytewrite(unsigned char date);
unsigned char DS1302byteread();

unsigned char code timeinit[] = {   //2021��11��29��09ʱ14��00�룬����һ
	0X00,0X14,0X09,0X29,0X11,0X01,0X21
};

/**********DS1302��ʼ��**********/
void DS1302init()
{
	unsigned char date;
	SCE = 0;  //ʹ������Ĭ�ϵ͵�ƽ
	SCK = 0;  //ʱ������Ĭ�ϵ͵�ƽ
	date = DS1302singleread(0x81);  //����Ĵ���
	if((date & 0x80) != 0)          //����Ĵ������λΪ1����ʱ��ֹͣ����
	{                             
		DS1302singlewrite(0X8E,0X00); //ȥ��д����λ
		DS1302burstwrite(timeinit);   //burstģʽдʱ��
	}
}
/**********DS1302������**********/
unsigned char DS1302singleread(unsigned char adr)
{
	unsigned char date;
	SCE = 1;                //ʹ��Ƭѡ�ź�
	DS1302bytewrite(adr);   //���ֽ�д��Ҫ��ȡ���ݵĵ�ַ
	date = DS1302byteread();//���ֽڶ�
	SCE = 0;                //����Ƭѡ�ź�
	return date;
}
/**********DS1302д����**********/
void DS1302singlewrite(unsigned char adr,unsigned char date)
{
	SCE = 1;              //ʹ��Ƭѡ�ź�
	DS1302bytewrite(adr); //���ֽ�д��Ҫд�����ݵĵ�ַ
	DS1302bytewrite(date);//���ֽ�д������
	SCE = 0;              //����Ƭѡ�ź�
}
/**********DS1302burstд����**********/
void DS1302burstwrite(unsigned char *date)
{
	unsigned char i;
	SCE = 1;              //ʹ��Ƭѡ�ź�
	DS1302bytewrite(0XBE);//����Ϊburstģʽ
	for(i=0;i<8;i++)
	{
		DS1302bytewrite(date[i]);//һ����д�����Ĵ�������
	}
	SCE = 0;              //����Ƭѡ�ź�
}
/**********DS1302burst������**********/
void DS1302burstread(unsigned char *date)
{
	unsigned char i;
	SCE = 1;              //ʹ��Ƭѡ�ź�
	DS1302bytewrite(0XBF);//����Ϊburstģʽ
	for(i=0;i<8;i++)
	{
		date[i] = DS1302byteread();//һ���Զ�ȡ����Ĵ�������
	}
	SCE = 0;              //����Ƭѡ�ź�
}
/**********DS1302���ֽ�д����**********/
void DS1302bytewrite(unsigned char date)
{
	unsigned char i;
	for(i=0X01; i!=0; i=i<<1)
	{
		if((i&date) != 0)
		{
			SIO = 1;
		}
		else
		{
			SIO = 0;
		}
		SCK = 1;
		SCK = 0;
	}
	SIO = 1;
}
/**********DS1302���ֽڲ���**********/
unsigned char DS1302byteread()
{
	unsigned char i;
	unsigned char date = 0;
	for(i=0X01;i!=0;i=i<<1)
	{
		if(SIO != 0)
		{
			date = date | i;
		}
		SCK = 1;
		SCK = 0;
	}
	return date;
}