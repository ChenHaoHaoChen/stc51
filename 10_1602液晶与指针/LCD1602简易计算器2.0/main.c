#include <reg52.h>
unsigned char TH0R;                //��ʱ����λT0����ֵ     
unsigned char TL0R;                //��ʱ����λT0����ֵ   
unsigned char step = 1;            //���㲽�����
unsigned char len1;     
unsigned char len2;
unsigned char type;
signed long num1 = 0;              //����
signed long num2 = 0;              //������
signed long result = 0;            //���

extern void LCD1602init();         //�ⲿLCD1602��������
extern void LCD1602clearscreen();
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);

extern void keydriver();           //�ⲿ������������
extern void keyscan();

void configtime0(unsigned char ms);//��������
void esc();

void main()
{
	EA = 1;                          //�����ж�
	configtime0(1);                  //���ö�ʱʱ��1ms
	LCD1602init();                   //LCD1602��ʼ��
	LCD1602show(3,0,"JiSuanQi");     //Ĭ����ʾ�������������������������
	LCD1602show(0,1,"AnRenYiJianJiXu");
	while(1)
	{
		keydriver();                   //������������
	}
}
/*����ת�ַ�������*/
unsigned char numtostring(signed long num,unsigned char *str)
{
	unsigned char buffer[10];
	signed char i = 0;
	unsigned char len = 0;
	if(num < 0)       //������Ϊ����ʱ��תΪ�ַ�ʱ����ǰ���һ����-��
	{
		num = - num;
		*str++ = '-';
		len++;
	}
	while(num > 0)
	{
		buffer[i] = num % 10;
		num = num / 10;
		i++;
	}
	len = len + i;
	i = i - 1;
	while(i>=0)
	{
		*str++ = buffer[i] + '0';
		i--;
	}
	*str = '\0';      //�ַ�����������\0
	return len;       //�����ַ������ȣ�����\0
}
/*���ְ���ִ�к���*/
void numkeyaction(unsigned char num)
{
	unsigned char str[7];
	if(step == 1)     //������1���и�ֵ����ʾ
	{
		num1 = num1*10 + num;
		len1 = numtostring(num1,str);
		LCD1602show(0,0,str);
	}
	else if(step == 2)//������2���и�ֵ����ʾ
	{
		num2 = num2*10 + num;
		len2 = numtostring(num2,str);
		LCD1602show(len1+1,0,str);
	}
	else if(step == 3)
	{
		esc();
	}
}
/*�������������*/
void operkeyaction(unsigned char oper)
{
	if(oper == 10)     //������
	{
		type = '+';
		LCD1602show(len1,0,"+");
		step = 2;
	}
	else if(oper == 11)//������
	{
		type = '-';
		LCD1602show(len1,0,"-");
		step = 2;
	}
	else if(oper == 12)//������
	{
		type = '*';
		LCD1602show(len1,0,"*");
		step = 2;
	}
	else if(oper == 13)//������
	{
		type = '/';
		LCD1602show(len1,0,"/");
		step = 2;
	}
}
/*����������*/
void getresult()
{
	unsigned char len;
	unsigned char str[8];
	if(type == '+')
	{
		result = num1 + num2;
	}
	else if(type == '-')
	{
		result = num1 - num2;
	}
	else if(type == '*')
	{
		result = num1 * num2;
	}
	else if(type == '/')
	{
		result = num1 / num2;
	}
	step = 3;
	len = numtostring(result,str);
	LCD1602show(len1+len2+1,0,"=");
	LCD1602show(16-len,1,str);
}
/*���㺯��*/
void esc()
{
	num1 = 0;
	num2 = 0;
	result = 0;
	step = 1;
	LCD1602clearscreen();
	LCD1602show(0,0,"0");
}
/*��������*/
void keyaction(unsigned char key)
{
	static unsigned char i = 0;
	if(i == 0)
	{
		LCD1602clearscreen();
		i = 1;
	}
  if( (key >= 0) && (key <= 9) )//key=0~9ִ�����ְ�������
	{
		numkeyaction(key);
	}
	else if( (key >= 10) && (key <= 13) )//key=10~13ִ�����ְ�������
	{
	 operkeyaction(key);
	}
	else if(key == 14)
	{
		esc();
	}
	else if(key == 15)
	{
		getresult();
	}
}
/*���ö�ʱʱ�亯��*/
void configtime0(unsigned char ms)
{
	unsigned long temp;
	temp = 11059200/12;
	temp = (temp * ms)/1000;//ע�����ﲻ��Ϊ��temp*(ms/1000),����temp������Ϊ0
	temp = 65536 - temp;
  TH0R = (unsigned char)(temp >> 8);
	TL0R = (unsigned char)(temp);
	TMOD = TMOD & 0XF0;     //���ö�ʱ��Ϊ1ģʽ
	TMOD = TMOD | 0X01;
	TH0 = TH0R;
	TL0 = TL0R;
	TR0 = 1;                //��ʼ��ʱ
	ET0 = 1;                //�򿪶�ʱ��0�ж�
}
/*ÿ1ms����һ�ζ�ʱ��T0�ж�*/
void interrupttime0() interrupt 1
{
	TH0 = TH0R;             //�������ö�ʱʱ��
	TL0 = TL0R;
	keyscan();
}