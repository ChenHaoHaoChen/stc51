#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit key4 = P2^7;
unsigned char code number[]={   //定义number数组存放0-9对应的16进制编码
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char keyfinally = 1;   //keyfinally为消抖完毕按键最终值
void main()
{
	unsigned char backup = 1;     //backup为按键上一个理想状态，抖动不算
	unsigned char i = 0;          //数码管显示变量
	ENLED = 0;                    //打开74138译码器
	ADDR3 = 1;
	ADDR2 = 0;                    //打开最低为数码管
	ADDR1 = 0;
	ADDR0 = 0;
	TMOD = 0x01;                  //TO定时器设置为1模式
	TH0 = 0xf8;                   //定时2ms
	TL0 = 0xcd;
	TR0 = 1;                      //开始计时 
	EA = 1;                       //打开定时T0中断
	ET0 = 1;
	P2 = 0xf7;//若是按键有效，keyIn1-4必须输出1(高电平)，
	          //keyOut1-4有且只能有一个输出0，其他必须为1，谁为0，对应那一行按键就有效
	P0 = number[i];               //显示最低位数码管
	while(1)
	{
		if(keyfinally != backup)
		{
			if(backup == 0)
			{
				i++;
				if(i == 10)
				{i = 0;}
				P0 = number[i];
			}
			backup = keyfinally;
		}
	}
}
void interrupttime0() interrupt 1  //1为x，x*8+3=向量地址
{
	static unsigned char keybuffer = 0xff;//用于检测连续8个状态，每个状态2ms，是否全为1或0
	TH0 = 0xf8;
	TL0 = 0xcd;
	keybuffer = (keybuffer << 1) | key4;
	if(keybuffer == 0x00)
	{
		keyfinally = 0;                //连续8个状态全为0时，默认按键为0(按下)
	}
	else if(keybuffer == 0xff)
	{
		keyfinally = 1;                //连续8个状态全为1时，默认按键为1(弹起)
	}
	else
	{}
}