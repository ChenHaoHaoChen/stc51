#ifndef _DS1302_H
#define _DS1302_H
extern void DS1302init();                         //DS1302初始化
extern void DS1302burstread(unsigned char *date); //DS1302burst模式读
#endif