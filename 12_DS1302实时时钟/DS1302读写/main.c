#include <reg52.h>
sbit DS1302_CE = P1^7;
sbit DS1302_SCK = P3^5;
sbit DS1302_SIO = P3^4;
unsigned char TH0R;          //定时器T0重载值
unsigned char TL0R;
unsigned char flag200ms = 0; //200ms定时标志

extern void LCD1602init();   //外部函数声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
void DS1302init();           //函数声明
void configtime0(unsigned char ms);
unsigned char DS1302singleread(unsigned char addsn);

void main()
{
	unsigned char i; 
	unsigned char sec = 0XAA;  //秒位刷新比较
	unsigned char time[7];     //存放DS1302内部时间
	unsigned char str1[12];    //显示字符串1
	unsigned char str2[12];    //显示字符串2
	EA = 1;                    //打开总中断
	configtime0(1);            //定时1ms
	DS1302init();              //DS1302初始化
	LCD1602init();             //LCD1602初始化
	while(1)
	{
		if(flag200ms == 1)
		{
			flag200ms = 0;
			for(i=0;i<7;i++)
			{
				time[i] = DS1302singleread(i);
			}
			if(sec != time[0])
			{
				str1[0] = '2';
				str1[1] = '0';
				str1[2] = (time[6] >> 4) + '0';  //年十位转化为字符
				str1[3] = (time[6] & 0X0F) + '0';//年个位转化为字符
				str1[4] = '-';
				str1[5] = (time[4] >> 4) + '0';  //月十位转化为字符
				str1[6] = (time[4] & 0X0F) + '0';//月个位转化为字符
				str1[7] = '-';
				str1[8] = (time[3] >> 4) + '0';  //日十位转化为字符
				str1[9] = (time[3] & 0X0F) + '0';//日个位转化为字符
				str1[10] = '\0';
				LCD1602show(3,0,str1);
				
				str2[0] = (time[2] >> 4) + '0';  //时十位转化为字符
				str2[1] = (time[2] & 0X0F) + '0';//时个位转化为字符
				str2[2] = ':';
				str2[3] = (time[1] >> 4) + '0';  //分十位转化为字符
				str2[4] = (time[1] & 0X0F) + '0';//分个位转化为字符
				str2[5] = ':';
				str2[6] = (time[0] >> 4) + '0';  //秒十位转化为字符
				str2[7] = (time[0] & 0X0F) + '0';//秒个位转化为字符
				str2[8] = '\0';
				LCD1602show(4,1,str2);
				
				sec = time[0];                   //将当前秒值赋值给sec以便下次比较秒值
			}
		}
	}
}
/*DS1302单字节读函数*/
unsigned char DS1302readbyte()
{
	unsigned char dat = 0;
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if(DS1302_SIO != 0)       //先读取SIO值，再拉高拉低时钟
		{
			dat = dat | mask;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
	return dat;
}
/*DS1302单字节写函数*/
void DS1302writebyte(unsigned char dat)
{
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if((mask & dat) != 0)     
		{
			DS1302_SIO = 1;         //先准备好SIO数据，再拉高拉低时钟
		}
		else
		{
			DS1302_SIO = 0;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
}
/*DS1302单独读函数*/
unsigned char DS1302singleread(unsigned char addsn)
{
	unsigned char dat;
	DS1302_CE = 1;
	DS1302writebyte((addsn<<1) | 0X81);
	dat = DS1302readbyte();
	DS1302_CE = 0;
	return dat;
}
/*DS1302单独写函数*/
void DS1302singlewrite(unsigned char addsn,unsigned char dat)
{
	DS1302_CE = 1;
	DS1302writebyte((addsn<<1) | 0X80);
	DS1302writebyte(dat);
	DS1302_CE = 0;
}
/*DS1302初始化函数*/
void DS1302init()
{
	unsigned char i; 
	unsigned char code date[7]={
		0X00,0X17,0X21,0X27,0X08,0X05,0X21
	};
	DS1302_CE = 0;            
	DS1302_SCK = 0;
	DS1302singlewrite(7,0X00);
	for(i=0;i<7;i++)
	{
		DS1302singlewrite(i,date[i]);
	}
}
/*T0定时器定时1ms配置函数*/
void configtime0(unsigned char ms)
{
	unsigned long temp;
	temp = 11059200/12;
	temp = (temp*ms)/1000;
	temp = 65536 - temp;
	TH0R = (unsigned char)(temp >> 8);
	TL0R = (unsigned char)(temp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*中断函数，每1ms进入一次*/
void interrupttime0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt == 200)
	{
		cnt = 0;
		flag200ms = 1;
	}
}