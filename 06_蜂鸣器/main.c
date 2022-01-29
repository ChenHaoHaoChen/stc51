#include <reg52.h>

sbit BUZZ = P1^6;

void main()
{
	TMOD = 0X01;
	TH0 = 0XFE;
	TL0 = 0X33;
	TR0 = 1;
	EA = 1;
	ET0 = 1;
	BUZZ = 0;
	while(1);
}
void interrupttime0() interrupt 1
{
	TH0 = 0XFE;
	TL0 = 0X33 ;
	BUZZ = ~BUZZ;
}