/**************************
 按键扫描、动作、消抖程序
**************************/
#include <reg52.h>
#include "main.h"
#include "LCD1602.h"

bit bit_start = 0; //电度表起动状态数码管显示标志位
bit bit_time = 0;  //电度表时间状态数码管显示标志位
bit bit_end = 0;   //电度表结费状态数码管显示标志位
bit bit_change = 0;//电度表修改状态数码管显示标志位
bit bit_extra = 1; 
unsigned char keyfinally[4] = {  //按键消抖后终值(1为弹起，0为按下)
	1,1,1,1
};
unsigned char keyback[4] = {     //按键消抖后上时刻值(1为弹起，0为按下)
	1,1,1,1
};

void keyaction(unsigned char i);

/**********按键扫描函数**********/
void keyscan()
{
	static unsigned char i = 0;
	if(keyfinally[i] != keyback[i])
	{
		if(keyback[i] == 0)
		{
			keyaction(i);
		}
		keyback[i] = keyfinally[i];
	}
	i++;
	if(i == 4)  //循环扫描四个按键
	{
		i = 0;
	}
}
/**********按键动作函数**********/
void keyaction(unsigned char i)
{
	if(i == 0)      //功能0：启动状态数码管显示
	{
		bit_start = 1;
		bit_time = 0;
		bit_end = 0;
		LCD1602clearscreen();
	}
	else if(i == 1) //功能1：时间状态数码管显示
	{
		bit_start = 0;
		bit_time = 1;
		bit_end = 0;
		if(bit_change == 1 && bit_extra == 0) //电费加一
		{
			ELEFEE++;
			bit_start = 0;
			bit_time = 0;
			bit_end = 0;
		}
		LCD1602clearscreen();
	}
	else if(i == 2) //功能2：结费状态数码管显示
	{
		bit_start = 0;
		bit_time = 0;
		bit_end = 1;
		if(bit_change == 1 && bit_extra == 0) //电费减一
		{
			ELEFEE--;
			bit_start = 0;
			bit_time = 0;
			bit_end = 0;
		}
		LCD1602clearscreen();
	}
	else if(i == 3) //功能3：修改电费数码管显示
	{
		bit_start = 0;
		bit_time = 0;
		bit_end = 0;
		bit_change = ~bit_change;
		LCD1602clearscreen();
	}
}
/**********按键消抖函数**********/
void keybuff()
{
	static unsigned char i;
	static unsigned char keybuffer[] = {
	0XFF,0XFF,0XFF,0XFF
	};
	keybuffer[0] = keybuffer[0] << 1 | KEY1;
	keybuffer[1] = keybuffer[1] << 1 | KEY2;
	keybuffer[2] = keybuffer[2] << 1 | KEY3;
	keybuffer[3] = keybuffer[3] << 1 | KEY4;
  for(i=0;i<4;i++) 
	{
		if(keybuffer[i] == 0XFF)
		{
			keyfinally[i] = 1;
		}
		else if(keybuffer[i] == 0X00)
		{
			keyfinally[i] = 0;
		}
	}
}