
#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//***************************NRF24L01函数声明******************************//
extern void Delay10us();
extern void Delay100us();
extern unsigned char SPI_RW(unsigned char byte);
extern unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);
extern unsigned char SPI_Read(unsigned char reg);
extern unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
extern unsigned char SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bytes);
extern uchar NRF24L01_Check(void);
extern uchar NRFRevDate(uchar *RevDate);
extern void init_NRF24L01(void);
extern void RX_Mode(void);
extern uchar Rx_Pipe_Num_Choose(void);

//**************************nRF24L01 IO端口定义******************************//
sbit MISO =P2^3;	//主机读从设备（主入从出）
sbit MOSI =P2^1;	//主机写从设备（主出从入）
sbit SCK  =P2^4;	//时钟信号
sbit CE   =P2^7;	
sbit CSN  =P2^0;	//片选信号（控制芯片是否被选中）
sbit IRQ  =P2^2;	

//***************************NRF24L01数据设置******************************//
#define TX_ADR_WIDTH 5 		// 发送地址宽度
#define RX_ADR_WIDTH 5 		// 接收地址宽度
#define TX_PLOAD_WIDTH 32   // 发送数据宽度
#define RX_PLOAD_WIDTH 32   // 接收数据宽度

//***************************NRF24L01全局变量******************************//
extern uchar const RX_ADDRESS_P0[TX_ADR_WIDTH]; //数据通道0接收地址
extern uchar const RX_ADDRESS_P1[TX_ADR_WIDTH]; //数据通道1接收地址
extern uchar const RX_ADDRESS_P2[1]; //数据通道2接收地址，由于只是低八位和通道0地址不一样，只写低八位即可
extern uchar const RX_ADDRESS_P3[1]; //数据通道3接收地址，由于只是低八位和通道0地址不一样，只写低八位即可
extern uchar const RX_ADDRESS_P4[1]; //数据通道4接收地址，由于只是低八位和通道0地址不一样，只写低八位即可
extern uchar const RX_ADDRESS_P5[1]; //数据通道5接收地址，由于只是低八位和通道0地址不一样，只写低八位即可
extern uchar RxBuf_P0[RX_PLOAD_WIDTH];//接收缓冲区
extern uchar RxBuf_P1[RX_PLOAD_WIDTH];//通道1接收缓冲区
extern uchar RxBuf_P2[RX_PLOAD_WIDTH];//通道2接收缓冲区
extern uchar RxBuf_P3[RX_PLOAD_WIDTH];//通道3接收缓冲区
extern uchar RxBuf_P4[RX_PLOAD_WIDTH];//通道4接收缓冲区
extern uchar RxBuf_P5[RX_PLOAD_WIDTH];//通道5接收缓冲区

//===================================================================================================================================
//===================================================================================================================================
//===================================================================================================================================


/*******************命令寄存器***************************/
#define READ_REG 0x00 		// Define read command to register
#define WRITE_REG 0x20 		// Define write command to register
#define RD_RX_PLOAD 0x61 	// Define RX payload register address
#define WR_TX_PLOAD 0xA0 	// Define TX payload register address
#define FLUSH_TX 0xE1 		// Define flush TX register command
#define FLUSH_RX 0xE2 		// Define flush RX register command
#define REUSE_TX_PL 0xE3 	// Define reuse TX payload register command
#define NOP 0xFF 			// Define No Operation, might be used to read status register
/******************寄存器地址****************************/
#define CONFIG 0x00 		// 'Config' register addres
#define EN_AA 0x01 			// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02 		// 'Enabled RX addresses' register address
#define SETUP_AW 0x03 		// 'Setup address width' register address
#define SETUP_RETR 0x04 	// 'Setup Auto. Retrans' register address
#define RF_CH 0x05 			// 'RF channel' register address
#define RF_SETUP 0x06 		// 'RF setup' register address
#define STATUS 0x07 		// 'Status' register address
#define OBSERVE_TX 0x08 	// 'Observe TX' register address
#define CD 0x09 			// 'Carrier Detect' register address
#define RX_ADDR_P0 0x0A 	// 'RX address pipe0' register address
#define RX_ADDR_P1 0x0B 	// 'RX address pipe1' register address
#define RX_ADDR_P2 0x0C 	// 'RX address pipe2' register address
#define RX_ADDR_P3 0x0D 	// 'RX address pipe3' register address
#define RX_ADDR_P4 0x0E 	// 'RX address pipe4' register address
#define RX_ADDR_P5 0x0F 	// 'RX address pipe5' register address
#define TX_ADDR 0x10 		// 'TX address' register address
#define RX_PW_P0 0x11 		// 'RX payload width, pipe0' register address
#define RX_PW_P1 0x12 		// 'RX payload width, pipe1' register address
#define RX_PW_P2 0x13 		// 'RX payload width, pipe2' register address
#define RX_PW_P3 0x14 		// 'RX payload width, pipe3' register address
#define RX_PW_P4 0x15 		// 'RX payload width, pipe4' register address
#define RX_PW_P5 0x16 		// 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17 	// 'FIFO Status Register' register address

#endif
