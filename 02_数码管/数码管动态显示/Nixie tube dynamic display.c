#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned char code number[]={   //定义number数组存放0-9对应的16进制编码
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char buffer[]={       //数码管显示缓冲存放数组
	0xff,0xff,0xff,0xff,0xff,0xff
  };
void secondcount();
void ledrefresh();
void main()
{  
	ENLED = 0;
	ADDR3 = 1;
	TMOD = 0x01;                  //1ms定时
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	
	while(1)
	{
		if (TF0 == 1)
		{
			TF0 = 0;
			TH0 = 0xfc;
			TL0 = 0x67;
      secondcount();
		}				
	    ledrefresh();  
	}
}


void secondcount()
{
	static unsigned int cnt = 0;         //对10ms计数，cnt=1000即1s清零
	static unsigned long sec = 0;        //数码管显示计数，1秒加一
	cnt++;
			if (cnt == 1000)
			{
				cnt = 0;
				sec++;
				buffer[0] = number[sec%10];        //显示缓冲
				buffer[1] = number[sec/10%10];     //十位
				buffer[2] = number[sec/100%10];    //百位
				buffer[3] = number[sec/1000%10];   //千位
				buffer[4] = number[sec/10000%10];  //万位
		    buffer[5] = number[sec/100000%10]; //十万位
      }
}
void ledrefresh()
{
	static unsigned char i = 0; 
	P0 = 0xff;     //消除数码管上鬼影 			
	switch (i) 
			 {				 //每1ms进入一次，点亮一个数码管
				 case 0 : ADDR2 = 0; ADDR1 = 0; ADDR0 = 0; P0 = buffer[0]; i++; break ;
			   case 1 : ADDR2 = 0; ADDR1 = 0; ADDR0 = 1; P0 = buffer[1]; i++; break ;
			   case 2 : ADDR2 = 0; ADDR1 = 1; ADDR0 = 0; P0 = buffer[2]; i++; break ;
			   case 3 : ADDR2 = 0; ADDR1 = 1; ADDR0 = 1; P0 = buffer[3]; i++; break ;
			   case 4 : ADDR2 = 1; ADDR1 = 0; ADDR0 = 0; P0 = buffer[4]; i++; break ;
			   case 5 : ADDR2 = 1; ADDR1 = 0; ADDR0 = 1; P0 = buffer[5]; i = 0; break ;
			 }
}	