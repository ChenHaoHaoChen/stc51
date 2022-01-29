#include <reg52.h>
#define LCD1602_DB P0      //�궨�����ߣ��мǲ��ӷֺ�
sbit LCD1602_RS = P1^0;    //����/����ѡ��ˣ�1/0�� 
sbit LCD1602_RW = P1^1;    //��/дѡ��Σ�1/0��
sbit LCD1602_E = P1^5;     //LCD1602ʹ�ܶˣ��ߵ�ƽ��Ч��

void LCD1602init();        //��������
void LCD1602writecmd(unsigned char cmd);
void LCD1602readstate();
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);
void LCD1602setcoor(unsigned char x,unsigned char y);
void LCD1602writedate(unsigned char str);

void main()
{
	unsigned char str1[]={ //��һ����Ҫ��ʾ���ַ���1����10���ַ�
		"Chen Hao Hao"
	};
	unsigned char str2[]={ //�ڶ�����Ҫ��ʾ���ַ���1����9���ַ�
		"2021/8/17"
	};
	LCD1602init();
	LCD1602show(2,0,str1);
	LCD1602show(2,1,str2);
	while(1);
}
/*LCD1602��ʼ������*/
void LCD1602init()
{
	LCD1602writecmd(0X38); //��ʾģʽ����
	LCD1602writecmd(0X0C); //��ʾ��������
	LCD1602writecmd(0X06); //��ʾ�������
	LCD1602writecmd(0X01); //��ʾ����
}
/*LCD1602дָ���*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();    //дָ��֮ǰ��Ҫ��״̬��ȷ��STA7=0
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 0;         //����дָ��ֻ����ʹ�ܶ��и�����ʱ��������������������ø�����
	LCD1602_E = 1;
	LCD1602_E = 0;
}
/*LCD1602��״̬����*/
void LCD1602readstate()  //���ڼ��STA7��DB7���Ƿ��ڸߵ�ƽ
{
	unsigned char sta7;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do
	{
		LCD1602_E = 1;
		sta7 = LCD1602_DB;
		LCD1602_E = 0;
	}
	while(sta7 & 0X80);    //��STA7���λ����0ʱ�����Խ�����һ��
}
/*LCD1602��ʾ����*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p)
{
	LCD1602setcoor(x,y);
	while(*p != '\0')
	{
		LCD1602writedate(*p++); //*p++����*p��p++
	}
}
/*LCD1602���궨λ����*/
void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add;
	if(y == 0)
	{
		add = 0X00 + x;
	}
	else
	{
		add = 0X40 + x;
	}
	LCD1602writecmd(add | 0X80);//��������ָ���ַ��0X80+��ַ��
}
/*LCD1602д���ݺ���*/
void LCD1602writedate(unsigned char str)
{
	LCD1602readstate();         //д����֮����Ҫ��״̬�������STA7�Ƿ�Ϊ0
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = str;
	LCD1602_E = 0;              //д����ʹ�ܶ˸����崥�����������������һ��������
	LCD1602_E = 1;
	LCD1602_E = 0;
}