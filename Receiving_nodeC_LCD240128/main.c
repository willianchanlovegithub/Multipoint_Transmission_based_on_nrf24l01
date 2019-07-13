
//240128满屏可直接显示16行*30列=480个字符
//240128满屏可显示8行*15列=120个16*16字模的汉字
//lcdwc(0x90);//关闭文本显示，关闭图形显示
//lcdwc(0x94);//启用文本显示，关闭图形显示
//lcdwc(0x98);//关闭文本显示，启用图形显示
//lcdwc(0x9c);//启用文本显示，启用图形显示

#include <reg52.h>
#include <intrins.h>
#include "LCD240128.h"
#include "NRF24L01.h"
#include "Serial_Com.h"

//1s延时函数
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

//主函数
void main(void)
{   
	lcdreset();					//初始化液晶显示控制器
	lcdfill(0x00);				//清屏
	UART_Init();				//初始化串口通信
	init_NRF24L01();			//初始化nRF24L01无线模块
	RX_Mode();					//初始化nRF24L01到接收模式
	
	//用于检测nRF24L01模块是否存在或者正常工作
	if(!NRF24L01_Check())
	{
		DirectDisplayString(0,5,"  nRF24L01+ Module Check OK!  ");
		//图片显示: 在(104,50)位置显示32*32点阵图标
		XPOS=104;
		YPOS=50;
		putimage(Img_check_ok_32);	//写数据到当前LCDRAM地址中
		Delay1000ms();
		lcdfill(0x00);				//清屏
		beautifulshow();			//图形画面展示程序
		DirectDisplayString(0,5, ">>> A-Temperature: NO DATA <<<");
		DirectDisplayString(0,7, ">>> B-Temperature: NO DATA <<<");
		DirectDisplayString(0,9, ">>> C-Temperature: NO DATA <<<");
		DirectDisplayString(0,11,">>> D-Temperature: NO DATA <<<");
	}
	else
	{
		DirectDisplayString(0,5,"nRF24L01+ Module Check Failed!");
		//图片显示: 在(104,50)位置显示32*32点阵图标
		XPOS=104;
		YPOS=50;
		putimage(Img_check_no_ok_32);	//写数据到当前LCDRAM地址中
		while(1);
	}
	
	while(1)
	{
		//根据不同的数据通道显示各自的数据
		if(!Rx_Pipe_Num_Choose())
		{
			//格式PutString(x轴坐标,y轴坐标,数据);
			DirectDisplayString(18,5, RxBuf_P0);//显示数据通道0的数据
			DirectDisplayString(18,7, RxBuf_P1);//显示数据通道1的数据
			DirectDisplayString(18,9, RxBuf_P2);//显示数据通道2的数据
			DirectDisplayString(18,11,RxBuf_P3);//显示数据通道3的数据
			
			//串口通信发送数据
			UART_SendStr(RxBuf_P0);//数据通道0的数据通过串口发送给上位机
			UART_SendStr(RxBuf_P1);//数据通道1的数据通过串口发送给上位机
			UART_SendStr(RxBuf_P2);//数据通道2的数据通过串口发送给上位机
			UART_SendStr(RxBuf_P3);//数据通道3的数据通过串口发送给上位机
		}
	}
}