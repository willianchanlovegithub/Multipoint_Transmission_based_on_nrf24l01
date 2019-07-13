
#include <intrins.h>
#include "LCD1602.h"

//延时程序仅适用于STC89xxx
void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

uchar LCD1602_Wait(void) //读1602内部状态字 ，查看是否准备好
{
	uchar Sta;
	LcdRs=0;				   //LcdRS=L,LcdRw=H,LcdEn=H 输出D0~D7状态字
	LcdRw=1;		   Delay1ms();   
	LcdEn=1;	      
  Delay1ms();   
	Sta = LCD1602_Port;	 
  Delay1ms();   							 
	LcdEn=0;
	Sta = Sta & 0x80;
	return Sta;		
}

void LCD1602_Write(bit style, uchar input)
{
	while(LCD1602_Wait());	   			//若忙碌，则等待,免得显示乱码
	LcdEn = 0;											
	LcdRs = style;					        //设置是数据'1',还是命令'0'
	LcdRw = 0;		 
  Delay1ms();	
	LCD1602_Port = input;
 	Delay1ms();		 
	LcdEn = 1;		 
  Delay1ms();	
	LcdEn = 0;	
	Delay1ms();
}

void LCD1602_Initial()
{
	LcdEn=0;
	LCD1602_Write(0,0x38);    //8位数据端口,2行显示,5*7点阵 
	LCD1602_Write(0,0x0c);	  //开启显示, 无光标
	LCD1602_Write(0,0x06);	  //AC递增, 画面不动       
	LCD1602_Write(0,0x01);    //清屏
	Delay10ms();
}

void LCD1602_GotoXY(uchar x, uchar y)	 //X为显示指针的位置，即为各行的第几个位置，Y选行
{
	if(x==0)
		LCD1602_Write(0,0x80+y);
	if(x==1)
		LCD1602_Write(0,(0x80+0x40+y));
}

void LCD1602_Display_NoXY(uchar *str)	//向LCD写入字符串
{
	while(*str!='\0')
	{
		LCD1602_Write(1,*str);
		str++;
	}
}