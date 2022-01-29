/*
LCD1602配置函数
*/
#include <reg52.h>
#define LCD1602_DB P0    //宏定义总线，切记不加分号
sbit LCD1602_RS = P1^0;  //数据/命令选择端（1/0） 
sbit LCD1602_RW = P1^1;  //读/写选择段（1/0）
sbit LCD1602_E = P1^5;   //LCD1602使能端（高电平有效）
void LCD1602init();      //函数声明
void LCD1602writecmd(unsigned char cmd);
void LCD1602readstate();
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);
void LCD1602setcoor(unsigned char x,unsigned char y);
void LCD1602writedata(unsigned char dat);
/*LCD1602初始化*/
void LCD1602init()
{
	LCD1602writecmd(0X38);//显示模式设置
	LCD1602writecmd(0X0C);//显示开关设置
	LCD1602writecmd(0X06);//显示光标设置
	LCD1602writecmd(0X01);//显示清屏
}
/*LCD1602写指令函数*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();   //写指令前需进行读状态
	LCD1602_RS = 0;  
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 0;        //写指令高脉冲触发
	LCD1602_E = 1;
	LCD1602_E = 0;
}
/*LCD1602读状态函数*/
void LCD1602readstate()
{
	unsigned char sta7;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do                   //通过判断STA7（DB7）是否为1，
	{                    //得知LCD1602是否忙，1：忙，0：不忙
		LCD1602_E = 1;     //只在E端用到时才打开，不然会产生影响
		sta7 = LCD1602_DB;
		LCD1602_E = 0;
	}
	while(sta7 & 0X80);
}
/*LCD1602显示函数*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p)
{
	LCD1602setcoor(x,y); //LCD1602坐标定位函数
	while(*p != '\0')
	{
		LCD1602writedata(*p++);
	}
}
/*LCD1602坐标定位函数*/
void LCD1602setcoor(unsigned char x,unsigned char y)
{
	unsigned char add;
	if(y == 0)
	{
		add = 0X00 + x;   //第一行首地址为0X00
	}
	else 
	{
		add = 0X40 + x;   //第一行首地址为0X40
	}
	LCD1602writecmd(add | 0X80); //定位好需再次写指令
}
/*LCD1602写数据函数*/
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