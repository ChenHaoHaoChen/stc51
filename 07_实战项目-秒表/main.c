#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit ESC = P2^5;
sbit ENTER = P2^6;
unsigned char code number[10]={   //�������ʾ��������     
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
 };
unsigned char keyfinally[2]={     //������ֵ
	1,1
 };
unsigned char ledbuffer[6]={      //��λ�������ʾ��������
	0XFF,0XFF,0XFF,0XFF,0XFF,0XFF
 };
unsigned char decimal = 0;        //��ʱС������
unsigned int integer = 0;         //��ʱ��������
unsigned char TH01;               //��ʱ����֮���λ
unsigned char TL01;               //��ʱ����֮���λ
bit symbol = 0;                   //��ʼ���ݶ���������ʱ��־λ
void configtime(unsigned char ms);//������������
void keydriver();           
void stop();
void run();
void count();
void display();
void ledscan();
void keyscan();

void main()
{
	ENLED = 0;                      //��74138������
	ADDR3 = 1;
	TMOD = 0X01;                    //���ö�ʱ��Ϊ1ģʽ
	EA = 1;                         //�ж�ʹ�ܴ�
	P2 = 0XFE;                      // 1111 1110 �򿪵����Ű���
	configtime(2);                  //��ʱ�������Ӳ�����һ�㲹��40��
	while(1)
	{
		keydriver();
	}
}

void configtime(unsigned char ms)
{
	unsigned long temp = 0;
	temp = 65536-(ms*11059200)/(12*1000);//��ʱ2ms
	temp = temp + 40;                    //����40
	TH01 = (unsigned char )(temp >> 8);  //��λ��ֵ��TH01
	TL01 = (unsigned char )temp;         //��λ��ֵ��TL01
	TH0 = TH01;
	TL0 = TL01;
	TR0 = 1;                             //��T0��ʱ
	ET0 = 1;                             //��T0�ж�
}

void keydriver()                       //������⺯�� 
{
	static unsigned char backup[2]={     //������һ��״̬
		1,1
	};
	static unsigned char i;
	for(i=0;i<2;i++)
	{
		if(keyfinally[i] !=backup[i])
		{
			if(backup[i] == 0)
			{
				if(i == 0)                     //i == 0ʱ���϶�ESC��������
				{
					stop();                      //ESC�������£����㲢ֹͣ����            
				}
				else if(i == 1)                //i == 1ʱ���϶�ENTER��������
				{
					run();                       //ENTER�������£���ʼ��ʱ������ͣ��ʱ
				}
			}
			backup[i] = keyfinally[i];       //��ǰ״̬��ֵ��backup
		}
	}
}

void stop()                            //ֹͣ��������
{
	decimal = 0;                         //С����������������
	integer = 0;
	symbol = 0;                          //��־λ���㣬�������ֹͣ������һֱ��ʾ0.00
}

void run()                             //��ʼ��ʱ������ͣ����
{
	if(symbol)                           //���ڿ�ʼ����ͣ��ת��
	{
		symbol = 0;
	}
	else
	{
		symbol = 1;
	}
}

void interrupttime0() interrupt 1      //ÿ2ms����һ���ж�
{
	static unsigned char ms10 = 0;       //10ms��ʱ
	TH0 = TH01;                          //���¶�ʱ
	TL0 = TL01;
	ms10++;
	if(ms10 == 5)                        //����ʱ��10msʱ
	{
		ms10 = 0;                          //10ms��ʱ����
		count();                           //����������ÿ10ms��һ
	}
	display();                           //�������ʾ����
	ledscan();                           //�����ɨ�躯��
	keyscan();                           //����ɨ�躯��
}

void count()
{
	if(symbol)                            //������־λsymbol=1ʱ����ʼ����
	{
	  decimal++;
	  if(decimal == 100)
	  {
		  decimal = 0;
		  integer++;
		  if(integer == 10000)              //���ʱ��9999.99s��Լ����������Сʱ
		  {
			  integer = 0;
		  }
	  }
	}
}

void display()                          //�������ʾ����
{
	static unsigned char i;
	ledbuffer[0] = number[decimal%10];
	ledbuffer[1] = number[decimal/10%10];
	ledbuffer[2] = number[integer%10] & 0X7F;
	ledbuffer[3] = number[integer/10%10];
	ledbuffer[4] = number[integer/100%10];
	ledbuffer[5] = number[integer/1000%10];
	for(i=5;i>2;i--)                     //��λ����
	{
		if(ledbuffer[i] == 0XC0)
		{
			ledbuffer[i] = 0XFF;
		}
		else
		{
			break;
		}
	}
}

void ledscan()                         //�����ɨ�躯����ÿ2ms�л������һ��
{
	static unsigned char i = 0;
	P0 = 0XFF;                           //���������
	switch(i)
	{
		case 0:ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 1:ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 2:ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 3:ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;P0 = ledbuffer[i];i++;break;
		case 4:ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;P0 = ledbuffer[i];i++;break;
		case 5:ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;P0 = ledbuffer[i];i=0;break;
		default:break;
	}
}

void keyscan()                              //����ɨ�躯��
{
	static unsigned char keybuffer[2]={       //����ɨ�軺������
		0XFF,0XFF 
	};
	static unsigned char i;
	keybuffer[0] = (keybuffer[0] <<1) | ESC;  //ÿ2ms��λһ��
	keybuffer[1] = (keybuffer[1] <<1) | ENTER;
	for(i=0;i<2;i++)
	{
		if( (keybuffer[i] ) == 0X00 )           //����ɨ��8�ζ���0ʱ����16ms��Ϊ����״̬ʱ���϶���������
		{
			keyfinally[i] = 0;
		}
		else if( (keybuffer[i] ) == 0XFF )      //����ɨ��8�˶���1ʱ����16ms��Ϊ����״̬ʱ���϶���������
		{
			keyfinally[i] = 1;
		}
	}
}