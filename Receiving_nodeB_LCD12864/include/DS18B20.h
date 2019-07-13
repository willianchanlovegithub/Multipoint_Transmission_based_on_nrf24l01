
#ifndef __DS18B20_H__
#define __DS18B20_H_

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//IO口定义**************************************************************
sbit DSPORT=P0^0;//DS18B20数据口

//函数声明**************************************************************
extern void Delay1ms();
extern uchar Ds18b20Init();
extern void Ds18b20WriteByte(uchar dat);
extern uchar Ds18b20ReadByte();
extern void  Ds18b20ChangTemp();
extern void  Ds18b20ReadTempCom();
extern int Ds18b20ReadTemp();

#endif