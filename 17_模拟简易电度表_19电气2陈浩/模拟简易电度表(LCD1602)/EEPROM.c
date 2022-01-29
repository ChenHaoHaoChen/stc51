/**************************
  EEPROM��д����ʼ������
**************************/
#include <reg52.h>
#include <intrins.h>
#include "I2C.h"
#include "main.h" 

/*��ȡEEPROM��һ���ֽڣ�addressΪ��ַ*/
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
/*д��EEPROM��һ���ֽڣ�addressΪ��ַ��datΪһ�ֽ�����*/
void EEPROMwritebyte(unsigned char address,unsigned char dat)
{
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cwrite(dat);
	I2Cstop();
}
/**********EEPROM��ʼ��**********/
void EEPROMinit()
{
	 ELEFEE = EEPROMreadbyte(0X04);   //��ȡ���
	 if(ELEFEE > 999 || ELEFEE == 0) //����ȡ��� >= 9.99����Ĭ������Ϊ 0.58
	 {
		 ELEFEE = 52;
	 }
	 
	 ELECON = EEPROMreadbyte(0X06);   //��ȡ�õ����߰�λ 
	 ELECON = ELECON << 8 | EEPROMreadbyte(0X07); //��ȡ�õ����Ͱ�λ 
	 if(ELECON >= 5000)
	 {
		 ELECON = 0;
	 }
}
/**********EEPROMд���õ���**********/
void ELECONaccident()
{
	EEPROMwritebyte(0X06,ELECON >> 8);
	EEPROMwritebyte(0X07,ELECON & 0X0F);
}