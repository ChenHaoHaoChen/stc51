#ifndef _MAIN_H
#define _MAIN_H
/******************全局变量声明******************/
extern unsigned int ELEFEE;  //电费,默认为0.52元/度
extern unsigned int ELECON;  //用电量
extern unsigned int FEE;     //总电费
extern unsigned char time[8];//DS1302读出时间暂存

/**********液晶显示LCD1602模块引脚定义***********/
#define LCD1602DB P0    //定义LCD1602总线
sbit LCD1602RS = P1^0;  //RS为数据/命令选择端（1/0）
sbit LCD1602RW = P1^1;  //RW为读/写选择端（1/0）
sbit LCD1602E = P1^2;   //E为使能端
/*************运行指示灯模块引脚定义*************/
sbit L1 = P1^3;
/**************照明灯灯模块引脚定义**************/
sbit D1501 = P1^4;      //D1501照明灯
/***************AD转换模块引脚定义***************/
sbit CLK = P2^0;
sbit ADO = P2^1;
sbit CS = P2^2;
/***********实时时钟DS1302模块引脚定义***********/
sbit SCE = P2^3;
sbit SCK = P2^4;
sbit SIO = P2^5;
/***************EEPROM模块引脚定义***************/
sbit SCL = P2^6;
sbit SDA = P2^7;
/****************按键模块引脚定义****************/
sbit KEY1 = P3^4;        //K17按键，控制电度表启动状态显示
sbit KEY2 = P3^5;        //K18按键，控制电度表时钟状态显示
sbit KEY3 = P3^6;        //K19按键，控制电度表电费结算显示
sbit KEY4 = P3^7;        //K20按键，控制电度表电费修改显示
#endif