//存在点阵鬼影以及LED鬼影，由于中断0和中断1分别以1ms和4ms刷新，
//当中断0刷新时会对中断1控制的点阵产生影响
//当中断1刷新时会对中断0控制的数码管和LED产生影响
//待改进：2021.7.23
#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned int cnt = 0;
unsigned long sec = 0;
unsigned char code number[]={        //数码管显示译码
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char buffer[]={
	0xff,0xff,0xff,0xff,0xff,0xff,0xff //数码管显示缓冲区
  };
unsigned char image[]={              //点阵显示数组
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xC3,0xE7,0xE7,0xE7,0xE7,0xE7,0xC3,
  0xFF,0x99,0x00,0x00,0x00,0x81,0xC3,0xE7,
	0xFF,0x99,0x99,0x99,0x99,0x99,0x81,0xC3,
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
 };
void main()
{
	static unsigned char j1 = 0;       //j用于LED显示移位变量                       
	EA = 1;                            //打开T0与T1中断
	ET0 = 1;
	ET1 = 1;
	
	TMOD = 0x01;
	TH0 = 0xfc;                        //T0定时器，定时1ms
	TL0 = 0x67;
	TR0 = 1;
	
	TH1 = 0xf1;                        //T1定时器，定时4ms
	TL1 = 0x9a;
	TR1 = 1;
	
	ENLED = 0;                         //74138低电平使能端打开
	
	while(1)
	{
		if(cnt == 1000)                  //对1ms计数，当cnt=1000,即1s时，用buffer数组存放数码管1-6存放的数字
		{
			cnt = 0;
			sec++;
			buffer[0] = number[sec%10];
			buffer[1] = number[sec/10%10];
			buffer[2] = number[sec/100%10];
			buffer[3] = number[sec/1000%10];
			buffer[4] = number[sec/10000%10];
			buffer[5] = number[sec/100000%10];
			buffer[6] = ~(0x01<<j1);      //led显示部分
			j1++;
			if(j1 == 8)
			{
				j1 = 0;
			}
		}
	}
}

void interrupttime0() interrupt 1  //中断0用于显示数码管和LED，1ms刷新一次
{
	static unsigned char i1 = 0;     //静态变量初次进入赋值0，下次进入不再赋值0，可直接跳过
	ADDR3 = 1;
	TH0 = 0xfc;
	TL0 = 0x67;
	P0 = 0xff;
	cnt++;
	switch(i1)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = buffer[0];i1++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = buffer[1];i1++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = buffer[2];i1++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = buffer[3];i1++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = buffer[4];i1++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = buffer[5];i1++;break;
		case 6:ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;P0 = buffer[6];i1=0;break;
	}
}

void interrupttime1() interrupt 3  //中断1用于显示点阵，4ms刷新一次
{
	static unsigned char i2 = 0;     //静态变量初次进入赋值0，下次进入不再赋值0，可直接跳过
	static unsigned char j2 = 0;
	static unsigned char tms = 0;
	ADDR3 = 0;
	TH1 = 0xf1;
	TL1 = 0x9a;
	P0 = 0xff;
	switch(i2)
	{
		case 0 :ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = image[j2+0];i2++;break;
		case 1 :ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = image[j2+1];i2++;break;
		case 2 :ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = image[j2+2];i2++;break;
		case 3 :ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = image[j2+3];i2++;break;
		case 4 :ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = image[j2+4];i2++;break;
		case 5 :ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = image[j2+5];i2++;break;
	  case 6 :ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;P0 = image[j2+6];i2++;break;
		case 7 :ADDR2 = 1;ADDR1 = 1;ADDR0 = 1;P0 = image[j2+7];i2=0;break;
	}
	tms++;
	if(tms == 50)
	{
		j2++;
		if(j2 == 32)
		{
			j2 = 0;
		}
	}
}
	


