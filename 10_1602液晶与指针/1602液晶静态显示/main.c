#include <reg52.h>
#define LCD1602_DB P0      //宏定义总线，切记不加分号
sbit LCD1602_RS = P1^0;    //数据/命令选择端（1/0） 
sbit LCD1602_RW = P1^1;    //读/写选择段（1/0）
sbit LCD1602_E = P1^5;     //LCD1602使能端（高电平有效）

void LCD1602init();        //函数声明
void LCD1602writecmd(unsigned char cmd);
void LCD1602readstate();
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);
void LCD1602setcoor(unsigned char x,unsigned char y);
void LCD1602writedate(unsigned char str);

void main()
{
	unsigned char str1[]={ //第一行需要显示的字符串1，共10个字符
		"Chen Hao Hao"
	};
	unsigned char str2[]={ //第二行需要显示的字符串1，共9个字符
		"2021/8/17"
	};
	LCD1602init();
	LCD1602show(2,0,str1);
	LCD1602show(2,1,str2);
	while(1);
}
/*LCD1602初始化函数*/
void LCD1602init()
{
	LCD1602writecmd(0X38); //显示模式设置
	LCD1602writecmd(0X0C); //显示开关设置
	LCD1602writecmd(0X06); //显示光标设置
	LCD1602writecmd(0X01); //显示清屏
}
/*LCD1602写指令函数*/
void LCD1602writecmd(unsigned char cmd)
{
	LCD1602readstate();    //写指令之前需要读状态，确保STA7=0
	LCD1602_RS = 0;
	LCD1602_RW = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 0;         //由于写指令只有在使能端有高脉冲时，才启动，这里软件设置高脉冲
	LCD1602_E = 1;
	LCD1602_E = 0;
}
/*LCD1602读状态函数*/
void LCD1602readstate()  //用于检测STA7（DB7）是否处于高电平
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
	while(sta7 & 0X80);    //当STA7最高位等于0时，可以进行下一步
}
/*LCD1602显示函数*/
void LCD1602show(unsigned char x,unsigned char y,unsigned char *p)
{
	LCD1602setcoor(x,y);
	while(*p != '\0')
	{
		LCD1602writedate(*p++); //*p++：先*p再p++
	}
}
/*LCD1602坐标定位函数*/
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
	LCD1602writecmd(add | 0X80);//设置数据指针地址：0X80+地址码
}
/*LCD1602写数据函数*/
void LCD1602writedate(unsigned char str)
{
	LCD1602readstate();         //写数据之间需要读状态，即检测STA7是否为0
	LCD1602_RS = 1;
	LCD1602_RW = 0;
	LCD1602_DB = str;
	LCD1602_E = 0;              //写数据使能端高脉冲触发，这里用软件设置一个高脉冲
	LCD1602_E = 1;
	LCD1602_E = 0;
}