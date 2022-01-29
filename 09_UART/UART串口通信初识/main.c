#include <reg52.h>
void configbuad(unsigned int buad);//函数声明
void main()
{
	EA = 1;            //打开总中断
	ES = 1;            //打开UART串口中断
  SCON = 0X50;       //设置串口通信为1模式
	configbuad(9600);  //配置波特率为9600
	while(1);
}
void configbuad(unsigned int buad)
{
	TMOD = TMOD & 0X0F;//设置定时器1为模式2，即自动1重装模式
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12)/32/buad;
	TL1 = TH0;
	TR1 = 1;           //定时器1开始定时
}
void interruptUART() interrupt 4
{
	if(TI)             //RI为接受中断标志，且需要软件清零
	{
		TI = 0;
	}
	else if(RI)        //TI为发送中断标志，且需要软件清零
	{
		RI = 0;
		SBUF = SBUF + 1; //SBUF为接受到的数据
	}
}