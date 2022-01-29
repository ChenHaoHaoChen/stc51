#include <reg52.h>
unsigned char TH0R;
unsigned char TL0R;
unsigned char flag200ms = 0;

void configtime0(unsigned char ms);

extern void LCD1602init();//LCD1602外部声明
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);

extern void DS1302init(); //DS1302外部声明
extern void DS1302burstread(unsigned char *p);

void main()
{
	unsigned char sec;
	unsigned char time[8];
	unsigned char str1[12];
	unsigned char str2[12];
	EA = 1;
	configtime0(1);
	DS1302init();
	LCD1602init();
	while(1)
	{
		if(flag200ms == 1)
		{
			flag200ms = 0;
			DS1302burstread(time);
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
/*配置定时器T0定时1ms*/
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
/*定时器T0中断，每1ms进入一次*/
void interrupttime0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt >= 200)
	{
		cnt = 0;
		flag200ms = 1;
	}
}