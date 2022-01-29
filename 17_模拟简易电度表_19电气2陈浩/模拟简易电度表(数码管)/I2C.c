/**************************
     I2C通信总线程序
**************************/
#include <reg52.h>
#include <intrins.h>
#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit SCL = P2^6;
sbit SDA = P2^7;
/**********产生总线起始信号**********/
void I2Cstart()
{
 SDA = 1;    //首先确保 SDA、SCL 都是高电平
 SCL = 1;
 I2CDelay();
 SDA = 0;    
 I2CDelay();
 SCL = 0;    
}
/**********产生总线停止信号**********/
void I2Cstop()
{
 SCL = 0;    //首先确保 SDA、SCL 都是低电平
 SDA = 0;
 I2CDelay();
 SCL = 1;    
 I2CDelay();
 SDA = 1;    
 I2CDelay();
}
/**********I2C总线写操作,dat-待写入字节,返回值-从机应答位的值**********/
bit I2Cwrite(unsigned char dat)
{
 bit ack;                           //用于暂存应答位的值
 unsigned char i;                   
 for(i=0x80; i!=0; i>>=1)           //从高位到低位依次进行
 {
		if ((i&dat) == 0)               //该位的值输出到 SDA 上
		{
		SDA = 0;
		}
		else
		{
		SDA = 1;
		}
 I2CDelay();
 SCL = 1;                           
 I2CDelay();
 SCL = 0;                           
 }
 SDA = 1;                           //8 位数据发送完后，主机释放 SDA，以检测从机应答
 I2CDelay();
 SCL = 1;                           //拉高 SCL
 ack = SDA;                         //读取此时的 SDA 值，即为从机的应答值
 I2CDelay();
 SCL = 0;                           //再拉低 SCL 完成应答位，并保持住总线
 return (~ack);                     //0=不存在或忙或写入失败，1=存在且空闲或写入成功
                                   
}
/**********I2C总线读操作,并发送非应答信号,返回值-读到的字节**********/
unsigned char I2CreadNACK()
{
 unsigned char i;
 unsigned char dat;
 SDA = 1;                       //首先确保主机释放 SDA
 for (i=0x80; i!=0; i>>=1)          //从高位到低位依次进行
 {
 I2CDelay();
 SCL = 1;                           
		if(SDA == 0)                    //读取 SDA 的值
		{
			dat &= ~i;                    //为 0 时，dat 中对应位清零
		}
		else
		{
			dat |= i;                     //为 1 时，dat 中对应位置 1
		}
 I2CDelay();
 SCL = 0;                      		  //再拉低 SCL，以使从机发送出下一位
 }
 SDA = 1;                       		//8 位数据发送完后，拉高 SDA，发送非应答信号
 I2CDelay();
 SCL = 1;                       		
 I2CDelay();
 SCL = 0;                      		  //再拉低 SCL 完成非应答位，并保持住总线
 return dat;
}
/**********I2C 总线读操作，并发送应答信号，返回值-读到的字节**********/
//unsigned char I2CreadACK()
//{
// unsigned char i;
// unsigned char dat;
// SDA = 1;                       //首先确保主机释放 SDA
// for (i=0x80; i!=0; i>>=1) 			//从高位到低位依次进行
// {
// I2CDelay();
// SCL = 1;                       
// if(SDA == 0)                   //读取 SDA 的值
// dat &= ~i;                     //为 0 时，dat 中对应位清零
// else
// dat |= i;                      //为 1 时，dat 中对应位置 1
// I2CDelay();
// SCL = 0;                       //再拉低 SCL，以使从机发送出下一位
// }
// SDA = 0;                       //8位数据发送完后，拉低SDA，发送应答信号
// I2CDelay();
// SCL = 1;                       
// I2CDelay();
// SCL = 0;                       //再拉低 SCL 完成应答位，并保持住总线
// return dat;
//}