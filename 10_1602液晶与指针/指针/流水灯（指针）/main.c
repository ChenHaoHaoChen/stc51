#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
void pointer(unsigned char *p);
void main()
{
	unsigned char buffer = 0x01;
	unsigned int i;
	ADDR3 = 1;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR0 = 0;
	while(1)
	{
		P0 = ~buffer;
		for(i=0;i<30000;i++);
		pointer(&buffer);
	}
}
void pointer(unsigned char *p)
{
	*p = *p << 1;
	if(*p == 0x00)
	{
		*p = 0x01;
	}
}