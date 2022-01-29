#include <reg52.h>
#include <intrins.h> 
#define delay() {_nop_();_nop_();_nop_();_nop_();}//delay()函数为延时4个机械周期
sbit SDA = P3^6;
sbit SCL = P3^7;

extern void LCD1602init();//外部函数声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
unsigned char I2Caddress(unsigned char address);//函数声明

void main()
{
	unsigned char ACK;      //ACK变量为接受方发送的数据，ACK = 0：表示此地址存在，反之，不存在
	unsigned char str[10];  //LCD1602显示数组
	LCD1602init();          //LCD1602初始化
	ACK = I2Caddress(0X50); //用I2C通信发送地址
	str[0] = '0';
	str[1] = 'X';
	str[2] = '5';
	str[3] = '0';
	str[4] = ':';
	str[5] = ACK+'0';       //将ACK转换为字符形式
	str[6] = '\0';
	LCD1602show(0,0,str);   //LCD1602显示 地址：0/1（0：地址存在；1：地址不存在）
	
  ACK = I2Caddress(0X66);
	str[0] = '0';
	str[1] = 'X';
	str[2] = '6';
	str[3] = '6';
	str[4] = ':';
	str[5] = ACK+'0';
	str[6] = '\0';
	LCD1602show(0,1,str);
	while(1);               //程序执行完等待
}
/*I2C通信启始信号函数*/
void I2Cstart()
{
	SCL = 1;
	SDA = 1;
	delay();
	SDA = 0;
	delay();
	SCL = 0;
}
/*I2C通信数据传输函数*/
unsigned char I2Cwritedata(dat)
{
	unsigned char ACK;
	unsigned char mask;
	for(mask=0X80;mask!=0;mask=mask>>1)
	{
		if((mask & dat) == 0)
		{
			SDA = 0;
		}
		else 
		{
			SDA = 1;
		}
		delay();
		SCL = 1;
		delay();
		SCL = 0;
	}
	SDA = 1;
	delay();
	SCL = 1;
	ACK = SDA;
  delay();
	SCL = 0;
	return ACK;
}
/*I2C停止信号函数*/
void I2Cstop()
{
	SCL = 0;
	SDA = 0;
	delay();
	SCL = 1;
	delay();
	SDA = 1;
	delay();
}
/*I2C通信寻址函数*/
unsigned char I2Caddress(unsigned char address)
{
	unsigned char ACK;
	I2Cstart();
	ACK = I2Cwritedata(address<<1); //寻址命令最低位为读写位，用于表示之后的操作是读或写
	I2Cstop();
	return ACK;
}