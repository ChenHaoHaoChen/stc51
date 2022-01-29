#include <reg52.h>

sbit ADDR3 = P1^3;
sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;
sbit ENLED = P1^4;

void main()
{
	unsigned int i;
	unsigned char cnt=0;
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	
	while(1)
	{
		//P0 = ~(0x01<<cnt); 
		//for(i=0;i<=30000;i++);
		//cnt++;
		//if(cnt==8)
		//{
		//	cnt=0;
		//}

		for(cnt=0;cnt<=8;cnt++)
		{
	  P0 =~(0x01<<cnt);
	  for(i=0;i<=30000;i++);
		}
		
		for(cnt=0;cnt<=8;cnt++)
		{
			P0 =~(0x80>>cnt);
			for(i=0;i<=30000;i++);
		}
		
	}
}