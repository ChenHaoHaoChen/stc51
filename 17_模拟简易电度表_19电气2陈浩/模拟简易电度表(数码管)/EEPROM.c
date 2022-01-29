/**************************
  EEPROM读写及初始化程序
**************************/
#include <reg52.h>
#include <intrins.h>
#include "I2C.h"
extern unsigned int ELEFEE; 
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
	 ELEFEE = EEPROMreadbyte(0X03);
}
///**********EEPROM读取函数,buf-数据接收指针,addr-EEPROM中的起始地址，len-读取长度**********/
//void EEPROMread(unsigned char *buf, unsigned char addr, unsigned char len)
//{
//  do
//  {                         //用寻址操作查询当前是否可进行读写操作
//  I2Cstart();
//  if(I2Cwrite(0x50<<1))     //应答则跳出循环，非应答则进行下一次查询
//  {
//		 break;
//  }
//  I2Cstop();
//  }while(1);
//  I2Cwrite(addr);           //写入起始地址
//  I2Cstart();               //发送重复启动信号
//  I2Cwrite((0x50<<1)|0x01); //寻址器件，后续为读操作
//  while (len > 1)           //连续读取 len-1 个字节
//  {
//		*buf++ = I2CreadACK();  //最后字节之前为读取操作+应答
//		len--;
//	}
//	*buf = I2CreadNAK();      //最后一个字节为读取操作+非应答
//	I2Cstop();
//}

///**********EEPROM写入函数,buf-源数据指针,addr-EEPROM中的起始地址,len-写入长度**********/
//void EEPROMwrite(unsigned char *buf, unsigned char addr, unsigned char len)
//{
//	while(len > 0)
//	{
//                            //等待上次写入操作完成
//	do{                       //用寻址操作查询当前是否可进行读写操作
//	I2Cstart();
//	if(I2Cwrite(0x50<<1))     //应答则跳出循环，非应答则进行下一次查询
//	{
//		 break;
//	}
//	I2Cstop();
//	}while(1);
//                            //按页写模式连续写入字节
//	I2Cwrite(addr);           //写入起始地址
//	while(len > 0)
//	{
//	I2Cwrite(*buf++);         //写入一个字节数据
//	len--;                    //待写入长度计数递减
//	addr++;                   //EEPROM地址递增
//	if((addr&0x07) == 0)      //检查地址是否到达页边界,24C02 每页 8 字节，
// {                          //所以检测低 3 位是否为零即可
//		break;                  //到达页边界时，跳出循环，结束本次写操作
// }
// }
// I2Cstop();
// }
//}