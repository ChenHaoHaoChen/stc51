#include <reg52.h>

extern void LCD1602init();  //�ⲿ��������
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern void I2Cstop();
extern unsigned char I2CreadNACK();

unsigned char I2Creadbyte(unsigned char address);//��������
void I2Cwritebyte(unsigned char address,unsigned char dat);

void main()
{
	unsigned char dat;
	unsigned char str[10];
	LCD1602init();              //LCD1602��ʼ��
	dat = I2Creadbyte(0x00);    //��ȡEEPROM0X00��ַΪ�ϵ�����
	str[0] = dat % 10 + '0';    //����������ȡ���λ��
	str[1] = '\0';              //�ĳ��ַ�����ʽ
	LCD1602show(0,0,str);       //����ȡ����������ʾ��LCD1602��
	I2Cwritebyte(0X00,dat+1);   //����ȡ�������ݼ�1����д��0X00��ַ��
	while(1);                   //����ȴ�
}
/*��ȡEEPROM��һ���ֽڣ�addressΪ��ַ*/
unsigned char I2Creadbyte(unsigned char address)
{
	unsigned char dat;
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cstart();
	I2Cwrite((0X50<<1) | 0X01);
	dat = I2CreadNACK();
	I2Cstop();
	return dat;
}
/*д��EEPROM��һ���ֽڣ�addressΪ��ַ��datΪһ�ֽ�����*/
void I2Cwritebyte(unsigned char address,unsigned char dat)
{
	I2Cstart();
	I2Cwrite(0X50<<1);
	I2Cwrite(address);
	I2Cwrite(dat);
	I2Cstop();
}