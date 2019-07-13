#include <reg52.h>
#include "NRF24L01.h"
#include "LCD1602.h"
#include "DS18B20.h"

//************************************主函数*******************************/
void main(void)
{
	uchar Display_Buff[7];
	long uint Send_Count = 0;//记录发送次数计数器
	init_NRF24L01();
//	TX_Mode(); 
	LCD1602_Initial();		//LCD1602初始化
	Delay10ms();
	
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
	
	LCD1602_GotoXY(0,0);
	LCD1602_Display_NoXY("Times:          ");
	LCD1602_GotoXY(1,0);
	LCD1602_Display_NoXY("Send:           ");
	
	TX_Mode();
	
	while(1)
	{
		//给发送缓冲区TxBuf赋值
		TxBuf[0]=datas[0];//显示“+”“-”
		TxBuf[1]=datas[1]+'0';//百位
		TxBuf[2]=datas[2]+'0';//十位
		TxBuf[3]=datas[3]+'0';//个位
		TxBuf[4]=0x2E;//显示小数点“.”
		TxBuf[5]=datas[4]+'0';//小数点后一位
		TxBuf[6]=datas[5]+'0';//小数点后两位
		TxBuf[7]=0x43;//显示“C”
		
		//SPI_Write_Buf(WR_TX_PLOAD, Display_Buff, TX_PLOAD_WIDTH); // 传输的是Display_Buff(传输次数)而不是传输内容(TxBuf)
		SPI_Write_Buf(WR_TX_PLOAD, TxBuf, TX_PLOAD_WIDTH); // 装载数据
		SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e); //CRC使能，16位校验，上电，发射模式，启动发射
		CE=1;
		Delay10us();	   //延时10us，等待发送完毕
		//CheckACK();	
		
		if(!CheckACK())//用于发射检测应答信号，发射成功返回0
		{
			if(Send_Count<99999)
			{
				Send_Count++;	//发送计数器加1	
				Delay1000ms();//每隔一秒发送一次数据
			}
			else
				Send_Count=1;
		}
		
		//下面LCD1602显示操作
		Display_Buff[0] = Send_Count / 10000 + '0';
		Display_Buff[1] = (Send_Count % 10000) / 1000 + '0';
		Display_Buff[2] = (Send_Count % 1000) / 100 + '0';
		Display_Buff[3] = (Send_Count % 100) / 10 + '0';
		Display_Buff[4] = Send_Count % 10 + '0';
		Display_Buff[5] = '\0';
		
		//显示发送的次数
		LCD1602_GotoXY(0,6);
		LCD1602_Display_NoXY(Display_Buff); 
		//把处理后的温度值在1602上显示出来
		LcdDisplayTemp(Ds18b20ReadTemp());
	}
}

