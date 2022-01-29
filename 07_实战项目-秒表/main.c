#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit ESC = P2^5;
sbit ENTER = P2^6;
unsigned char code number[10]={   //数码管显示驱动数组     
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char keyfinally[2]={     //按键终值
	1,1
 };
unsigned char ledbuffer[6]={      //六位数码管显示缓冲数组
	0XFF,0XFF,0XFF,0XFF,0XFF,0XFF
 };
unsigned char decimal = 0;        //计时小数部分
unsigned int integer = 0;         //计时整数部分
unsigned char TH01;               //定时补偿之后高位
unsigned char TL01;               //定时补偿之后地位
bit symbol = 0;                   //开始，暂定，结束定时标志位
void configtime(unsigned char ms);//函数声明部分
void keydriver();           
void stop();
void run();
void count();
void display();
void ledscan();
void keyscan();

void main()
{
	ENLED = 0;                      //打开74138译码器
	ADDR3 = 1;
	TMOD = 0X01;                    //设置定时器为1模式
	EA = 1;                         //中断使能打开
	P2 = 0XFE;                      // 1111 1110 打开第四排按键
	configtime(2);                  //定时函数（加补偿，一般补偿40）
	while(1)
	{
		keydriver();
	}
}

void configtime(unsigned char ms)
{
	unsigned long temp = 0;
	temp = 65536-(ms*11059200)/(12*1000);//定时2ms
	temp = temp + 40;                    //补偿40
	TH01 = (unsigned char )(temp >> 8);  //高位赋值给TH01
	TL01 = (unsigned char )temp;         //低位赋值给TL01
	TH0 = TH01;
	TL0 = TL01;
	TR0 = 1;                             //打开T0定时
	ET0 = 1;                             //打开T0中断
}

void keydriver()                       //按键检测函数 
{
	static unsigned char backup[2]={     //按键上一个状态
		1,1
	};
	static unsigned char i;
	for(i=0;i<2;i++)
	{
		if(keyfinally[i] !=backup[i])
		{
			if(backup[i] == 0)
			{
				if(i == 0)                     //i == 0时，认定ESC按键按下
				{
					stop();                      //ESC按键按下，清零并停止计数            
				}
				else if(i == 1)                //i == 1时，认定ENTER按键按下
				{
					run();                       //ENTER按键按下，开始计时或者暂停计时
				}
			}
			backup[i] = keyfinally[i];       //当前状态赋值给backup
		}
	}
}

void stop()                            //停止计数函数
{
	decimal = 0;                         //小数和整数部分清零
	integer = 0;
	symbol = 0;                          //标志位清零，即数码管停止跳动，一直显示0.00
}

void run()                             //开始计时或者暂停函数
{
	if(symbol)                           //用于开始和暂停的转换
	{
		symbol = 0;
	}
	else
	{
		symbol = 1;
	}
}

void interrupttime0() interrupt 1      //每2ms进入一次中断
{
	static unsigned char ms10 = 0;       //10ms定时
	TH0 = TH01;                          //重新定时
	TL0 = TL01;
	ms10++;
	if(ms10 == 5)                        //当定时到10ms时
	{
		ms10 = 0;                          //10ms定时清零
		count();                           //计数函数，每10ms加一
	}
	display();                           //数码管显示函数
	ledscan();                           //数码管扫描函数
	keyscan();                           //按键扫描函数
}

void count()
{
	if(symbol)                            //仅当标志位symbol=1时，开始计数
	{
	  decimal++;
	  if(decimal == 100)
	  {
		  decimal = 0;
		  integer++;
		  if(integer == 10000)              //最大定时到9999.99s，约等于两个半小时
		  {
			  integer = 0;
		  }
	  }
	}
}

void display()                          //数码管显示函数
{
	static unsigned char i;
	ledbuffer[0] = number[decimal%10];
	ledbuffer[1] = number[decimal/10%10];
	ledbuffer[2] = number[integer%10] & 0X7F;
	ledbuffer[3] = number[integer/10%10];
	ledbuffer[4] = number[integer/100%10];
	ledbuffer[5] = number[integer/1000%10];
	for(i=5;i>2;i--)                     //高位灭零
	{
		if(ledbuffer[i] == 0XC0)
		{
			ledbuffer[i] = 0XFF;
		}
		else
		{
			break;
		}
	}
}

void ledscan()                         //数码管扫描函数，每2ms切换数码管一次
{
	static unsigned char i = 0;
	P0 = 0XFF;                           //数码管消隐
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i=0;break;
		default:break;
	}
}

void keyscan()                              //按键扫描函数
{
	static unsigned char keybuffer[2]={       //按键扫描缓冲数组
		0XFF,0XFF 
	};
	static unsigned char i;
	keybuffer[0] = (keybuffer[0] <<1) | ESC;  //每2ms移位一次
	keybuffer[1] = (keybuffer[1] <<1) | ENTER;
	for(i=0;i<2;i++)
	{
		if( (keybuffer[i] ) == 0X00 )           //连续扫描8次都是0时，即16ms内为按下状态时，认定按键按下
		{
			keyfinally[i] = 0;
		}
		else if( (keybuffer[i] ) == 0XFF )      //连续扫描8此都是1时，即16ms内为弹起状态时，认定按键弹起
		{
			keyfinally[i] = 1;
		}
	}
}