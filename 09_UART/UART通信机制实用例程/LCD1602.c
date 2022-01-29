/**************************
LCD1602���ú���
***************************/
#include <reg52.h>
#define LCD1602DB P0    //��������
sbit LCD1602RS = P1^0;  //RSΪ����/����ѡ��ˣ�1/0��
sbit LCD1602RW = P1^1;  //RWΪ��/дѡ��ˣ�1/0��
sbit LCD1602E = P1^5;   //EΪʹ�ܶ�
/*LCD1602��״̬����*/
void LCD1602readstate()
{
	unsigned char sta;
	LCD1602RS = 0;
	LCD1602RW = 1;
	LCD1602DB = 0XFF;
	do                    //æλ��⣬��LCD1602DB���λΪ0ʱ����æ��
	{
		LCD1602E = 1;       //ʹ�ܶ��õ���ʱ���ڿ��������ú������˿�
		sta = LCD1602DB;
		LCD1602E = 0;
	}
	while(sta & 0X80);
}
/*LCD1602дָ���*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();   //дָ��ǰ������ж�״̬
	LCD1602RS = 0;
	LCD1602RW = 0;
	LCD1602DB = cmd;
	LCD1602E = 0;         //�����崥��
	LCD1602E = 1;
	LCD1602E = 0;
}
/*LCD1602��ʼ������*/
void LCD1602init()
{
	LCD1602writecmd(0X38);//��ʾģʽ����
	LCD1602writecmd(0X0C);//��ʾ��������
	LCD1602writecmd(0X06);//��ʾ�������
	LCD1602writecmd(0X01);//��ʾ����
}
/*LCD1602��ʾ��������*/
void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add; 
	if(y == 0)
	{
		add = 0X00 + x;     //��һ���׵�ַΪ0X00
	}
	else 
	{
		add = 0X40 + x;     //�ڶ����׵�ַΪ0X40
	}
	LCD1602writecmd(add | 0X80);//��������ָ��
}
/*LCD1602д���ݺ���*/
void LCD1602writedata(unsigned char string)
{
	LCD1602readstate();  //д����ǰ����ж�״̬��������DB7Ϊæλ���
	LCD1602RS = 1;
	LCD1602RW = 0;
	LCD1602DB = string;
	LCD1602E = 0;
	LCD1602E = 1;
	LCD1602E = 0;
}
/*LCD1602��ʾ���ú���*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *str)
{
	LCD1602setcoor(x,y);   //������ʾ����
	while(*str != '\0')
	{
		LCD1602writedata(*str++);
	}
}
/*LCD1602�������㺯��*/
void LCD1602clearscreen()
{
	LCD1602writecmd(0X01);
}