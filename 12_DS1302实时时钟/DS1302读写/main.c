#include <reg52.h>
sbit DS1302_CE = P1^7;
sbit DS1302_SCK = P3^5;
sbit DS1302_SIO = P3^4;
unsigned char TH0R;          //��ʱ��T0����ֵ
unsigned char TL0R;
unsigned char flag200ms = 0; //200ms��ʱ��־

extern void LCD1602init();   //�ⲿ��������
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *str);
void DS1302init();           //��������
void configtime0(unsigned char ms);
unsigned char DS1302singleread(unsigned char addsn);

void main()
{
	unsigned char i; 
	unsigned char sec = 0XAA;  //��λˢ�±Ƚ�
	unsigned char time[7];     //���DS1302�ڲ�ʱ��
	unsigned char str1[12];    //��ʾ�ַ���1
	unsigned char str2[12];    //��ʾ�ַ���2
	EA = 1;                    //�����ж�
	configtime0(1);            //��ʱ1ms
	DS1302init();              //DS1302��ʼ��
	LCD1602init();             //LCD1602��ʼ��
	while(1)
	{
		if(flag200ms == 1)
		{
			flag200ms = 0;
			for(i=0;i<7;i++)
			{
				time[i] = DS1302singleread(i);
			}
			if(sec != time[0])
			{
				str1[0] = '2';
				str1[1] = '0';
				str1[2] = (time[6] >> 4) + '0';  //��ʮλת��Ϊ�ַ�
				str1[3] = (time[6] & 0X0F) + '0';//���λת��Ϊ�ַ�
				str1[4] = '-';
				str1[5] = (time[4] >> 4) + '0';  //��ʮλת��Ϊ�ַ�
				str1[6] = (time[4] & 0X0F) + '0';//�¸�λת��Ϊ�ַ�
				str1[7] = '-';
				str1[8] = (time[3] >> 4) + '0';  //��ʮλת��Ϊ�ַ�
				str1[9] = (time[3] & 0X0F) + '0';//�ո�λת��Ϊ�ַ�
				str1[10] = '\0';
				LCD1602show(3,0,str1);
				
				str2[0] = (time[2] >> 4) + '0';  //ʱʮλת��Ϊ�ַ�
				str2[1] = (time[2] & 0X0F) + '0';//ʱ��λת��Ϊ�ַ�
				str2[2] = ':';
				str2[3] = (time[1] >> 4) + '0';  //��ʮλת��Ϊ�ַ�
				str2[4] = (time[1] & 0X0F) + '0';//�ָ�λת��Ϊ�ַ�
				str2[5] = ':';
				str2[6] = (time[0] >> 4) + '0';  //��ʮλת��Ϊ�ַ�
				str2[7] = (time[0] & 0X0F) + '0';//���λת��Ϊ�ַ�
				str2[8] = '\0';
				LCD1602show(4,1,str2);
				
				sec = time[0];                   //����ǰ��ֵ��ֵ��sec�Ա��´αȽ���ֵ
			}
		}
	}
}
/*DS1302���ֽڶ�����*/
unsigned char DS1302readbyte()
{
	unsigned char dat = 0;
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if(DS1302_SIO != 0)       //�ȶ�ȡSIOֵ������������ʱ��
		{
			dat = dat | mask;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
	return dat;
}
/*DS1302���ֽ�д����*/
void DS1302writebyte(unsigned char dat)
{
	unsigned char mask;
	for(mask=0X01;mask!=0;mask=mask<<1)
	{
		if((mask & dat) != 0)     
		{
			DS1302_SIO = 1;         //��׼����SIO���ݣ�����������ʱ��
		}
		else
		{
			DS1302_SIO = 0;
		}
		DS1302_SCK = 0;
		DS1302_SCK = 1;
		DS1302_SCK = 0;
	}
}
/*DS1302����������*/
unsigned char DS1302singleread(unsigned char addsn)
{
	unsigned char dat;
	DS1302_CE = 1;
	DS1302writebyte((addsn<<1) | 0X81);
	dat = DS1302readbyte();
	DS1302_CE = 0;
	return dat;
}
/*DS1302����д����*/
void DS1302singlewrite(unsigned char addsn,unsigned char dat)
{
	DS1302_CE = 1;
	DS1302writebyte((addsn<<1) | 0X80);
	DS1302writebyte(dat);
	DS1302_CE = 0;
}
/*DS1302��ʼ������*/
void DS1302init()
{
	unsigned char i; 
	unsigned char code date[7]={
		0X00,0X17,0X21,0X27,0X08,0X05,0X21
	};
	DS1302_CE = 0;            
	DS1302_SCK = 0;
	DS1302singlewrite(7,0X00);
	for(i=0;i<7;i++)
	{
		DS1302singlewrite(i,date[i]);
	}
}
/*T0��ʱ����ʱ1ms���ú���*/
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
/*�жϺ�����ÿ1ms����һ��*/
void interrupttime0() interrupt 1
{
	static unsigned char cnt = 0;
	TH0 = TH0R;
	TL0 = TL0R;
	cnt++;
	if(cnt == 200)
	{
		cnt = 0;
		flag200ms = 1;
	}
}