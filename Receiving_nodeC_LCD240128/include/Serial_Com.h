

#ifndef __SERIAL_COM_H__
#define __SERIAL_COM_H__

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

extern void UART_Init(void);//串口初始化函数
extern void UART_SendByte(uchar uDat);//发送一个字节函数
extern void UART_SendStr(uchar *pStr);//发送一个字符串的函数
	
#endif