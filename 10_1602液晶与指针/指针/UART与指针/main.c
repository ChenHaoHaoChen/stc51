#include <reg52.h>
unsigned char cmdarriver = 0;        //cmdarriver为命令到来变量，当进入接受中断时置1
unsigned char index = 0;             //index为接受SBUF变量
unsigned char cnt = 0;               //cnt为计数变量，存放数组元素个数
unsigned char *p;                    //指针变量p
unsigned char arrary1[]={"1-Hello!\r\n"};        //数组1-4
unsigned char arrary2[]={'2','-','H','e','l','l','o','!','\r','\n'};
unsigned char arrary3[]={51,45,72,101,108,108,111,33,13,10};
unsigned char arrary4[]={"4-Hello!\r\n"};
void configUART(unsigned int buad);  //函数声明
void main()
{
	EA = 1;                            //打开中断使能
	configUART(9600);                  //配置波特率为9600
	while(1)
	{
		if(cmdarriver)                  
	  {
		  cmdarriver = 0;
		  switch(index)
		  {
			  case 1:p = arrary1;          //p指针指向arrary1数组首地址，即第一个元素地址
			         cnt = sizeof(arrary1);//计算arrary1数组元素个数
			         TI = 1;               //由软件直接进入中断
			         break;
			  case 2:p = arrary2;
			         cnt = sizeof(arrary2);
			         TI = 1;
			         break;
			  case 3:p = arrary3;
			         cnt = sizeof(arrary3);
						   TI = 1;
			         break;
			  case 4:p = arrary4;
			         cnt = sizeof(arrary4);
			         TI = 1;			
			         break;
			  default:
				       break;
		  }
	  }
	}
}
void configUART(unsigned int buad)
{
	SCON = 0X50;                       //配置UART串口为模式1（最常用的模式）
	TMOD = TMOD & 0X0F;                //配置定时器1为模式2，即自动重装模式
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12/32)/buad; //固定公式计算波特率
	TL1 = TH1;
	TR1 = 1;                           //打开定时器1
	ES = 1;                            //打开UART串口中断  
}
/*UART串口中断，当RI=1（发送中断标志位，发送到停止位中间位置）进入中断
                当TI=1（接受中断标志位，接受到停止位中间位置）进入中断*/
void interruptUART() interrupt 4
{
	if(RI)
	{
		RI = 0;
		cmdarriver = 1;
		index = SBUF;
	}
	if(TI)
	{
		TI = 0;
		if(cnt > 0)
		{
			SBUF = *p;
			p++;
			cnt--;
		}
	}
}			