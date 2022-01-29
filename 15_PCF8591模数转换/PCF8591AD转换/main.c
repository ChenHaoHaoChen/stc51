/********************
A/D��ģ��ת����ͨ��channel0~3�ڶ�ȡ�ⲿģ���ѹ��
������ת��Ϊ���ֵ�ѹ�̶�
*********************/
#include <reg52.h>
bit flag300ms = 0;         //300msˢ�±�־λ
unsigned char TH0R;        //��ʱ������ֵ����λ
unsigned char TL0R;        //��ʱ������ֵ����λ

extern void LCD1602init(); //�ⲿ��������
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
extern void I2Cstart();
extern unsigned char I2Cwrite(unsigned char dat);
extern unsigned char I2CreadACK();
extern unsigned char I2CreadNACK();
extern void I2Cstop();

void configtime0(unsigned char ms);
unsigned char getadcval(unsigned char channel);
void valtostring(unsigned char val,unsigned char *str);

void main()
{
	unsigned char val;     
	unsigned char str[16];
	EA = 1;           //�����ж�
	configtime0(10);  //��ʱ��T0��ʱ10ms
	LCD1602init();    //LCD1602��ʼ��
	LCD1602show(0,0,"Val0 Val1 Val3");
	while(1)
	{
		if(flag300ms)
		{
			flag300ms = 0;
			val = getadcval(0);   //��ȡ��ѹֵ�̶ȣ�����val = 5����ѹ = ��val*��׼��ѹ2.5��/��8λ���̶�=2^8-1=255��
			valtostring(val,str); //����ȡ��ѹ�̶�ת��Ϊ��ѹֵ����λ�ַ���������
			LCD1602show(0,1,str);
			
			val = getadcval(1);
			valtostring(val,str);
			LCD1602show(5,1,str);
			
			val = getadcval(3);
			valtostring(val,str);
			LCD1602show(10,1,str);
		}
	}
}
/*��ȡ��ѹ�̶Ⱥ���*/
unsigned char getadcval(unsigned char channel)
{
	unsigned char val;
	I2Cstart();              //I2C��ʼ�ź�
	I2Cwrite(0X90);          //д��I2C������ַ����ѡ��дʹ��
	I2Cwrite(0X40 | channel);//д��D/A���ݼĴ�����ַ����ѡ��channel��0-3����
	I2Cstart();              //���·���I2C��ʼ�ź�
	I2Cwrite(0X91);          //д��I2C������ַ����ѡ���ʹ��
	I2CreadACK();            //�ȶ�һ�����ֽڣ��ṩ����ת��ʱ��
	val = I2CreadNACK();     
	I2Cstop();               //I2Cֹͣ�ź�
	return val;
}
/*��ѹת�ַ�������*/
void valtostring(unsigned char val,unsigned char *str)
{
	val = (val*25)/255;
	str[0] = (val/10) + '0';
	str[1] = '.';
	str[2] = (val%10) + '0';
	str[3] = 'V';
	str[4] = '\0';
}
/*���ö�ʱ��T0����*/
void configtime0(unsigned char ms)
{
	unsigned long tmp;
	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	TH0R = (unsigned char)(tmp >> 8);
	TL0R = (unsigned char)(tmp);
	TMOD = TMOD & 0XF0;
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;
	ET0 = 1;
}
/*��ʱ��T0�жϣ�ÿ10ms����һ��*/
void interrupttimme0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt >= 30)    //ÿ��ʱ��300ms��300ms��־λflag300msλ��1
	{
		cnt = 0;
		flag300ms = 1;
	}
}