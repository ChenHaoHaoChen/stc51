#include <reg52.h>

sbit LED2 = P0^0;
sbit ENLED = P1^4;
sbit ADDR3 = P1^3;
sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;

void main()
{
	unsigned int i;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	while(1)
	{
		LED2 = 0;
		for(i=1;i<=10000;i++)
		LED2 = 1;
		for(i=1;i<=10000;i++)
	}
}
