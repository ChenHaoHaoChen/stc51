#include <reg52.h>
#include <intrins.h> 
#define delay() {_nop_();_nop_();_nop_();_nop_();}//delay()����Ϊ��ʱ4����е����
sbit SDA = P3^6;
sbit SCL = P3^7;

extern void LCD1602init();//�ⲿ��������
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
unsigned char I2Caddress(unsigned char address);//��������

void main()
{
	unsigned char ACK;      //ACK����Ϊ���ܷ����͵����ݣ�ACK = 0����ʾ�˵�ַ���ڣ���֮��������
	unsigned char str[10];  //LCD1602��ʾ����
	LCD1602init();          //LCD1602��ʼ��
	ACK = I2Caddress(0X50); //��I2Cͨ�ŷ��͵�ַ
	str[0] = '0';
	str[1] = 'X';
	str[2] = '5';
	str[3] = '0';
	str[4] = ':';
	str[5] = ACK+'0';       //��ACKת��Ϊ�ַ���ʽ
	str[6] = '\0';
	LCD1602show(0,0,str);   //LCD1602��ʾ ��ַ��0/1��0����ַ���ڣ�1����ַ�����ڣ�
	
  ACK = I2Caddress(0X66);
	str[0] = '0';
	str[1] = 'X';
	str[2] = '6';
	str[3] = '6';
	str[4] = ':';
	str[5] = ACK+'0';
	str[6] = '\0';
	LCD1602show(0,1,str);
	while(1);               //����ִ����ȴ�
}
/*I2Cͨ����ʼ�źź���*/
void I2Cstart()
{
	SCL = 1;
	SDA = 1;
	delay();
	SDA = 0;
	delay();
	SCL = 0;
}
/*I2Cͨ�����ݴ��亯��*/
unsigned char I2Cwritedata(dat)
{
	unsigned char ACK;
	unsigned char mask;
	for(mask=0X80;mask!=0;mask=mask>>1)
	{
		if((mask & dat) == 0)
		{
			SDA = 0;
		}
		else 
		{
			SDA = 1;
		}
		delay();
		SCL = 1;
		delay();
		SCL = 0;
	}
	SDA = 1;
	delay();
	SCL = 1;
	ACK = SDA;
  delay();
	SCL = 0;
	return ACK;
}
/*I2Cֹͣ�źź���*/
void I2Cstop()
{
	SCL = 0;
	SDA = 0;
	delay();
	SCL = 1;
	delay();
	SDA = 1;
	delay();
}
/*I2Cͨ��Ѱַ����*/
unsigned char I2Caddress(unsigned char address)
{
	unsigned char ACK;
	I2Cstart();
	ACK = I2Cwritedata(address<<1); //Ѱַ�������λΪ��дλ�����ڱ�ʾ֮��Ĳ����Ƕ���д
	I2Cstop();
	return ACK;
}