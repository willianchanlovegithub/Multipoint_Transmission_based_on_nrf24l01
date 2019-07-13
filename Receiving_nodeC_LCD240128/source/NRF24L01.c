
#include <intrins.h>
#include "NRF24L01.h"

uchar const RX_ADDRESS_P0[TX_ADR_WIDTH]= {0x77,0x35,0xF0,0xD3,0xE7}; //数据通道0接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
uchar const RX_ADDRESS_P1[TX_ADR_WIDTH]= {0xC2,0xC2,0xC2,0xC2,0xC2}; //数据通道1接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
uchar const RX_ADDRESS_P2[1]= {0xC3}; //数据通道2接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定，由于只是低八位和通道0地址不一样，只写低八位即可
uchar const RX_ADDRESS_P3[1]= {0xC4}; //数据通道3接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定，由于只是低八位和通道0地址不一样，只写低八位即可
uchar const RX_ADDRESS_P4[1]= {0xC5}; //数据通道4接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定，由于只是低八位和通道0地址不一样，只写低八位即可
uchar const RX_ADDRESS_P5[1]= {0xC6}; //数据通道5接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定，由于只是低八位和通道0地址不一样，只写低八位即可

//********************接收缓冲区*******************************
uchar RxBuf_P0[RX_PLOAD_WIDTH]={0};//接收缓冲区
uchar RxBuf_P1[RX_PLOAD_WIDTH]={0};//通道1接收缓冲区
uchar RxBuf_P2[RX_PLOAD_WIDTH]={0};//通道2接收缓冲区
uchar RxBuf_P3[RX_PLOAD_WIDTH]={0};//通道3接收缓冲区
uchar RxBuf_P4[RX_PLOAD_WIDTH]={0};//通道4接收缓冲区
uchar RxBuf_P5[RX_PLOAD_WIDTH]={0};//通道5接收缓冲区

//********************************************************************************
unsigned char bdata sta; //状态标志
sbit RX_DR =sta^6;
sbit TX_DS =sta^5;
sbit MAX_RT =sta^4;

//*********************延时子函数************************
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay100us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 43;
	while (--i);
}

//***********************************************************************
unsigned char SPI_RW(unsigned char byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // 输出8位
	{
		MOSI = (byte & 0x80); 	//获得byte的最高位数据
		byte = (byte << 1); 	// shift next bit into MSB..
		SCK = 1; 				// Set SCK high..
		byte |= MISO; 			//获得从MISO获得的一位数据
		SCK = 0; 				// ..then set SCK low again
	}
	return(byte);				//最后返回从MISO读出的数据 				
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
	uchar status;
	CSN = 0;              // 使能SPI传输
	status = SPI_RW(reg); //发送寄存器号 
	SPI_RW(value);        //写入寄存器的值
	CSN = 1; 			  // 禁止SPI传输	 
	return(status);       // 返回状态值
}

//读取SPI寄存器值
//reg:要读的寄存器
unsigned char SPI_Read(unsigned char reg)
{
	uchar reg_val;
	CSN = 0; 		     // 使能SPI传输		
	SPI_RW(reg);         // 发送寄存器号
	reg_val = SPI_RW(0); // 读取寄存器内容
	CSN = 1;             // 禁止SPI传输	
	return(reg_val);     // 返回状态值
}

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//bytes:数据长度
//返回值,此次读到的状态寄存器值 
unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	uchar status,byte_ctr;
	CSN = 0; // 使能SPI传输
	status = SPI_RW(reg); // 发送寄存器值(位置),并读取状态值   	   
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0); // 读出数据
	CSN = 1; // 关闭SPI传输
	return(status); // 返回读到的状态值
}

//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//bytes:数据长度
//返回值,此次读到的状态寄存器值
unsigned char SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status,byte_ctr;
	CSN = 0; // 使能SPI传输
	status = SPI_RW(reg); // 发送寄存器值(位置),并读取状态值
	Delay10us();
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // 写入数据	 
		SPI_RW(*pBuf++);
	CSN = 1; //关闭SPI传输
	return(status); // 返回读到的状态值
}

//检测24L01是否存在
//地址寄存器TX_ADDR即可读也可写，所以可以用地址寄存器TX_ADDR验证一个SPI通信是否正常，即检测写进去的和读出来的数据是否一样
//返回值:0，成功;1，失败
uchar NRF24L01_Check(void)
{
	uchar check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	uchar check_out_buf[5]={0x00,0x00,0x00,0x00,0x00};
	
	SPI_Write_Buf(WRITE_REG + TX_ADDR , check_in_buf , 5);  //写入check_in_buf的地址数据
	SPI_Read_Buf(READ_REG + TX_ADDR , check_out_buf , 5);  //读出写入的地址给check_out_buf
	
	//比较check_in_buf和check_out_buf的内容是否一致
	if((check_out_buf[0] == 0x11)&&\
	   (check_out_buf[1] == 0x22)&&\
	   (check_out_buf[2] == 0x33)&&\
       (check_out_buf[3] == 0x44)&&\
       (check_out_buf[4] == 0x55))
		return 0;//内容完全一致，则检测到24L01
	
    else 
		return 1;//否则检测24L01错误
}

//判断是否接收到数据
//接收到返回1，没接收到返回0
//uchar NRFRevDate(uchar *RevDate)
//{
//	uchar RevFlags=0;
//	sta=SPI_Read(STATUS);//发送数据后读取状态寄存器
//  if(RX_DR)				// 判断是否接收到数据
//	{
//		CE=0; 			//SPI使能
//		SPI_Read_Buf(RD_RX_PLOAD,RevDate,RX_PLOAD_WIDTH);// 从RXFIFO读取数据
//		RevFlags=1;	   //读取数据完成标志
//		CE=1;
//		SPI_RW_Reg(WRITE_REG+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清除中断标
//	 	CSN=0;
//		SPI_RW(FLUSH_RX);
//		CSN=1;
//	}
//	return(RevFlags);
//}
/**********************NRF24L01初始化*********************************/
void init_NRF24L01(void)
{	
	Delay100us();
	CSN=1; // SPI片选取消		
	SCK=0; // Spi clock line init high
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了	
void RX_Mode(void)
{
	//NRF设置为接收模式并接收数据
	CE=0;
	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS_P0, RX_ADR_WIDTH);//写RX节点地址（数据通道0）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P1, RX_ADDRESS_P1, RX_ADR_WIDTH);//写RX节点地址（数据通道1）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P2, RX_ADDRESS_P2, 1);//写RX节点地址（数据通道2）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P3, RX_ADDRESS_P3, 1);//写RX节点地址（数据通道3）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P4, RX_ADDRESS_P4, 1);//写RX节点地址（数据通道4）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P5, RX_ADDRESS_P5, 1);//写RX节点地址（数据通道5）
	
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x3F); // 允许通道0~5自动应答
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x3F); //允许通道0~5接收数据
	
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);//选择通道0有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P1, RX_PLOAD_WIDTH);//选择通道1有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P2, RX_PLOAD_WIDTH);//选择通道2有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P3, RX_PLOAD_WIDTH);//选择通道3有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P4, RX_PLOAD_WIDTH);//选择通道4有效数据宽度
	SPI_RW_Reg(WRITE_REG + RX_PW_P5, RX_PLOAD_WIDTH);//选择通道5有效数据宽度
	
	SPI_RW_Reg(WRITE_REG + RF_CH, 40); //设置工作通道频率，Fo=(2400+RF-CH)MHz，2440MHz=2.440GHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f);//设置发射参数（设置发射速率2MHz，发射功率为最大值0dB）
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); //CRC使能，16位校验，上电，接收模式 ，启动接受
	
	CE = 1; //CE置高，启动接收模式
}

//接收数据通道号选择对应的接收缓冲区RxBuf_P0~RxBuf_P5
uchar Rx_Pipe_Num_Choose(void)
{
    uchar state;
    state=SPI_Read(STATUS);  			//读取状态寄存器的值
    SPI_RW_Reg(WRITE_REG + STATUS,state); //清除TX_DS或MAX_RT中断标志
    if(state&0x40)								//接收到数据
    {
        CE = 0;  //待机
        switch(state&0X0E) 
		{
			case 0x00:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P0,RX_PLOAD_WIDTH);
				break;			//读取通道0数据
			case 0x02:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P1,RX_PLOAD_WIDTH);
				break;			//读取通道1数据
			case 0x04:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P2,RX_PLOAD_WIDTH);
				break;			//读取通道2数据
			case 0x06:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P3,RX_PLOAD_WIDTH);
				break;			//读取通道3数据
			case 0x08:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P4,RX_PLOAD_WIDTH);
				break;			//读取通道4数据
			case 0x0A:
				SPI_Read_Buf(RD_RX_PLOAD,RxBuf_P5,RX_PLOAD_WIDTH);
				break;			//读取通道5数据
        }
        SPI_RW_Reg(FLUSH_RX,0xff);					//清除RX FIFO寄存器
        CE = 1;
        Delay100us();
        return 0;
    }
	else
		return 1;  //没收到任何数据
}