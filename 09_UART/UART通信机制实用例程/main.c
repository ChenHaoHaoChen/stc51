#include <reg52.h>
sbit BUZZ = P1^6;

void main()
{
	EA = 1;
	configtime0(1);
	configbuad(9600);
	LCD1602init();
}