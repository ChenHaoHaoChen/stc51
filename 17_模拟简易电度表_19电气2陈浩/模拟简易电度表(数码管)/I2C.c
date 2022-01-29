/**************************
     I2Cͨ�����߳���
**************************/
#include <reg52.h>
#include <intrins.h>
#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}
sbit SCL = P2^6;
sbit SDA = P2^7;
/**********����������ʼ�ź�**********/
void I2Cstart()
{
 SDA = 1;    //����ȷ�� SDA��SCL ���Ǹߵ�ƽ
 SCL = 1;
 I2CDelay();
 SDA = 0;    
 I2CDelay();
 SCL = 0;    
}
/**********��������ֹͣ�ź�**********/
void I2Cstop()
{
 SCL = 0;    //����ȷ�� SDA��SCL ���ǵ͵�ƽ
 SDA = 0;
 I2CDelay();
 SCL = 1;    
 I2CDelay();
 SDA = 1;    
 I2CDelay();
}
/**********I2C����д����,dat-��д���ֽ�,����ֵ-�ӻ�Ӧ��λ��ֵ**********/
bit I2Cwrite(unsigned char dat)
{
 bit ack;                           //�����ݴ�Ӧ��λ��ֵ
 unsigned char i;                   
 for(i=0x80; i!=0; i>>=1)           //�Ӹ�λ����λ���ν���
 {
		if ((i&dat) == 0)               //��λ��ֵ����� SDA ��
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
 SDA = 1;                           //8 λ���ݷ�����������ͷ� SDA���Լ��ӻ�Ӧ��
 I2CDelay();
 SCL = 1;                           //���� SCL
 ack = SDA;                         //��ȡ��ʱ�� SDA ֵ����Ϊ�ӻ���Ӧ��ֵ
 I2CDelay();
 SCL = 0;                           //������ SCL ���Ӧ��λ��������ס����
 return (~ack);                     //0=�����ڻ�æ��д��ʧ�ܣ�1=�����ҿ��л�д��ɹ�
                                   
}
/**********I2C���߶�����,�����ͷ�Ӧ���ź�,����ֵ-�������ֽ�**********/
unsigned char I2CreadNACK()
{
 unsigned char i;
 unsigned char dat;
 SDA = 1;                       //����ȷ�������ͷ� SDA
 for (i=0x80; i!=0; i>>=1)          //�Ӹ�λ����λ���ν���
 {
 I2CDelay();
 SCL = 1;                           
		if(SDA == 0)                    //��ȡ SDA ��ֵ
		{
			dat &= ~i;                    //Ϊ 0 ʱ��dat �ж�Ӧλ����
		}
		else
		{
			dat |= i;                     //Ϊ 1 ʱ��dat �ж�Ӧλ�� 1
		}
 I2CDelay();
 SCL = 0;                      		  //������ SCL����ʹ�ӻ����ͳ���һλ
 }
 SDA = 1;                       		//8 λ���ݷ���������� SDA�����ͷ�Ӧ���ź�
 I2CDelay();
 SCL = 1;                       		
 I2CDelay();
 SCL = 0;                      		  //������ SCL ��ɷ�Ӧ��λ��������ס����
 return dat;
}
/**********I2C ���߶�������������Ӧ���źţ�����ֵ-�������ֽ�**********/
//unsigned char I2CreadACK()
//{
// unsigned char i;
// unsigned char dat;
// SDA = 1;                       //����ȷ�������ͷ� SDA
// for (i=0x80; i!=0; i>>=1) 			//�Ӹ�λ����λ���ν���
// {
// I2CDelay();
// SCL = 1;                       
// if(SDA == 0)                   //��ȡ SDA ��ֵ
// dat &= ~i;                     //Ϊ 0 ʱ��dat �ж�Ӧλ����
// else
// dat |= i;                      //Ϊ 1 ʱ��dat �ж�Ӧλ�� 1
// I2CDelay();
// SCL = 0;                       //������ SCL����ʹ�ӻ����ͳ���һλ
// }
// SDA = 0;                       //8λ���ݷ����������SDA������Ӧ���ź�
// I2CDelay();
// SCL = 1;                       
// I2CDelay();
// SCL = 0;                       //������ SCL ���Ӧ��λ��������ס����
// return dat;
//}