/********************
用I2C通信对EEPROM的多字节写入
********************/
#include <reg52.h>

extern void LCD1602init();
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);

extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();
extern unsigned char I2CreadACK();
extern unsigned char I2CreadNACK();

void I2CEEPROMread(unsigned char address,unsigned char *buffer,unsigned char len);
void I2CEEPROMwrite(unsigned char address,unsigned char *buffer,unsigned char len);
void numtostring(unsigned char *str,unsigned char *buffer,unsigned char len);

void main()
{
	unsigned char buffer[5];
	unsigned char str[20];
	unsigned char i;
	
	LCD1602init();
	
	I2CEEPROMread(0X00,buffer,sizeof(buffer));
	
	numtostring(str,buffer,sizeof(buffer));
	LCD1602show(0,0,str);
	
	for(i=0;i<sizeof(buffer);i++)
	{
		buffer[i] = buffer[i] + i +1;
	}
	I2CEEPROMwrite(0X00,buffer,sizeof(buffer));
	while(1);
}

void I2CEEPROMread(unsigned char address,unsigned char *buffer,unsigned char len)
{
	do
	{
		I2Cstart();
		if( (~I2Cwrite(0X50<<1)) )
		{
			break;
		}
		I2Cstop();
	}
	while(1);
	I2Cwrite(address);
	I2Cstart();
	I2Cwrite((0X50<<1) | 0X01);
	while(len>1)
	{
		*buffer++ = I2CreadACK();
		len--;
	}
	*buffer = I2CreadNACK();
	I2Cstop();
}

void I2CEEPROMwrite(unsigned char address,unsigned char *buffer,unsigned char len)
{
	while(len--)
	{
		do
	  {
		I2Cstart();
		if( (~I2Cwrite(0X50<<1)) )
		  {
			  break;
		  }
		I2Cstop();
	  }
		while(1);
	  I2Cwrite(address++);
	  I2Cwrite(*buffer++);
	  I2Cstop();
	}
}

void numtostring(unsigned char *str,unsigned char *buffer,unsigned char len)
{
	unsigned char temp;
	while(len--)
	{
		temp = *buffer >>4;
		if(temp <= 9)
		{
			*str++ = temp + '0';
		}
		else
		{
			*str++ = temp - 10 + 'A';
		}
		temp = *buffer & 0X0F;
		if(temp <= 9)
		{
			*str++ = temp + '0';
		}
		else
		{
			*str++ = temp - 10 + 'A';
		}
		*str++ = ' ';
		buffer++;
	}
	//*str = '\0';
}















