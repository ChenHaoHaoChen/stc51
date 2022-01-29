/**************************
  EEPROM读写及初始化程序
**************************/
#include <reg52.h>
#include <intrins.h>
#include "I2C.h"
#include "main.h" 

/*读取EEPROM中一个字节，address为地址*/
unsigned char EEPROMreadbyte(unsigned char address)
{
	unsigned char dat;
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cstart();
	I2Cwrite((0X50<<1) | 0X01);
	dat = I2CreadNACK();
	I2Cstop();
	return dat;
}
/*写入EEPROM中一个字节，address为地址，dat为一字节数据*/
void EEPROMwritebyte(unsigned char address,unsigned char dat)
{
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cwrite(dat);
	I2Cstop();
}
/**********EEPROM初始化**********/
void EEPROMinit()
{
	 ELEFEE = EEPROMreadbyte(0X04);   //读取电费
	 if(ELEFEE > 999 || ELEFEE == 0) //若读取电费 >= 9.99，则默认设置为 0.58
	 {
		 ELEFEE = 52;
	 }
	 
	 ELECON = EEPROMreadbyte(0X06);   //读取用电量高八位 
	 ELECON = ELECON << 8 | EEPROMreadbyte(0X07); //读取用电量低八位 
	 if(ELECON >= 5000)
	 {
		 ELECON = 0;
	 }
}
/**********EEPROM写入用电量**********/
void ELECONaccident()
{
	EEPROMwritebyte(0X06,ELECON >> 8);
	EEPROMwritebyte(0X07,ELECON & 0X0F);
}