#ifndef _KEY_H
#define _KEY_H
extern bit bit_start;              //电度表起动状态数码管显示标志位
extern bit bit_time;               //电度表时间状态数码管显示
extern bit bit_end;                //电度表结费状态数码管显示标志位
extern bit bit_change;             //电度表修改状态数码管显示标志位
extern bit bit_extra;
extern unsigned char keyfinally[4];//按键终值(1为弹起，0为按下)
extern void keyscan();             //按键扫描
extern void keybuff();             //按键消抖
#endif