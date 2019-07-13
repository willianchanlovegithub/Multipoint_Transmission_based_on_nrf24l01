
#ifndef __LCD240128_H__
#define __LCD240128_H__

#include<reg52.h>
//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//-------------------------------------------------------------------------------
//PIN1: FG   [构造地]-------------------VSS
//PIN2: VSS  [电源地]-------------------VSS
//PIN3: VDD  [电源正极]-----------------VDD(+5V)
//PIN4: Vo   [LCD 偏压输入]-------------接10K的可调电阻2K电阻到VSS及VEE
//                                      Vo=VEE时为显示最深
//PIN5: WR   [写信号 低有效]------------P1.3
//PIN6: RD   [读信号 低有效]------------P1.2
//PIN7: CE   [使能信号 低有效]----------P1.1
//PIN8: CD   [数据/命令选择端（H/L）]---P1.0
//PIN9: NC   [空脚]---------------------不连线
//PIN10:RST  [复位端 低有效]------------VDD
//PIN11:D0   [Data I/O]-----------------P0.0
//PIN12:D1   [Data I/O]-----------------P0.1
//PIN13:D2   [Data I/O]-----------------P0.2
//PIN14:D3   [Data I/O]-----------------P0.3
//PIN15:D4   [Data I/O]-----------------P0.4
//PIN16:D5   [Data I/O]-----------------P0.5
//PIN17:D6   [Data I/O]-----------------P0.6
//PIN18:D7   [Data I/O]-----------------P0.7
//PIN19:FS   [字体点阵]-----------------VSS
//PIN20:VEE  [LCD负压输出]--------------提供液晶负电压.
//PIN21:BLA  [背光源正极]---------------见使用说明书.
//PIN22:BLK  [背光源负极]---------------VSS
//注:89C52的晶振频率为11.0592MHz.

//-------------------------------------------------------------------------------
//以下为产品接口引脚在演示程序中的预定义
//用户在编写应用程序时,需按自己的实际硬件连线来重新定义
sbit CDPIN=P1^0;                        // C/D对应单片机引脚
sbit CEPIN=P1^1;                        // CE对应单片机引脚
sbit WRPIN=P1^3;                        // WR对应单片机引脚
sbit RDPIN=P1^2;                        // RD对应单片机引脚

//-------------------------------------------------------------------------------
//全局变量
extern unsigned char XPOS;			//列方向LCD点阵位置指针
extern unsigned char YPOS;			//行方向LCD点阵位置指针
extern bit CharImageReverse;		//字符及图形的反显控制,0(正常显示),1(反显)


//-------------------------------------------------------------------------------
//以下为精简版中英文字库
//实际使用时请包含由Sunman精简版中英文字符库代码生成器.exe自动生成的charlib.c文
//文件来替换该部分
//精简版中英文字符库常量定义
//#define ENGLISHCHARNUMBER  10       //精简版英文字符库中的英文字符的个数
//#define CHINESECHARNUMBER  10      	//精简版中文字符库中的中文字符的个数
#define ENGLISHCHARSIZE    8       	//英文字符X方向显示点的个数
#define CHINESECHARSIZE    16      	//中文字符X方向及中英文字符Y方向显示点的个数
#define ENGLISHCHARDOTSIZE 16      	//单个英文字符点阵的字节数
#define CHINESECHARDOTSIZE 32      	//单个中文字符点阵的字节数
#define XDOTS   240               	//图形空间X方向大小
#define YDOTS   128     			//图形空间X方向大小
#define lcd_DATA P0					//数据端口

//-------------------------------------------------------------------------------
//全局变量声明
extern unsigned int code EnglishCode1[];
extern unsigned int code ChineseCode1[];
extern unsigned int code ChineseCode2[];
extern unsigned char code EnglishCharDot1[];
extern unsigned char code ChineseCharDot1[];
extern unsigned char code ChineseCharDot2[];
extern unsigned char code Img_temp1_32[];
extern unsigned char code Img_temp2_32[];
extern unsigned char code Img_check_ok_32[];
extern unsigned char code Img_check_no_ok_32[];

//-------------------------------------------------------------------------------
//以下为演示程序的函数定义
extern void beautifulshow(void);								//图形画面展示程序
extern void putchar(unsigned int *C_E_Code_Name,unsigned char *C_E_CharDot_Name,unsigned int c,unsigned int CharNumber);		
																//定位写字符子程序
extern void putstr(unsigned int *C_E_Code_Name,unsigned char *C_E_CharDot_Name,unsigned char code *s,unsigned int CharNumber);	
																//显示字符串子程序,字符码为0时退出
extern unsigned char code *getchinesecodepos(unsigned int *ChineseCode_Name,unsigned char *ChineseCharDot_Name,unsigned int ac,unsigned int CharNumber);
extern unsigned char code *getenglishcodepos(unsigned int *EnglishCode_Name,unsigned char *EnglishCharDot_Name,unsigned char ac,unsigned int CharNumber);
extern void putimage(unsigned char code *s);   					//显示图形子程序
extern void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s);
																//显示指定大小的图形子程序
extern void point(bit b);										//按b的数据在(XPOS,YPOS)位置绘制点
extern void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
extern void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b);
extern void triangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit b);
extern void lcdfill(unsigned char d);          					//整屏显示d代表的字节数据子程序
extern void lcdpos(void);                      					//设置坐标点(XPOS,YPOS)内部RAM地址的子程序
extern void lcdreset(void);                    					//液晶显示控制器初始化子程序
extern void lcdwd(unsigned char d);            					//送图形数据子程序
extern unsigned char lcdrd(void);              					//读图形数据子程序
extern void lcdwc(unsigned char c);            					//送指令子程序
extern void lcdwc2(unsigned char c,unsigned char d);			//送2字节指令子程序
extern void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2);
																//送3字节指令子程序
extern unsigned char lcdrdata(void);           					//读数据子程序
extern void lcdwdata(unsigned char d);         					//送数据子程序
extern void lcdwaitidle(void);                 					//忙检测子程序
extern void CharLocateXY(uchar x,uchar y);						//直接显示时设定显示地址函数
extern void DirectDisplayChar(uchar x,uchar y,uchar CharByte);	//直接显示一个ASCII码函数
extern void DirectDisplayString(uchar x,uchar y,uchar *str);	//直接显示一个字符串

#endif