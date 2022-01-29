/**************************
LCD1602配置函数
***************************/
#include <reg52.h>
#define LCD1602DB P0    //定义总线
sbit LCD1602RS = P1^0;  //RS为数据/命令选择端（1/0）
sbit LCD1602RW = P1^1;  //RW为读/写选择端（1/0）
sbit LCD1602E = P1^5;   //E为使能端
/*LCD1602读状态函数*/
void LCD1602readstate()
{
	unsigned char sta;
	LCD1602RS = 0;
	LCD1602RW = 1;
	LCD1602DB = 0XFF;
	do                    //忙位检测，当LCD1602DB最高位为0时，不忙。
	{
		LCD1602E = 1;       //使能端用到的时候在开，先配置好其他端口
		sta = LCD1602DB;
		LCD1602E = 0;
	}
	while(sta & 0X80);
}
/*LCD1602写指令函数*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();   //写指令前必须进行读状态
	LCD1602RS = 0;
	LCD1602RW = 0;
	LCD1602DB = cmd;
	LCD1602E = 0;         //高脉冲触发
	LCD1602E = 1;
	LCD1602E = 0;
}
/*LCD1602初始化函数*/
void LCD1602init()
{
	LCD1602writecmd(0X38);//显示模式设置
	LCD1602writecmd(0X0C);//显示开关设置
	LCD1602writecmd(0X06);//显示光标设置
	LCD1602writecmd(0X01);//显示清屏
}
/*LCD1602显示坐标设置*/
void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add; 
	if(y == 0)
	{
		add = 0X00 + x;     //第一行首地址为0X00
	}
	else 
	{
		add = 0X40 + x;     //第二行首地址为0X40
	}
	LCD1602writecmd(add | 0X80);//设置数据指针
}
/*LCD1602写数据函数*/
void LCD1602writedata(unsigned char string)
{
	LCD1602readstate();  //写数据前须进行读状态，即进行DB7为忙位检测
	LCD1602RS = 1;
	LCD1602RW = 0;
	LCD1602DB = string;
	LCD1602E = 0;
	LCD1602E = 1;
	LCD1602E = 0;
}
/*LCD1602显示配置函数*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *str)
{
	LCD1602setcoor(x,y);   //设置显示坐标
	while(*str != '\0')
	{
		LCD1602writedata(*str++);
	}
}
/*LCD1602整屏清零函数*/
void LCD1602clearscreen()
{
	LCD1602writecmd(0X01);
}