#include <reg52.h>
unsigned char cmdarriver = 0;        //cmdarriverΪ���������������������ж�ʱ��1
unsigned char index = 0;             //indexΪ����SBUF����
unsigned char cnt = 0;               //cntΪ�����������������Ԫ�ظ���
unsigned char *p;                    //ָ�����p
unsigned char arrary1[]={"1-Hello!\r\n"};        //����1-4
unsigned char arrary2[]={'2','-','H','e','l','l','o','!','\r','\n'};
unsigned char arrary3[]={51,45,72,101,108,108,111,33,13,10};
unsigned char arrary4[]={"4-Hello!\r\n"};
void configUART(unsigned int buad);  //��������
void main()
{
	EA = 1;                            //���ж�ʹ��
	configUART(9600);                  //���ò�����Ϊ9600
	while(1)
	{
		if(cmdarriver)                  
	  {
		  cmdarriver = 0;
		  switch(index)
		  {
			  case 1:p = arrary1;          //pָ��ָ��arrary1�����׵�ַ������һ��Ԫ�ص�ַ
			         cnt = sizeof(arrary1);//����arrary1����Ԫ�ظ���
			         TI = 1;               //�����ֱ�ӽ����ж�
			         break;
			  case 2:p = arrary2;
			         cnt = sizeof(arrary2);
			         TI = 1;
			         break;
			  case 3:p = arrary3;
			         cnt = sizeof(arrary3);
						   TI = 1;
			         break;
			  case 4:p = arrary4;
			         cnt = sizeof(arrary4);
			         TI = 1;			
			         break;
			  default:
				       break;
		  }
	  }
	}
}
void configUART(unsigned int buad)
{
	SCON = 0X50;                       //����UART����Ϊģʽ1����õ�ģʽ��
	TMOD = TMOD & 0X0F;                //���ö�ʱ��1Ϊģʽ2�����Զ���װģʽ
	TMOD = TMOD | 0X20;
	TH1 = 256 - (11059200/12/32)/buad; //�̶���ʽ���㲨����
	TL1 = TH1;
	TR1 = 1;                           //�򿪶�ʱ��1
	ES = 1;                            //��UART�����ж�  
}
/*UART�����жϣ���RI=1�������жϱ�־λ�����͵�ֹͣλ�м�λ�ã������ж�
                ��TI=1�������жϱ�־λ�����ܵ�ֹͣλ�м�λ�ã������ж�*/
void interruptUART() interrupt 4
{
	if(RI)
	{
		RI = 0;
		cmdarriver = 1;
		index = SBUF;
	}
	if(TI)
	{
		TI = 0;
		if(cnt > 0)
		{
			SBUF = *p;
			p++;
			cnt--;
		}
	}
}			