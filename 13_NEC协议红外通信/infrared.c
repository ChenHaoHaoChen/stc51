/*****************
NEC����ң�����ó���
*****************/
#include <reg52.h>
sbit IRD = P3^3;         //NEC����ͨ������   
unsigned char irflag = 0;//NEC����ͨ�Ž�����ɱ�־
unsigned char ircode[4]; //NEC����ͨ�Ž����ַ���0��1Ϊ�û�����û�����
/*NEC����ͨ�ų�ʼ��*/                         //2��3Ϊ������Ͱ�������
void Infraredinit()                           
{               
	TMOD = TMOD & 0X0F;   //���ö�ʱ��1Ϊ1ģʽ
	TMOD = TMOD | 0X10;
	TR1 = 0;              //ֻ��ǿ����������ⲿ�жϣ�����Ҳ��
	ET1 = 0;
	IT1 = 1;              //�ⲿ�ж�1������־λֵ1�����½��ش���
	EX1 = 1;              //���ⲿ�ж�ʹ��
}
/*��ȡ�͵�ƽʱ��*/
unsigned int getlowtime()
{
	TH1 = 0;           
	TL1 = 0;
	TR1 = 1;               //��ʼ��ʱ
	while(!IRD)
	{
		if(TH1 >= 0XDC)      //�ȴ�10ms������Ӧ���Ƴ�
		{
			break;
		}
	}
	TR1 = 0;               //�رն�ʱ
	return (TH1*256 + TL1);//���ض�ʱʱ��
}
/*��ȡ�ߵ�ƽʱ��*/
unsigned int gethightime()
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;               //��ʼ��ʱ
	while(IRD)
	{
		if(TH1 >= 0XDC)      //�ȴ�10ms������Ӧ���Ƴ�
		{
			break;
		}
	}
	TR1 = 0;               //�رն�ʱ
	return (TH1*256 + TL1);//���ض�ʱʱ��
}
/*�ⲿ�ж�1*/
void Infraedinterrupt() interrupt 2
{
	unsigned int time;     //timeΪ��ȡ�ߵ͵�ƽʱ��
	unsigned char i,j;
	unsigned char byte;    //����ͨ���ֽ�
	/*����������е�9ms�ز�����ΧΪ��8.5ms~9.5ms*/
	time = getlowtime();   //��ȡ�ز�ʱ�䣨�ز�Ϊ�͵�ƽ��
	if( (time < 7833) || (time > 8755) )
	{
		IE1 = 0;
		return;              //�����������˳��ж�
	}
	/*����������е�4.5ms���У���ΧΪ��4ms~5ms*/
	time = gethightime();  //��ȡ����ʱ�䣨����Ϊ�ߵ�ƽ��
	if( (time <3686 ) || (time > 4608) )
	{
		IE1 = 0;
		return;              //�����������˳��ж�
	}
	/*ѭ���Ĵηֱ��ȡ4���ֽڵ��û��룬�û����룬�����룬��������*/
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			/*���560us�ز�����ΧΪ��280us~840us*/
			time = getlowtime();//��ȡ�ز�ʱ��
			if( (time <258) || (time >774) )
			{
				IE1 = 0;
				return;
			}
			time = gethightime();//��ȡ����ʱ��
			if( (time > 258) && (time < 774) )
			{
				byte = byte >> 1;  //������ʱ��Ϊ560us����Χ��280us~840us���������Ϊ��0
			}
			else if( (time > 922) && (time < 2175) )
			{
				byte = byte >> 1;  //������ʱ��Ϊ1.68ms����Χ��1ms~2.36ms���������Ϊ��1
				byte = byte | 0X80;
			}
			else 
			{
				IE1 = 0;           
				return;             //�������ǣ����˳��ж�
			}
		}
		ircode[i] = byte;       //ѭ������ȡ�����ֽڴ����������
	}
	irflag = 1;               //����ͨ����ɣ�����ͨ�ű�־λ��1
	IE1 = 0;                  //�ⲿ�ж�1�ж������־λ����
}