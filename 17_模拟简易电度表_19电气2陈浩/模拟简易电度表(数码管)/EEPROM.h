#ifndef _EEPROM_H
#define _EEPROM_H
extern unsigned char EEPROMreadbyte(unsigned char address);
extern void EEPROMwritebyte(unsigned char address,unsigned char dat);
extern void EEPROMinit();
#endif