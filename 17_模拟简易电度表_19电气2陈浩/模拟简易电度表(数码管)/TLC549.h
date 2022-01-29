#ifndef _TLC549_H
#define _TLC549_H
extern unsigned char RV;                 //光敏电阻电压采集值
extern void ADscan();
extern void ADrefresh();
unsigned char ADsampling();
#endif