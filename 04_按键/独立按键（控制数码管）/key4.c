#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit KEY = P2^7;                //定义一个按键
unsigned char code number[]={   //定义number数组存放0-9对应的16进制编码
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
void main()
{
	unsigned char i = 0;          
	bit backup = 1;               //backup为按键的上一个状态
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	P2 = 0XF7;
	P0 = number[i];
	while(1)
	{
		if(KEY != backup)           //当按键当前状态与上一个状态不同时进入
		{
			if(backup == 0)           //当按键上一个状态为0时进入
			{
			  i++;
			  if(i == 10)             //先判断后是否等于10，后点亮小灯
			  {
				  i = 0;
			  }
				P0 = number[i];         //点亮小灯
			}
			backup = KEY;             //完成判断后，将按键当前状态赋值给backup
		}
	}	
}