#include <reg52.h>

sbit LED = P0^0;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void main()
{
	unsigned int cnt=0;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	
	LED = 0;
	
	TMOD = 0x01;       //TMOD设置为0x01即为1模式
	TH0 = 0xdc;        //TH0初值为0xdc，到0xff为10ms
	TL0 = 0x00;
	TR0 = 1;           //TRO置1，定时器开始定时
	
	while(1)
	{
		if (TF0 == 1)    //当TF0=1，即定时器发生溢出
		{
			TH0 = 0xdc;    //复位
			TL0 = 0x00;    //复位
			TF0 = 0;       //复位
			cnt++;         //cnt自加
			if (cnt == 600)//当cnt=600，即定时600*10ms=1s
			{
				cnt = 0;     //cnt复位
				LED=~LED;    //灭6s后LED亮六秒，以此反复 
			}
		}
	}
}