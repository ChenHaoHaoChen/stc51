#ifndef _LCD1602_H
#define _LCD1602_H
extern void LCD1602init();       //LCD1602��ʼ��
extern void LCD1602clearscreen();//LCD1602����
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);//LCD1602��ʾ
#endif