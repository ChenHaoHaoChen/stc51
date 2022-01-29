#ifndef _T0T1_H
#define _T0T1_H
extern unsigned char TH0R,TL0R;           //T0高低八位重载值
extern unsigned char TH1R,TL1R;           //T1高低八位重载值
extern void configtime0(unsigned char ms);//T0定时器配置函数
extern void configtime1(unsigned char ms);//T1定时器配置函数
#endif