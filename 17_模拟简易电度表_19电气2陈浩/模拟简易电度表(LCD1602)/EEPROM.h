#ifndef _EEPROM_H
#define _EEPROM_H
extern void EEPROMinit();                                            //EEPROM��ʼ��
extern void ELECONaccident();                                        //�������ָ��õ���
extern unsigned char EEPROMreadbyte(unsigned char address);          //EEPROM��һ���ֽ�
extern void EEPROMwritebyte(unsigned char address,unsigned char dat);//EEPROMдһ���ֽ�                                         
#endif