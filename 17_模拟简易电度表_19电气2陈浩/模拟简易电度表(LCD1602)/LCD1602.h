#ifndef _LCD1602_H
#define _LCD1602_H
extern void LCD1602init();       //LCD1602初始化
extern void LCD1602clearscreen();//LCD1602清屏
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);//LCD1602显示
#endif