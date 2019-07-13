#include <reg52.h>
#include "NRF24L01.h"
#include "LCD12864.h"
#include "DS18B20.h"

//************************************主函数*******************************/
void main(void)
{
	init_NRF24L01();
	Lcd12864_Init();
	Lcd12864_Delay_ms(10);
	
	//用于检测nRF24L01模块是否存在或者正常工作
	if(!NRF24L01_Check())
	{
		Lcd12864_WriteSomething(0 , 0 , "nRF24L01+ Module");
		Lcd12864_WriteSomething(1 , 0 , "Check OK!");
		Lcd12864_WriteSomething(2 , 0 , "System detection");
		Lcd12864_WriteSomething(3 , 0 , "is normal!");
		Lcd12864_Delay_ms(1500);
		Lcd12864_WriteSomething(0 , 0 , "A-TEMP:         ");
		Lcd12864_WriteSomething(1 , 0 , "B-TEMP:         ");
		Lcd12864_WriteSomething(2 , 0 , "C-TEMP:         ");
		Lcd12864_WriteSomething(3 , 0 , "D-TEMP:         ");
	}
	else
	{
		Lcd12864_WriteSomething(0 , 0 , "nRF24L01+ Module");
		Lcd12864_WriteSomething(1 , 0 , "Check Failed!");
		Lcd12864_WriteSomething(2 , 0 , "只可显示本机温度");
		Lcd12864_WriteSomething(3 , 0 , "L-temp:         ");
		while(1)
		{
			LcdDisplayTemp(Ds18b20ReadTemp());//把处理后的温度值在12864上显示出来（本机温度）
		}
	}
	
	RX_Mode();
	
	while(1)
	{
		if(!Rx_Pipe_Num_Choose())
		{
			Lcd12864_WriteSomething(0 , 4 , RxBuf_P0);//显示数据通道0的数据
			Lcd12864_WriteSomething(1 , 4 , RxBuf_P1);//显示数据通道1的数据
			Lcd12864_WriteSomething(2 , 4 , RxBuf_P2);//显示数据通道2的数据
			Lcd12864_WriteSomething(3 , 4 , RxBuf_P3);//显示数据通道3的数据
		}
	}
}