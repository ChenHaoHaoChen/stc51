/**************************
      TLC549ͨ�ų���
**************************/
#include <reg52.h>
#include <intrins.h>
#include "main.h"

unsigned char RV;  //���������ѹ�ɼ�ֵ
unsigned char ADsampling();

/**********ADɨ��**********/
void ADscan()
{
	if(RV < (255/5)*1.25)  //(255/5)*1.25 = 64��Ϊ1.25V
	{
		D1501 = 0;
	}
	else
	{
		D1501 = 1;
	}
}
/**********ADˢ�º���(ÿ100msˢ��)**********/
void ADrefresh()
{
	static unsigned char bit_AD = 0; 
	bit_AD++;
	if(bit_AD == 50)
	{
		bit_AD = 0;
		RV = ADsampling();
	}
}
/**********AD�ɼ�����**********/
unsigned char ADsampling()
{
	unsigned char i,temp = 0;
	CLK = 0;
	//AD_out = 1;
	_nop_();
	_nop_();
	 CS = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	if(ADO == 1)
	{
		temp = temp + 1;
	}
	for(i=0;i<8;i++)
	{
		CLK = 1;
		_nop_();
	  _nop_();
		CLK = 0;
		_nop_();
		_nop_();
		if(i != 7)
		{
			temp = temp << 1;
			if(ADO == 1)
			{
				temp = temp + 1;
			}
		}
	}
	CS = 1;
	return temp;
}