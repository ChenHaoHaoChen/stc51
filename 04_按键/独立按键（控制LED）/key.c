#include <reg52.h>

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

sbit LED2 = P0^0;
sbit LED3 = P0^1;
sbit LED4 = P0^2;
sbit LED5 = P0^3;

sbit KEY1 = P2^4;//不按动按键时，KEY=1;当按下按键时，KEY=0;
sbit KEY2 = P2^5;
sbit KEY3 = P2^6;
sbit KEY4 = P2^7;

void main()
{
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	P2 = 0xf7;    //0XF7=1111 0111，即keyout1 = 0；将矩阵按键转换为独立按键
	              //同一时刻，只能有一个keyout = 0;
	while(1)
	{
		LED2 = KEY1;
		LED3 = KEY2;
		LED4 = KEY3;
		LED5 = KEY4;
	}
}