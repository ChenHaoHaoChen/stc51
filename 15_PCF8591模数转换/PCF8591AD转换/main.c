/********************
A/D是模数转换，通过channel0~3口读取外部模拟电压，
并将其转换为数字电压刻度
*********************/
#include <reg52.h>
bit flag300ms = 0;         //300ms刷新标志位
unsigned char TH0R;        //定时器重载值高四位
unsigned char TL0R;        //定时器重载值低四位

extern void LCD1602init(); //外部函数声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern unsigned char I2CreadACK();
extern unsigned char I2CreadNACK();
extern void I2Cstop();

void configtime0(unsigned char ms);
unsigned char getadcval(unsigned char channel);
void valtostring(unsigned char val,unsigned char *str);

void main()
{
	unsigned char val;     
	unsigned char str[16];
	EA = 1;           //打开总中断
	configtime0(10);  //定时器T0定时10ms
	LCD1602init();    //LCD1602初始化
	LCD1602show(0,0,"Val0 Val1 Val3");
	while(1)
	{
		if(flag300ms)
		{
			flag300ms = 0;
			val = getadcval(0);   //获取电压值刻度，例：val = 5，电压 = （val*基准电压2.5）/（8位，刻度=2^8-1=255）
			valtostring(val,str); //将获取电压刻度转换为电压值保存位字符串数组中
			LCD1602show(0,1,str);
			
			val = getadcval(1);
			valtostring(val,str);
			LCD1602show(5,1,str);
			
			val = getadcval(3);
			valtostring(val,str);
			LCD1602show(10,1,str);
		}
	}
}
/*获取电压刻度函数*/
unsigned char getadcval(unsigned char channel)
{
	unsigned char val;
	I2Cstart();              //I2C起始信号
	I2Cwrite(0X90);          //写入I2C器件地址，并选择写使能
	I2Cwrite(0X40 | channel);//写入D/A数据寄存器地址，并选择channel（0-3）端
	I2Cstart();              //重新发送I2C起始信号
	I2Cwrite(0X91);          //写入I2C器件地址，并选择读使能
	I2CreadACK();            //先读一个空字节，提供采样转换时间
	val = I2CreadNACK();     
	I2Cstop();               //I2C停止信号
	return val;
}
/*电压转字符串函数*/
void valtostring(unsigned char val,unsigned char *str)
{
	val = (val*25)/255;
	str[0] = (val/10) + '0';
	str[1] = '.';
	str[2] = (val%10) + '0';
	str[3] = 'V';
	str[4] = '\0';
}
/*配置定时器T0函数*/
void configtime0(unsigned char ms)
{
	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	TH0R = (unsigned char)(tmp >> 8);
	TL0R = (unsigned char)(tmp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*定时器T0中断，每10ms进入一次*/
void interrupttimme0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt >= 30)    //每定时到300ms，300ms标志位flag300ms位置1
	{
		cnt = 0;
		flag300ms = 1;
	}
}