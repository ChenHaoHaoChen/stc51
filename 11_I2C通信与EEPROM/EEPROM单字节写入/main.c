#include <reg52.h>

extern void LCD1602init();  //外部函数声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();
extern unsigned char I2CreadNACK();

unsigned char I2Creadbyte(unsigned char address);//函数声明
void I2Cwritebyte(unsigned char address,unsigned char dat);

void main()
{
	unsigned char dat;
	unsigned char str[10];
	LCD1602init();              //LCD1602初始化
	dat = I2Creadbyte(0x00);    //读取EEPROM0X00地址为上的数据
	str[0] = dat % 10 + '0';    //读出的数据取其个位数
	str[1] = '\0';              //改成字符串形式
	LCD1602show(0,0,str);       //将读取到的数据显示在LCD1602上
	I2Cwritebyte(0X00,dat+1);   //将读取到的数据加1，并写入0X00地址中
	while(1);                   //程序等待
}
/*读取EEPROM中一个字节，address为地址*/
unsigned char I2Creadbyte(unsigned char address)
{
	unsigned char dat;
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cstart();
	I2Cwrite((0X50<<1) | 0X01);
	dat = I2CreadNACK();
	I2Cstop();
	return dat;
}
/*写入EEPROM中一个字节，address为地址，dat为一字节数据*/
void I2Cwritebyte(unsigned char address,unsigned char dat)
{
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cwrite(dat);
	I2Cstop();
}