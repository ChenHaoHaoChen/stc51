#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned char code lednumber[]={     //数码管显示驱动    
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char ledbuffer[6]={         //数码管显示缓冲区
	0XFF,0XFF,0XFF,0XFF,0XFF,0XFF
 };
unsigned char TH0R = 0;              //定时器T0重载值
unsigned char TL0R = 0;
unsigned long number;                //number为电脑端传递过来的十六进制字节
void configbuad(unsigned int buad);  //函数声明
void configtime0(unsigned char ms); 
void main()
{
	ENLED = 0;                         //打开74138译码器
	ADDR3 = 1;
	EA = 1;                            //打开中断总使能
	configtime0(1);                    //定时器T0定时1ms
	configbuad(9600);                  //配置波特率为9600
	while(1)
	{                                  //将接收字节在数码管以十六禁止显示出来
		ledbuffer[0] = lednumber[number & 0X0F]; 
		ledbuffer[1] = lednumber[number >> 4];
	}
}
/*配置定时器T0定时1ms*/
void configtime0(unsigned char ms)
{
	unsigned long temp;
	TMOD = TMOD & 0XF0;                //设置定时器T0为1模式
	TMOD = TMOD | 0X01;
	temp = 65536-(11059200/12*ms/1000);//计算定时1ms起点
	TH0R = (unsigned char)(temp >> 8); //计算定时1ms高八位和低八位
	TL0R = (unsigned char)temp;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;                           //开始定时
	ET0 = 1;                           //打开定时器T0中断
}
/*配置通信波特率*/
void configbuad(unsigned int buad)
{
	SCON = 0X50;                       //配置串口为模式1
	TMOD = TMOD & 0X0F;                //设置定时器T2为模式2
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12/32)/buad; //计算波特率
	TL1 = TH1;
	TR1 = 1;                           //定时器T1开始定时
	ES = 1;                            //打开UART串口中断
}
/*进入UART串口中断*/
void interruptUART() interrupt 4
{
	if(RI)                             //为接受状态
	{
		RI = 0;
		SBUF = SBUF;                     //在串口调试助手中也显示
		number = SBUF;                  
	}
	else if(TI)                        //为发送状态
	{
		TI = 0;
	}
}
/*每1ms进入定时器T0中断*/
void interrupttime0() interrupt 1
{
	static unsigned char i = 0; 
	TH0 = TH0R;                        //重新配置定时时间
	TL0 = TL0R;
	P0 = 0XFF;                         //数码管消除鬼隐
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[0];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[1];i=0;break;
		default:break;
	}
}