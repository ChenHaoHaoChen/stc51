#include <reg52.h>
bit flag500ms = 0;               //500ms��ʱ��־λ
unsigned char TH0R;              //��ʱ��T0��λ����ֵ
unsigned char TL0R;              //��ʱ��T0��λ����ֵ
unsigned char code str1[]={    //��Ҫ��ʾ���ַ���1
		"                Ben Gui Xin                    "
	};
	unsigned char code str2[]={    //��Ҫ��ʾ���ַ���2
		"                Shi Wo Er                      "
	};

void configtime(unsigned int ms);//��������  
extern void LCD1602init();       //�ⲿ��������
extern void LCD1602show(unsigned char x,unsigned char y,unsigned char *p);//�ⲿ��������

void main()
{
	unsigned char index = 0;       //ָ������
	EA = 1;                        //�����ж�
	LCD1602init();                 //LCD1602��ʼ������            
	configtime(10);                //���ö�ʱʱ��10ms
	while(1)
	{
		if(flag500ms)   
		{
			flag500ms = 0;
			LCD1602show(0,0,str1+index);//LCD1602��ʾ���� 
	    LCD1602show(0,1,str2+index);
			index++;
			if(index == (sizeof(str1)-1))
			{
				index = 0;
			}
		}
	}
}
/*���ö�ʱʱ�亯��*/
void configtime(unsigned int ms)
{
	unsigned long temp;
	temp = 65536 - (11059200/12*ms/1000);//���㶨ʱʱ��
	TH0R = (unsigned char)(temp >> 8); 
	TL0R = (unsigned char) temp;
	TMOD = TMOD & 0XF0; //���ö�ʱ��Ϊģʽ1
	TMOD = TMOD | 0X01;
	TH0 = TH0R;         
	TL0 = TL0R;
	TR0 = 1;            //��ʼ��ʱ
	ET0 = 1;            //�򿪶�ʱ��T0�ж�
}
/*��ʱ��T0�жϣ�ÿ10ms����һ��*/
void interrupttime0() interrupt 1
{
	static unsigned char i = 0;
	TH0 = TH0R;         //���¶�ʱ
	TL0 = TL0R;
	i++;
	if(i == 50)
	{
		i = 0;
		flag500ms = 1;    //��ʱ��500msʱ����־λ��1
	}
}