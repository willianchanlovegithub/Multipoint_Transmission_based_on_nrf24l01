
#ifndef __LCD12864_H__
#define __LCD12864_H__

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#define  LCD12864_DB P3		//数据端口

//控制端口定义**********************************************************
sbit LCDEN = P1^7;//使能信号
sbit RS = P1^5;//数据/命令选择段
sbit RW = P1^6;//读/写选择段

//全局变量声明**************************************************************
extern unsigned char datas[9]; //第一位存放正负符号，第二三四六七位存放温度值，第五八位存放小数点'.'和温度单位'C'

//函数声明************************************************************
extern void Lcd12864_Delay_ms(uint z);
extern void Busy_Check(void);
extern void Lcd12864_Write_Cmd(uchar com);
extern void Lcd12864_Write_Data(uchar dat);
extern void Lcd12864_Init(void);
extern void Lcd12864_WriteAddressSetting(uchar x , uchar y);
extern void Lcd12864_WriteSomething(uchar x , uchar y , uchar *str);
extern void LcdDisplayTemp(int temp);

#endif