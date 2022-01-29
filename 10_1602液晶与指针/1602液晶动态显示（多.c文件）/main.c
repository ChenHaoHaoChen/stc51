#include <reg52.h>
bit flag500ms = 0;               //500ms定时标志位
unsigned char TH0R;              //定时器T0高位重载值
unsigned char TL0R;              //定时器T0低位重载值
unsigned char code str1[]={    //需要显示的字符串1
		"                Ben Gui Xin                    "
	};
	unsigned char code str2[]={    //需要显示的字符串2
		"                Shi Wo Er                      "
	};

void configtime(unsigned int ms);//函数声明  
extern void LCD1602init();       //外部函数声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);//外部函数声明

void main()
{
	unsigned char index = 0;       //指针索引
	EA = 1;                        //打开总中断
	LCD1602init();                 //LCD1602初始化函数            
	configtime(10);                //配置定时时间10ms
	while(1)
	{
		if(flag500ms)   
		{
			flag500ms = 0;
			LCD1602show(0,0,str1+index);//LCD1602显示函数 
	    LCD1602show(0,1,str2+index);
			index++;
			if(index == (sizeof(str1)-1))
			{
				index = 0;
			}
		}
	}
}
/*配置定时时间函数*/
void configtime(unsigned int ms)
{
	unsigned long temp;
	temp = 65536 - (11059200/12*ms/1000);//计算定时时间
	TH0R = (unsigned char)(temp >> 8); 
	TL0R = (unsigned char) temp;
	TMOD = TMOD & 0XF0; //设置定时器为模式1
	TMOD = TMOD | 0X01;
	TH0 = TH0R;         
	TL0 = TL0R;
	TR0 = 1;            //开始定时
	ET0 = 1;            //打开定时器T0中断
}
/*定时器T0中断，每10ms进入一次*/
void interrupttime0() interrupt 1
{
	static unsigned char i = 0;
	TH0 = TH0R;         //重新定时
	TL0 = TL0R;
	i++;
	if(i == 50)
	{
		i = 0;
		flag500ms = 1;    //定时到500ms时，标志位置1
	}
}