/**************************
  EEPROM��д����ʼ������
**************************/
#include <reg52.h>
#include <intrins.h>
#include "I2C.h"
extern unsigned int ELEFEE; 
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
	 ELEFEE = EEPROMreadbyte(0X03);
}
///**********EEPROM��ȡ����,buf-���ݽ���ָ��,addr-EEPROM�е���ʼ��ַ��len-��ȡ����**********/
//void EEPROMread(unsigned char *buf, unsigned char addr, unsigned char len)
//{
//  do
//  {                         //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д����
//  I2Cstart();
//  if(I2Cwrite(0x50<<1))     //Ӧ��������ѭ������Ӧ���������һ�β�ѯ
//  {
//		 break;
//  }
//  I2Cstop();
//  }while(1);
//  I2Cwrite(addr);           //д����ʼ��ַ
//  I2Cstart();               //�����ظ������ź�
//  I2Cwrite((0x50<<1)|0x01); //Ѱַ����������Ϊ������
//  while (len > 1)           //������ȡ len-1 ���ֽ�
//  {
//		*buf++ = I2CreadACK();  //����ֽ�֮ǰΪ��ȡ����+Ӧ��
//		len--;
//	}
//	*buf = I2CreadNAK();      //���һ���ֽ�Ϊ��ȡ����+��Ӧ��
//	I2Cstop();
//}

///**********EEPROMд�뺯��,buf-Դ����ָ��,addr-EEPROM�е���ʼ��ַ,len-д�볤��**********/
//void EEPROMwrite(unsigned char *buf, unsigned char addr, unsigned char len)
//{
//	while(len > 0)
//	{
//                            //�ȴ��ϴ�д��������
//	do{                       //��Ѱַ������ѯ��ǰ�Ƿ�ɽ��ж�д����
//	I2Cstart();
//	if(I2Cwrite(0x50<<1))     //Ӧ��������ѭ������Ӧ���������һ�β�ѯ
//	{
//		 break;
//	}
//	I2Cstop();
//	}while(1);
//                            //��ҳдģʽ����д���ֽ�
//	I2Cwrite(addr);           //д����ʼ��ַ
//	while(len > 0)
//	{
//	I2Cwrite(*buf++);         //д��һ���ֽ�����
//	len--;                    //��д�볤�ȼ����ݼ�
//	addr++;                   //EEPROM��ַ����
//	if((addr&0x07) == 0)      //����ַ�Ƿ񵽴�ҳ�߽�,24C02 ÿҳ 8 �ֽڣ�
// {                          //���Լ��� 3 λ�Ƿ�Ϊ�㼴��
//		break;                  //����ҳ�߽�ʱ������ѭ������������д����
// }
// }
// I2Cstop();
// }
//}