#include <reg52.h>

extern void LCD1602init();
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);

extern void E2read(unsigned char address,unsigned char *date,unsigned char len);
extern void E2write(unsigned char address,unsigned char *date,unsigned char len);

void numtostring(unsigned char *buf,unsigned char *st,unsigned char len);

void main()
{
	unsigned char i;
	unsigned char buffer[5];
	unsigned char str[20];
	
	LCD1602init();
	E2read(0X00,buffer,sizeof(buffer));
	numtostring(buffer,str,sizeof(buffer));
	LCD1602show(0,0,str);
	for(i=0;i<sizeof(buffer);i++)
	{
		buffer[i] = buffer[i] + i + 1;
	}
	E2write(0X00,buffer,sizeof(buffer));
	while(1);
}

void numtostring(unsigned char *buf,unsigned char *st,unsigned char len)
{
	unsigned char temp;
	while(len--)
	{
		temp = *buf >> 4;
		if(temp <= 9)
		{
			*st++ = temp + '0';
		}
		else
		{
			*st++ = temp - 10 + 'A';
		}
		temp = 0X0F & *buf;
		if(temp <= 9)
		{
			*st++ = temp + '0';
		}
		else
		{
			*st++ = temp - 10 + 'A';
		}
		*st++ = ' ';
		buf++;
	}
	
}