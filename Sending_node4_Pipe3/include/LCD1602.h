
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

#define  LCD1602_Port P1		//数据端口

//控制端口定义**********************************************************
sbit LcdRs		= P3^3;
sbit LcdRw    = P3^4;
sbit LcdEn  	= P3^5;

//全局变量声明**************************************************************
extern unsigned char datas[6]; //第一位存放正负符号，第二位到第六位存放温度值

//函数生声明************************************************************
extern void Delay1000ms();
extern void Delay10ms();
extern void Delay1ms();
extern void LCD1602_Write(bit style, uchar input);
extern void LCD1602_Initial();
extern void LCD1602_GotoXY(uchar x, uchar y);
extern void LCD1602_Display_NoXY(uchar *str);
extern void LcdDisplayTemp(int temp);

#endif