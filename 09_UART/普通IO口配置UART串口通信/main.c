#include <reg52.h>
sbit PINRXD = P3^0;
sbit PINTXD = P3^1;

bit rxdend = 0;
bit txdend = 0;
bit rxdortxd = 0;
unsigned char rxdbuffer = 0;
unsigned char txdbuffer = 0;

void configbaud(unsigned int baud);
void startrxd();
void starttxd(unsigned char date);

void main()
{
	EA = 1;
	configbaud(9600);
	while(1)
	{
		while(PINRXD);
		startrxd();
		while(!rxdend);
		starttxd(rxdbuffer + 1);
		while(!txdend);
	}		
}
void configbaud(unsigned int baud)
{
	TMOD = 0X02;
	TH0 = 256 - (11059200/12)/baud;
}
void startrxd()
{
	TL0 = 256 - ((256 - TH0) >> 1);
	TR0 = 1;
	ET0 = 1;
	rxdend = 0;
	rxdortxd = 0;
}
void starttxd(unsigned char date)
{
	txdbuffer = date;
	TL0 = TH0;
	TR0 = 1;
	ET0 = 1;
	PINTXD = 0;
	txdend = 0;
	rxdortxd = 1;
}

void interrupttime0() interrupt 1
{
	static unsigned char cnt;
	
	if(rxdortxd)
	{
		cnt++;
		if(cnt <= 8)
		{
			PINTXD = txdbuffer & 0X01;
			txdbuffer = txdbuffer >> 1;
		}
		else if(cnt == 9)
		{
			PINTXD = 1;
		}
		else
		{
			cnt = 0;
			TR0 = 0;
			txdend = 1;
		}
	}
	else
	{
		if(cnt == 0)
		{
			if(!PINRXD)
			{
				rxdbuffer = 0;
				cnt++;
			}
			else
			{
				TR0 = 0;
			}
		}
		else if(cnt <= 8)
		{
			rxdbuffer = rxdbuffer >> 1;
			if(PINRXD)
			{
				rxdbuffer = rxdbuffer | 0x80;				
			}
			cnt++;
		}
		else
		{
			cnt = 0;
			TR0 = 0;
			if(PINRXD)
			{
				rxdend = 1;
			}
		}
	}
}