/**************************
按键配置函数
**************************/
#include <reg52.h>
sbit KEYOUT1 = P2^3;
sbit KEYOUT2 = P2^2;
sbit KEYOUT3 = P2^1;
sbit KEYOUT4 = P2^0;
sbit KEYIN1 = P2^4;
sbit KEYIN2 = P2^5;
sbit KEYIN3 = P2^6;
sbit KEYIN4 = P2^7;
unsigned char keyfinally[4][4]={         //按键终值
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
  };
unsigned char code keycodemap[4][4]={    //按键映射表
	{1,2,3,10},
	{4,5,6,12},
	{7,8,9,11},
	{0,14,15,13}
  };
extern void keyaction(unsigned char key);//外部函数声明
/*按键驱动函数*/
void keydriver()
{
	static unsigned char backup[4][4]={    //按键上一个状态值
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
  };
	unsigned char i,j;
	for(i=0;i<4;i++)                       //循环检测按键状态
	{
		for(j=0;j<4;j++)
		{
			if(keyfinally[i][j] != backup[i][j])
			{
				if(backup[i][j] != 0)            //按键按下有效（backup[i][j] == 0为按键弹起检测）
				{
					keyaction(keycodemap[i][j]);
				}
				backup[i][j] = keyfinally[i][j]; //将按键当前状态复制给backup数组，便于下次比较
			}
		}
	}
}
/*按键扫描函数，在中断中每1ms扫描一次*/
void keyscan()
{
	unsigned char i;
	static unsigned char keyout = 0;
	static unsigned char keybuffer[4][4]={ //按键状态缓冲区
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF},
	{0XFF,0XFF,0XFF,0XFF}
	};
	keybuffer[keyout][0] = (keybuffer[keyout][0] << 1) | KEYIN1;
	keybuffer[keyout][1] = (keybuffer[keyout][1] << 1) | KEYIN2;
	keybuffer[keyout][2] = (keybuffer[keyout][2] << 1) | KEYIN3;
	keybuffer[keyout][3] = (keybuffer[keyout][3] << 1) | KEYIN4;
	for(i=0;i<4;i++)                     
	{
		if( (keybuffer[keyout][i] & 0X0F) == 0X00 )//当按键状态缓冲为1111 0000时，
		{                                          //连续四次扫描值为0，即4*4ms都是按下状态认定按键按下
			keyfinally[keyout][i] = 0;
		}
		else if( (keybuffer[keyout][i] & 0X0F) == 0x0F )
		{                                          //当按键状态缓冲为0000 1111时，
			keyfinally[keyout][i] = 1;               //连续四次扫描值为1，即4*4ms都是按下状态认定按键弹起
		}
	}
	keyout++;
	if(keyout == 4)
	{
		keyout = 0;
	}
	switch(keyout)                               //同一时刻只允许有一个KEYOUT=0，通过循环扫描建立矩阵按键
	{                                            //扫描周期16ms
		case 0:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 0;break;
		case 1:KEYOUT4 = 1;KEYOUT3 = 1;KEYOUT2 = 0;KEYOUT1 = 1;break;
		case 2:KEYOUT4 = 1;KEYOUT3 = 0;KEYOUT2 = 1;KEYOUT1 = 1;break;
		case 3:KEYOUT4 = 0;KEYOUT3 = 1;KEYOUT2 = 1;KEYOUT1 = 1;break;
		default:break;
	}	
}