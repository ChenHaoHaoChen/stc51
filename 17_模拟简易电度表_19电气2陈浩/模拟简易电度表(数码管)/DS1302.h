#ifndef _DS1302_H
#define _DS1302_H
extern void DS1302init();
void DS1302burstread(unsigned char *date);
#endif