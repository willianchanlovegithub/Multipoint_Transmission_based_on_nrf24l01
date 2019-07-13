
#include <intrins.h>
#include "LCD1602.h"

unsigned char datas[6] = {0}; //第一位存放正负符号，第二位到第六位存放温度值

//延时程序仅适用于STC89xxx
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

/*uchar LCD1602_Wait(void)		   //读1602内部状态字 ，查看是否准备好
{
	uchar Sta;
	LcdRs=0;				   //LcdRS=L,LcdRw=H,LcdEn=H 输出D0~D7状态字
	LcdRw=1;		 
        Delay1ms();   
	LcdEn=1;	      
        Delay1ms();   
	Sta = LCD1602_Port;	 
        Delay1ms();   							 
	LcdEn=0;
	Sta = Sta & 0x80;
	return Sta;		
}*/

void LCD1602_Write(bit style, uchar input)
{
	//while(LCD1602_Wait());			//若忙碌，则等待,免得显示乱码
	LcdEn = 0;											
	LcdRs = style;					        //设置是数据'1',还是命令'0'
	//LcdRw = 0;		  
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
  LcdRw=0;
	LcdEn=0;
	LCD1602_Write(0,0x38);               //8位数据端口,2行显示,5*7点阵
	LCD1602_Write(0,0x0c);  //开启显示, 无光标
	LCD1602_Write(0,0x06);       //AC递增, 画面不动
	LCD1602_Write(0,0x01);   //清屏
	Delay10ms();
}

void LCD1602_GotoXY(uchar x, uchar y)	   //X为显示指针的位置，即为各行的第几个位置，Y选行
{
	if(x==0)
		LCD1602_Write(0,0x80+y);
	if(x==1)
		LCD1602_Write(0,(0x80+0x40+y));
}

void LCD1602_Display_NoXY(uchar *str)					 //向LCD写入字符串
{
	while(*str!='\0')
	{
		LCD1602_Write(1,*str);
		str++;
	}
}


//1602显示温度值
void LcdDisplayTemp(int temp) 	 //lcd显示
{
	
    
  //unsigned char datas[5] = {0, 0, 0, 0, 0}; //定义数组，存放温度值
	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
	  	LCD1602_GotoXY(1,5);
	    LCD1602_Write(1,'-');  		//显示负
			datas[0]=0x2d;//把“-”的ASCII码存放在datas[]数组第一位
			
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算由?.5，还是在小数点后面。
 
  	}
 	else
  	{			
	  	LCD1602_GotoXY(1,05);
	    LCD1602_Write(1,'+');  		//显示负
			datas[0]=0x2b;//把“+”的ASCII码存放在datas[]数组第一位
			
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[1] = temp / 10000;
	datas[2] = temp % 10000 / 1000;
	datas[3] = temp % 1000 / 100;
	datas[4] = temp % 100 / 10;
	datas[5] = temp % 10;
	
	LCD1602_GotoXY(1,6);
	LCD1602_Write(1,'0'+datas[1]); //百位 	
	LCD1602_GotoXY(1,7);
	LCD1602_Write(1,'0'+datas[2]); //十位
	LCD1602_GotoXY(1,8);
	LCD1602_Write(1,'0'+datas[3]); //个位 
	LCD1602_GotoXY(1,9);
	LCD1602_Write(1,'.'); 		//显示 ‘.’
	LCD1602_GotoXY(1,10);
	LCD1602_Write(1,'0'+datas[4]); //显示小数点后一位
	LCD1602_GotoXY(1,11);
	LCD1602_Write(1,'0'+datas[5]); //显示小数点后两位
	LCD1602_GotoXY(1,12);
	LCD1602_Write(1,0x43); //显示“C”
}
