#include <reg52.h>
unsigned long beats;
void cntbeats(unsigned long angle);
void main()
{
	TMOD = 0X01;        //设置定时器为1模式
	TH0 = 0XF8;         //定时2ms
	TL0 = 0XCD;
	TR0 = 1;            //开始定时
	EA = 1;             //打开总中断
	ET0 = 1;            //打开定时器T0中断
	cntbeats(360*2+180);
	while(1);
}
void cntbeats(unsigned long angle)//计算节拍函数
{
	EA = 0; //为了避免计算节拍时进入中断，计算前将中断关闭，计算后再打开
	beats = (angle*64*64)/360;
	EA = 1;
}
void interrupttime0() interrupt 1
{
	unsigned char code beat[8]={
	0x0e,0x0c,0x0d,0x09,0x0b,0x03,0x07,0x06
  };
	static unsigned char temp;
	static unsigned char i = 0;
	TH0 = 0XF8;//定时器重新定时2ms             
	TL0 = 0XCD;
	if(beats != 0)
	{
		temp = P1;            //temp为中间变量
	  temp = temp & 0XF0;   //将temp低四位清零，而不影响高四位
	  temp = temp | beat[i];//给P1低四位赋值，而不影响高四位
		P1 = temp;            
		i++;
		i = 0x07 & i;         //当i=8时，i=0
		beats--;
	}
	else
	{
		P1 = 0X0F | P1;       //P1低四位清零，高四位不变
	}
}