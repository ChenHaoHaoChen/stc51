#include <reg52.h>
bit flag1s = 0;
unsigned char TH0R;
unsigned char TL0R;

void configtime0(unsigned char ms);
unsigned char inttostring(unsigned char *str,int dat);

extern bit DS18B20start();
extern bit DS18B20gettemp(int *temp);
extern void LCD1602init();
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);

void main()
{
	bit res;
	int temp;
	int integer,decimal;
	unsigned char len;
	unsigned char str[12];
	EA = 1;
	configtime0(10);
	DS18B20start();
	LCD1602init();
	while(1)
	{
		if(flag1s)
		{
			flag1s = 0;
			res = DS18B20gettemp(&temp);
			if(res)
			{
				integer = temp >> 4;
				decimal = temp &0XF;
				len =inttostring(str,integer);
				str[len++] = '.';
				decimal = (decimal*10)/16;
				str[len++] = decimal + '0';
				while(len < 6)
				{
					str[len++] = ' ';
				}
				str[len] = '\0';
				LCD1602show(2,0,"Temperature");
				LCD1602show(2,1,str);
				LCD1602show(6,1,"'C");
			}
			else
			{
				LCD1602show(0,0,"error!");
			}
			DS18B20start();
		}
	}
}

unsigned char inttostring(unsigned char *str,int dat)
{
	signed char i = 0;
	unsigned char len = 0;
	unsigned char buffer[6];
	if(dat <0)
	{
		dat = -dat;
		*str++ = '-';
		len++;
	}
	do
	{
		buffer[i++] = dat % 10;
		dat = dat / 10;
	}
	while(dat>0);
	len = len + i;
	while(i-->0)
	{
		*str++ = buffer[i] + '0';
	}
	*str = '\0';
	return len;
}

void configtime0(unsigned char ms)
{
	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	TH0R = (unsigned char)(tmp >> 8);
	TL0R = (unsigned char)(tmp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}

void interrupttime0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt >= 100)
	{
		cnt = 0;
		flag1s = 1;
	}
}