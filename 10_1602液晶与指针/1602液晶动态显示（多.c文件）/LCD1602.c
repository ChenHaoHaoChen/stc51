/*
LCD1602���ú���
*/
#include <reg52.h>
#define LCD1602_DB P0    //�궨�����ߣ��мǲ��ӷֺ�
sbit LCD1602_RS = P1^0;  //����/����ѡ��ˣ�1/0�� 
sbit LCD1602_RW = P1^1;  //��/дѡ��Σ�1/0��
sbit LCD1602_E = P1^5;   //LCD1602ʹ�ܶˣ��ߵ�ƽ��Ч��
void LCD1602init();      //��������
void LCD1602writecmd(unsigned char cmd);
void LCD1602readstate();
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);
void LCD1602setcoor(unsigned char x,unsigned char y);
void LCD1602writedata(unsigned char dat);
/*LCD1602��ʼ��*/
void LCD1602init()
{
	LCD1602writecmd(0X38);//��ʾģʽ����
	LCD1602writecmd(0X0C);//��ʾ��������
	LCD1602writecmd(0X06);//��ʾ�������
	LCD1602writecmd(0X01);//��ʾ����
}
/*LCD1602дָ���*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();   //дָ��ǰ����ж�״̬
	LCD1602_RS = 0;  
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 0;        //дָ������崥��
	LCD1602_E = 1;
	LCD1602_E = 0;
}
/*LCD1602��״̬����*/
void LCD1602readstate()
{
	unsigned char sta7;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do                   //ͨ���ж�STA7��DB7���Ƿ�Ϊ1��
	{                    //��֪LCD1602�Ƿ�æ��1��æ��0����æ
		LCD1602_E = 1;     //ֻ��E���õ�ʱ�Ŵ򿪣���Ȼ�����Ӱ��
		sta7 = LCD1602_DB;
		LCD1602_E = 0;
	}
	while(sta7 & 0X80);
}
/*LCD1602��ʾ����*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p)
{
	LCD1602setcoor(x,y); //LCD1602���궨λ����
	while(*p != '\0')
	{
		LCD1602writedata(*p++);
	}
}
/*LCD1602���궨λ����*/
void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add;
	if(y == 0)
	{
		add = 0X00 + x;   //��һ���׵�ַΪ0X00
	}
	else 
	{
		add = 0X40 + x;   //��һ���׵�ַΪ0X40
	}
	LCD1602writecmd(add | 0X80); //��λ�����ٴ�дָ��
}
/*LCD1602д���ݺ���*/
void LCD1602writedata(unsigned char dat)
{
	LCD1602readstate();
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = dat;
	LCD1602_E = 0;
	LCD1602_E = 1;
	LCD1602_E = 0;
}