
#include <intrins.h>
#include "NRF24L01.h"

//uchar const TX_ADDRESS_P0[TX_ADR_WIDTH]= {0x77,0x35,0xF0,0xD3,0xE7}; //数据通道0发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const RX_ADDRESS_P0[RX_ADR_WIDTH]= {0x77,0x35,0xF0,0xD3,0xE7}; //数据通道0接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const TX_ADDRESS_P1[TX_ADR_WIDTH]= {0xC2,0xC2,0xC2,0xC2,0xC2}; //数据通道1发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const RX_ADDRESS_P1[RX_ADR_WIDTH]= {0xC2,0xC2,0xC2,0xC2,0xC2}; //数据通道1接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const TX_ADDRESS_P2[TX_ADR_WIDTH]= {0xC3,0xC2,0xC2,0xC2,0xC2}; //数据通道2发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const RX_ADDRESS_P2[RX_ADR_WIDTH]= {0xC3,0xC2,0xC2,0xC2,0xC2}; //数据通道2接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
uchar const TX_ADDRESS_P3[TX_ADR_WIDTH]= {0xC4,0xC2,0xC2,0xC2,0xC2}; //数据通道3发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
uchar const RX_ADDRESS_P3[RX_ADR_WIDTH]= {0xC4,0xC2,0xC2,0xC2,0xC2}; //数据通道3接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const TX_ADDRESS_P4[TX_ADR_WIDTH]= {0xC5,0xC2,0xC2,0xC2,0xC2}; //数据通道4发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const RX_ADDRESS_P4[RX_ADR_WIDTH]= {0xC5,0xC2,0xC2,0xC2,0xC2}; //数据通道4接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const TX_ADDRESS_P5[TX_ADR_WIDTH]= {0xC6,0xC2,0xC2,0xC2,0xC2}; //数据通道5发射地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定
//uchar const RX_ADDRESS_P5[RX_ADR_WIDTH]= {0xC6,0xC2,0xC2,0xC2,0xC2}; //数据通道5接收地址，先写低字节，所写字节数量由SETUP_AW（设置地址宽度的寄存器）设定

//********************发送缓冲区*******************************
uchar TxBuf[TX_PLOAD_WIDTH]={0};

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

//************************************************
unsigned char SPI_RW(unsigned char byte)
{
	uchar bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // 输出8位
	{
		MOSI = (byte & 0x80); 	// output 'byte', MSB to MOSI  获得byte的最高位数据
		byte = (byte << 1); 	// shift next bit into MSB..
		SCK = 1; 				// Set SCK high..
		byte |= MISO; 			// capture current MISO bit 获得从MISO获得的一位数据
		SCK = 0; 				// ..then set SCK low again
	}
	return(byte);				// return read byte	最后返回从MISO读出的数据 				
}

/********************************************************/
unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
{
	uchar status;
	CSN = 0;              // CSN low, init SPI transaction
	status = SPI_RW(reg); //返回从MISO读出的数据,status应为上次向该寄存器内写的value
	SPI_RW(value);        // and write value to it
	CSN = 1; 			  // CSN high again
	return(status);       // return nRF24L01 status byte
}

/*************************读出从机相关寄存器的数据*******************************/
unsigned char SPI_Read(unsigned char reg)
{
	uchar reg_val;
	CSN = 0; 		     // CSN low, initialize SPI communication...
	SPI_RW(reg);         // Select register to read from..
	reg_val = SPI_RW(0); // ..then read registervalue
	CSN = 1;             // CSN high, terminate SPI communication
	return(reg_val);     // return register value
}

/**********************读缓冲区**********************************/
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

/**********************写缓冲区**********************************/
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//bytes:数据长度
//返回值,此次读到的状态寄存器值
unsigned char SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes)
{
	uchar status,byte_ctr;
	CSN = 0; // Set CSN low, init SPI tranaction
	status = SPI_RW(reg); // Select register to write to and read status byte
	Delay10us();
	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
	SPI_RW(*pBuf++);
	CSN = 1; // Set CSN high again
	return(status); // return nRF24L01 status byte
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

/********************配置Tx模式******************/
void TX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS_P3, TX_ADR_WIDTH); //写TX节点的地址（数据通道3）
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS_P3, RX_ADR_WIDTH);//写RX节点的地址（接收地址）
	
	//这两行代码貌似不要nRF24L01也是能够正常发送的，为什么呢？
//	SPI_RW_Reg(WRITE_REG + EN_AA, 0x3F); // 允许通道0通道1通道2自动应答（三发一收）
//	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x3F); //接收数据通道0通道1通道2允许（三发一收）
	
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 自动重发延时500us + 86us,自动重发10次 
	SPI_RW_Reg(WRITE_REG + RF_CH, 40); //设置工作通道频率
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x0f); // 设置发射速率2MHz，发射功率为最大值0dB 
	SPI_RW_Reg(WRITE_REG + CONFIG,0x0e);//CRC使能，16位校验，上电，发射模式，启动发射
	CE=1;
}

//**********************NRF24L01初始化*********************************/
void init_NRF24L01(void)
{
	Delay100us();
	CSN=1; // Spi disable
	SCK=0; // Spi时钟线初始化	  
}
/****************************用于发射检测应答信号******************************/
uchar CheckACK()
{  
	sta=SPI_Read(STATUS);               // 返回状态寄存器
//	if(TX_DS||MAX_RT) 		//发送完毕中断
	if(IRQ==0)
	{
	   SPI_RW_Reg(WRITE_REG+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志
	   CSN=0;
	   SPI_RW(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
       CSN=1; 
	   return(0);
	}
	else
	   return(1);
}