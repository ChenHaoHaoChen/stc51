#ifndef _EEPROM_H
#define _EEPROM_H
extern void EEPROMinit();                                            //EEPROM初始化
extern void ELECONaccident();                                        //意外掉电恢复用电量
extern unsigned char EEPROMreadbyte(unsigned char address);          //EEPROM读一个字节
extern void EEPROMwritebyte(unsigned char address,unsigned char dat);//EEPROM写一个字节                                         
#endif