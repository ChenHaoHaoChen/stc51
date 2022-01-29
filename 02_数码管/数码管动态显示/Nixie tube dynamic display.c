#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
unsigned char code number[]={   //����number������0-9��Ӧ��16���Ʊ���
	0xc0,0xf9,0xa4,0xb0,0x99,
	0x92,0x82,0xf8,0x80,0x90
  };
unsigned char buffer[]={       //�������ʾ����������
	0xff,0xff,0xff,0xff,0xff,0xff
  };
void secondcount();
void ledrefresh();
void main()
{  
	ENLED = 0;
	ADDR3 = 1;
	TMOD = 0x01;                  //1ms��ʱ
	TH0 = 0xfc;
	TL0 = 0x67;
	TR0 = 1;
	
	while(1)
	{
		if (TF0 == 1)
		{
			TF0 = 0;
			TH0 = 0xfc;
			TL0 = 0x67;
      secondcount();
		}				
	    ledrefresh();  
	}
}


void secondcount()
{
	static unsigned int cnt = 0;         //��10ms������cnt=1000��1s����
	static unsigned long sec = 0;        //�������ʾ������1���һ
	cnt++;
			if (cnt == 1000)
			{
				cnt = 0;
				sec++;
				buffer[0] = number[sec%10];        //��ʾ����
				buffer[1] = number[sec/10%10];     //ʮλ
				buffer[2] = number[sec/100%10];    //��λ
				buffer[3] = number[sec/1000%10];   //ǧλ
				buffer[4] = number[sec/10000%10];  //��λ
		    buffer[5] = number[sec/100000%10]; //ʮ��λ
      }
}
void ledrefresh()
{
	static unsigned char i = 0; 
	P0 = 0xff;     //����������Ϲ�Ӱ 			
	switch (i) 
			 {				 //ÿ1ms����һ�Σ�����һ�������
				 case 0 : ADDR2 = 0; ADDR1 = 0; ADDR0 = 0; P0 = buffer[0]; i++; break ;
			   case 1 : ADDR2 = 0; ADDR1 = 0; ADDR0 = 1; P0 = buffer[1]; i++; break ;
			   case 2 : ADDR2 = 0; ADDR1 = 1; ADDR0 = 0; P0 = buffer[2]; i++; break ;
			   case 3 : ADDR2 = 0; ADDR1 = 1; ADDR0 = 1; P0 = buffer[3]; i++; break ;
			   case 4 : ADDR2 = 1; ADDR1 = 0; ADDR0 = 0; P0 = buffer[4]; i++; break ;
			   case 5 : ADDR2 = 1; ADDR1 = 0; ADDR0 = 1; P0 = buffer[5]; i = 0; break ;
			 }
}	