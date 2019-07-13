
#include "LCD12864.h"

unsigned char datas[9] = {0}; //第一位存放正负符号，第二三四六七位存放温度值，第五八位存放小数点'.'和温度单位'C'

//延时子函数
void Lcd12864_Delay_ms(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

//忙检测（经检测，没有忙检测也是实现显示功能）
void Busy_Check(void)
{
	LCD12864_DB = 0xff;
	RS = 0;
	RW = 1;//WR为高电平的时候为读
	LCDEN = 1;
	while(LCD12864_DB & 0x80);//状态字中STA7，1：禁止，0：使能。当禁止时（忙时），让程序留在while循环中
	Lcd12864_Delay_ms(1);
	LCDEN = 0;
}

//写指令子函数（根据数据手册时序图来写）
void Lcd12864_Write_Cmd(uchar com)
{
	Busy_Check();
	RS = 0;//写指令RS为低电平
	RW = 0;//WR为低电平的时候为写
	LCDEN = 1;
	LCD12864_DB = com;
	Lcd12864_Delay_ms(1);
	LCDEN = 0;
	
}

//写数据子函数（根据数据手册时序图来写）
void Lcd12864_Write_Data(uchar dat)
{
	Busy_Check();
	RS = 1;//写数据RS为高电平
	RW = 0;//WR为低电平的时候为写
	LCDEN = 1;
	LCD12864_DB = dat;
	Lcd12864_Delay_ms(1);
	LCDEN = 0;
}

//初始化子函数
void Lcd12864_Init(void)
{
	Lcd12864_Write_Cmd(0x30);//功能设置
	Lcd12864_Delay_ms(5);
	//Lcd12864_Write_Cmd(0x0f);//光标设置。开显示，下标显示，光标闪烁
	//Lcd12864_Write_Cmd(0x0e);//光标设置。开显示，下标显示，光标不闪烁
	Lcd12864_Write_Cmd(0x0c);//光标设置。开显示，下标不显示，光标不闪烁
	Lcd12864_Delay_ms(5);
	Lcd12864_Write_Cmd(0x01);//清屏
}

//显示地址设置
void Lcd12864_WriteAddressSetting(uchar x , uchar y)
{
	uchar addr;
	if(x == 0) addr = 0x80 + y;//从第一行显示
	else if(x == 1) addr = 0x90 + y;//从第二行显示
	else if(x == 2) addr = 0x88 + y;//从第三行显示
	else if(x == 3) addr = 0x98 + y;//从第四行显示
	Lcd12864_Write_Cmd(addr);
}

//显示字符函数封装
void Lcd12864_WriteSomething(uchar x , uchar y , uchar *str)
{
	Lcd12864_WriteAddressSetting(x , y);
	while(*str != '\0')
	{
		Lcd12864_Write_Data(*str++);
		Lcd12864_Delay_ms(1);//控制每行中每个字符显示快慢，也可以不要延时
	}
}

void LcdDisplayTemp(int temp)  //lcd显示
{
	float tp;  
	if(temp < 0)  //当温度值为负数
  	{
	  	Lcd12864_WriteSomething(3 , 4 , '-');  //显示负
		datas[0]=0x2d;  //把“-”的ASCII码存放在datas[]数组第一位
		
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
	  	Lcd12864_WriteSomething(3 , 4 , '+');  //显示正
		datas[0]=0x2b;  //把“+”的ASCII码存放在datas[]数组第一位
			
		tp=temp;  //因为数据处理有小数点所以将温度赋给一个浮点型变量
		          //如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	datas[1] = (temp / 10000)+'0';
	datas[2] = (temp % 10000 / 1000)+'0';
	datas[3] = (temp % 1000 / 100)+'0';
	datas[4] = '.';
	datas[5] = (temp % 100 / 10)+'0';
	datas[6] = (temp % 10)+'0';
	datas[7] = 'C';
	
	Lcd12864_WriteSomething(3 , 4 , datas); //在第四行第四个字符位置开始显示温度，例：+025.52C
}
