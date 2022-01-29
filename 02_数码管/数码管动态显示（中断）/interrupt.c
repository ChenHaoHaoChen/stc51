#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned char code number[]={
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char buffer[]={
	0xff,0xff,0xff,0xff,0xff,0xff
  };
unsigned int cnt = 0;
unsigned long sec = 0;
 unsigned char i = 0;
	
void main()
{ 
	ENLED = 0;    //74HC138使能端打开 
	ADDR3 = 1;
	
	TMOD = 0x01;  //打开定时器
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	
	EA=1;         //打开中断
	ET0 = 1;
	while(1)
	{
		if(cnt == 1000)
		{
			cnt = 0;
			sec++;
			buffer[0] = number[sec%10];
			if(sec<10)    {buffer[1] = 0xff;}
			else          {buffer[1] = number[sec/10%10];}
			
			if(sec<100)   {buffer[2] = 0xff;}
			else 	        {buffer[2] = number[sec/100%10];}
			
			if(sec<1000)  {buffer[3] = 0xff;}
			else 	        {buffer[3] = number[sec/1000%10];}
			
			if(sec<10000) {buffer[4] = 0xff;}
			else 	        {buffer[4] = number[sec/10000%10];}
			
			if(sec<100000){buffer[5] = 0xff;}
			else 	        {buffer[5] = number[sec/100000%10];}
			
		}
	}
}
void interrupttime() interrupt 1 //x*8+3=向量地址
{
	TH0 = 0xfc;
	TL0 = 0x67;
	cnt++;
	
	P0 = 0xff;
	
	switch(i)
		{
			case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = buffer[0];i++;break;
			case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = buffer[1];i++;break;
			case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = buffer[2];i++;break;
			case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = buffer[3];i++;break;
			case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = buffer[4];i++;break;
			case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = buffer[5];i=0;break;
		}
}




