
#include <reg52.h>
#include "NRF24L01.h"
#include "LCD1602.h"

//************************************主函数*******************************
void main(void)
{
//	uchar Display_Buff[7];           //显示缓冲接收次数
//	long uint Recive_Count = 0;      //记录接收成功次数计数器
	init_NRF24L01() ;
//	RX_Mode();
	LCD1602_Initial();		//LCD1602初始化
	Delay10ms();					//稍作延时
	
	//用于检测nRF24L01模块是否存在或者正常工作
	if(!NRF24L01_Check())
	{
		LCD1602_GotoXY(0,0);
		LCD1602_Display_NoXY("nRF24L01+ Module");
		LCD1602_GotoXY(1,0);
		LCD1602_Display_NoXY("Check OK!");	
		Delay1000ms();
		Delay1000ms();
	}
	else
	{
		LCD1602_GotoXY(0,0);
		LCD1602_Display_NoXY("nRF24L01+ Module");
		LCD1602_GotoXY(1,0);
		LCD1602_Display_NoXY("Check Failed!");
		while(1);
	}
	
	//初始化界面
	LCD1602_GotoXY(0,0);
	LCD1602_Display_NoXY("                ");
	LCD1602_GotoXY(1,0);
	LCD1602_Display_NoXY("                ");
	
	RX_Mode();
	
	while(1)
	{
		
		if(!Rx_Pipe_Num_Choose())
		{
			LCD1602_GotoXY(0,0);
	 		LCD1602_Display_NoXY(RxBuf_P0);//显示数据通道0的数据
			LCD1602_GotoXY(0,8);
	 		LCD1602_Display_NoXY(RxBuf_P1);//显示数据通道1的数据
			LCD1602_GotoXY(1,0);
	 		LCD1602_Display_NoXY(RxBuf_P2);//显示数据通道2的数据
			LCD1602_GotoXY(1,8);
	 		LCD1602_Display_NoXY(RxBuf_P3);//显示数据通道3的数据
		}
	}		
}