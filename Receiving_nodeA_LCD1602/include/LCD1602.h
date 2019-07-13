

#ifndef __LCD1602_H__
#define __LCD1602_H__

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define  LCD1602_Port P0		//数据端口

//**************************控制端口定义************************************//
sbit LcdRs = P2^3;
sbit LcdRw = P2^4;
sbit LcdEn = P2^5;	


//*************************1602全局变量************************************//
extern void Delay1000ms();
extern void Delay10ms();
extern void Delay1ms();
extern void LCD1602_Write(bit style, uchar input);
extern void LCD1602_Initial();
extern void LCD1602_GotoXY(uchar x, uchar y);
extern void LCD1602_Display_NoXY(uchar *str);
	
#endif