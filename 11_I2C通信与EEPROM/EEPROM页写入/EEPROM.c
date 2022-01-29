/*****************
EEPROMÅäÖÃº¯Êý
*****************/
#include <reg52.h>

extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();
extern unsigned char I2CreadNACK();
extern unsigned char I2CreadACK();
/*EEPROM¶Á²Ù×÷º¯Êý*/
void E2read(unsigned char address,unsigned char *date,unsigned char len)
{
	do
	{
		I2Cstart();
		if( (I2Cwrite(0X50<<1)) == 0 )
		{
			break;
		}
		I2Cstop();
	}while(1);
	I2Cwrite(address);
	I2Cstart();
	I2Cwrite( (0X50<<1) | 0X01 );
	while(len > 1)
	{
		*date++ = I2CreadACK();
		len--;
	}
	*date = I2CreadNACK();
	I2Cstop();
}
/*EEPROMÐ´²Ù×÷º¯Êý*/
void E2write(unsigned char address,unsigned char *date,unsigned char len)
{
	while(len>0)
	{
		do
		{
			I2Cstart();
			if( (I2Cwrite(0X50<<1)) == 0 )
			{
				break;
			}
			I2Cstop();
		}while(1);
		I2Cwrite(address);
		while(len>0)
		{
			I2Cwrite(*date++);
			len--;
			address++;
			if((address & 0X07) == 0)
			{
				break;
			}
		}
		I2Cstop();
	}
}