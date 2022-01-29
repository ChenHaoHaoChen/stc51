/*****************
NEC红外遥控配置程序
*****************/
#include <reg52.h>
sbit IRD = P3^3;         //NEC红外通信引脚   
unsigned char irflag = 0;//NEC红外通信接受完成标志
unsigned char ircode[4]; //NEC红外通信接受字符，0，1为用户码和用户反码
/*NEC红外通信初始化*/                         //2，3为按键码和按键反码
void Infraredinit()                           
{               
	TMOD = TMOD & 0X0F;   //配置定时器1为1模式
	TMOD = TMOD | 0X10;
	TR1 = 0;              //只是强调这里采用外部中断，不加也行
	ET1 = 0;
	IT1 = 1;              //外部中断1触发标志位值1，即下降沿触发
	EX1 = 1;              //打开外部中断使能
}
/*获取低电平时间*/
unsigned int getlowtime()
{
	TH1 = 0;           
	TL1 = 0;
	TR1 = 1;               //开始定时
	while(!IRD)
	{
		if(TH1 >= 0XDC)      //等待10ms仍无响应则推出
		{
			break;
		}
	}
	TR1 = 0;               //关闭定时
	return (TH1*256 + TL1);//返回定时时间
}
/*获取高电平时间*/
unsigned int gethightime()
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;               //开始定时
	while(IRD)
	{
		if(TH1 >= 0XDC)      //等待10ms仍无响应则推出
		{
			break;
		}
	}
	TR1 = 0;               //关闭定时
	return (TH1*256 + TL1);//返回定时时间
}
/*外部中断1*/
void Infraedinterrupt() interrupt 2
{
	unsigned int time;     //time为获取高低电平时间
	unsigned char i,j;
	unsigned char byte;    //红外通信字节
	/*检测引导码中的9ms载波，范围为：8.5ms~9.5ms*/
	time = getlowtime();   //获取载波时间（载波为低电平）
	if( (time < 7833) || (time > 8755) )
	{
		IE1 = 0;
		return;              //若不满足则退出中断
	}
	/*检测引导码中的4.5ms空闲，范围为：4ms~5ms*/
	time = gethightime();  //获取空闲时间（空闲为高电平）
	if( (time <3686 ) || (time > 4608) )
	{
		IE1 = 0;
		return;              //若不满足则退出中断
	}
	/*循环四次分别读取4个字节的用户码，用户反码，按键码，按键反码*/
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			/*检测560us载波，范围为：280us~840us*/
			time = getlowtime();//获取载波时间
			if( (time <258) || (time >774) )
			{
				IE1 = 0;
				return;
			}
			time = gethightime();//获取空闲时间
			if( (time > 258) && (time < 774) )
			{
				byte = byte >> 1;  //若空闲时间为560us，范围：280us~840us，则比特置为：0
			}
			else if( (time > 922) && (time < 2175) )
			{
				byte = byte >> 1;  //若空闲时间为1.68ms，范围：1ms~2.36ms，则比特置为：1
				byte = byte | 0X80;
			}
			else 
			{
				IE1 = 0;           
				return;             //若都不是，则退出中断
			}
		}
		ircode[i] = byte;       //循环将读取到的字节存放在数组中
	}
	irflag = 1;               //红外通信完成，红外通信标志位置1
	IE1 = 0;                  //外部中断1中断请求标志位清零
}