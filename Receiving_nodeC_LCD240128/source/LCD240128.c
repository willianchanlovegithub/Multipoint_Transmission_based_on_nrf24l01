
#include "LCD240128.h"
#include "DotCode.h"
#include <intrins.h>

unsigned char XPOS;			//列方向LCD点阵位置指针
unsigned char YPOS;			//行方向LCD点阵位置指针
bit CharImageReverse;		//字符及图形的反显控制,0(正常显示),1(反显)

//-------------------------------------------------------------------------------
//子程序名称:void beautifulshow(void)
//功能:图形画面展示程序
//-------------------------------------------------------------------------------
void beautifulshow(void)
{
	//图片反显: 在(0,0)位置显示32*32点阵图标
    XPOS=0;
    YPOS=0;
	CharImageReverse=1;//字符及图形的反显控制,0(正常显示),1(反显)
    putimage(Img_temp1_32);	//写数据到当前LCDRAM地址中
	CharImageReverse=0;//字符及图形的反显控制,0(正常显示),1(反显)
	
	//图片反显: 在(208,0)位置显示32*32点阵图标
    XPOS=208;
    YPOS=0;
    CharImageReverse=1;//字符及图形的反显控制,0(正常显示),1(反显)
    putimage(Img_temp2_32);	//写数据到当前LCDRAM地址中
	CharImageReverse=0;//字符及图形的反显控制,0(正常显示),1(反显)
	
	//字符串显示: 在(40,8)位置显示"无线多点温度采集系统"字符串.
	XPOS=40;
	YPOS=16;
	putstr(ChineseCode1,ChineseCharDot1,"多点无线温度采集系统",10);

	//字符串反显: 在(40,8)位置反显"无线多点温度采集系统"字符串.
	XPOS=40;
	YPOS=16;
	CharImageReverse=1;//字符及图形的反显控制,0(正常显示),1(反显)
	putstr(ChineseCode1,ChineseCharDot1,"多点无线温度采集系统",10);
	CharImageReverse=0;//字符及图形的反显控制,0(正常显示),1(反显)
	
	//字符串显示: 在(40,0)位置显示"WELCOME TO MY SYSTEM"字符串.
	XPOS=40;
	YPOS=0;
	putstr(EnglishCode1,EnglishCharDot1,"WELCOME TO MY SYSTEM",10);

	//绘图: 以(0,35)为左上角-以(239,51)为右下角,画矩形.
	rect(0,35,239,51,1);
	rect(0,51,239,67,1);
	rect(0,67,239,83,1);
	rect(0,83,239,99,1);

	//绘图: 以(0,104)-(119,127)-(239,104)三点做顶点,画三角形.
	triangle(0  ,104,119,127,239,104,1);//等边三角形
	triangle(0  ,107,104,127,0  ,127,1);//直角三角形1
	triangle(239,107,134,127,239,127,1);//直角三角形2
}

//-------------------------------------------------------------------------------
//以下putchar为字符对象的基本子程序,putstr为字符对象的扩充子程序,getchinesecodepos,
//getenglishcodepos为字符对象的辅助子程序.
//-------------------------------------------------------------------------------
//子程序名称:void putchar(unsigned int c).
//功能:在(XPOS,YPOS)位置写单个字符点阵,若c>128 表示为中文字符,否则为西文字符
//-------------------------------------------------------------------------------
void putchar(unsigned int *C_E_Code_Name,unsigned char *C_E_CharDot_Name,unsigned int c,unsigned int CharNumber)            //定位写字符子程序
{   
	if(c>128)
        putsizeimage(CHINESECHARSIZE,CHINESECHARSIZE,getchinesecodepos(C_E_Code_Name,C_E_CharDot_Name,c,CharNumber));
    else
        putsizeimage(ENGLISHCHARSIZE,CHINESECHARSIZE,getenglishcodepos(C_E_Code_Name,C_E_CharDot_Name,c,CharNumber));
}

//-------------------------------------------------------------------------------
//子程序名称:void putstr(unsigned char code *s).
//功能:写字符串点阵,若*s=0 表示字符串结束.
//-------------------------------------------------------------------------------
void putstr(unsigned int *C_E_Code_Name,unsigned char *C_E_CharDot_Name,unsigned char code *s,unsigned int CharNumber)      //显示字符串子程序,字符码为0时退出
{   
	unsigned int c;
    while(1)
    {   
		c=*s;
        s++;
        if(c==0)
			break;
        if(c<128)
            putchar(C_E_Code_Name,C_E_CharDot_Name,c,CharNumber);			//英文字符串
        else
        {   
			putchar(C_E_Code_Name,C_E_CharDot_Name,c*256+*s,CharNumber);	//中文字符串
            s++;
        }
    }
}

//-------------------------------------------------------------------------------
//子程序名称:unsigned char code *getchinesecodepos(unsigned char ac).
//功能:根据当前中文字符码查表后计算得到当前中文字符码的字库点阵位置.
//-------------------------------------------------------------------------------
unsigned char code *getchinesecodepos(unsigned int *ChineseCode_Name,unsigned char *ChineseCharDot_Name,unsigned int ac,unsigned int CharNumber)
{   
	unsigned int min,max,mid,midc;
    min=0;
    max=CharNumber-1;
    while(1)
    {   
		if(max-min<2)
        {   
			if(ChineseCode_Name[min]==ac)
                mid=min;
            else if(ChineseCode_Name[max]==ac)
                mid=max;
            else
                mid=0;
            break;
        }
        mid=(max+min)/2;
        midc=ChineseCode_Name[mid];
        if(midc==ac)
            break;
        else if(midc>ac)
            max=mid-1;
        else
            min=mid+1;
    }
    return ChineseCharDot_Name+mid*CHINESECHARDOTSIZE;
}

//-------------------------------------------------------------------------------
//子程序名称:unsigned char code *getenglishcodepos(unsigned char ac).
//功能:根据当前ASCII字符码查表后计算得到当前ASCII字符码的字库点阵位置.
//-------------------------------------------------------------------------------
unsigned char code *getenglishcodepos(unsigned int *EnglishCode_Name,unsigned char *EnglishCharDot_Name,unsigned char ac,unsigned int CharNumber)
{   
	unsigned char min,max,mid,midc;
    min=0;
    max=CharNumber-1;
    while(1)
    {   
		if(max-min<2)
        {   
			if(EnglishCode_Name[min]==ac)
                mid=min;
            else if(EnglishCode_Name[max]==ac)
                mid=max;
            else
                mid=0;
            break;
        }
        mid=(max+min)/2;
        midc=EnglishCode_Name[mid];
        if(midc==ac)
            break;
        else if(midc>ac)
            max=mid-1;
        else
            min=mid+1;
    }
    return EnglishCharDot_Name+mid*ENGLISHCHARDOTSIZE;
}

//-------------------------------------------------------------------------------
//以下putsizeimage为图形对象的基本子程序,putimage为图形对象的扩充子程序
//-------------------------------------------------------------------------------
//子程序名称:void putsizeimage(unsigned char XSIZE,unsigned YSIZE,unsigned char code *s)
//功能:在(XPOS,YPOS)位置绘制XSIZE列及YISZE行点阵的图形*S.
//-------------------------------------------------------------------------------
void putsizeimage(unsigned char XSIZE,unsigned char YSIZE,unsigned char code *s)
{  
	unsigned char k,lx,ly,a1,a2,x,Lcd_Mask;
	k=XPOS&0x7;
	XSIZE+=k;
	x=XPOS;
	s--;
	for(ly=0;ly<YSIZE;ly++,YPOS++)
	{  
		for(XPOS=x,lx=k;lx<XSIZE;)
		{  
			unsigned char p;
			a1=*s;
			s++;
			a2=*s;
			if(CharImageReverse)
			{   
				a1=~a1;
				a2=~a2;
			}
			for(p=0;p<k;p++)
			{  
				a2>>=1;
				if((a1&0x1)==1)
					a2+=0x80;
				a1>>=1;
			}
			if((k==0) && (XSIZE<lx+8))
			{   
				lcdpos();
				lx+=8;
				XPOS+=8;
			}
			else
			{   
				lcdpos();
				a1=lcdrd();
				lcdpos();
				lx+=8;
				XPOS+=8;
				Lcd_Mask=0xff;
				p=XSIZE&0x7;
				while(p>0)
				{   
					Lcd_Mask>>=1;
					XPOS--;
					p--;
				}
				p=0xff;
				while(XSIZE<lx)
				{   
					lx--;
					XPOS--;
					p<<=1;
				}
				Lcd_Mask&=p;
				a2&=Lcd_Mask;
				a2|=a1&(~Lcd_Mask);
			}
			lcdwd(a2);
		}
      if((k!=0) && (XSIZE&0x7 != 0) && (k >= XSIZE&0x7)) s--;
	}
	if(XDOTS-x < XSIZE-k)
		XPOS=0;
	else
		YPOS-=YSIZE;
}

//-------------------------------------------------------------------------------
//子程序名称:void putimage(unsigned char code *s).
//功能:在(XPOS,YPOS)位置绘制XSIZE[*s]列及YISZE[*(s+1)]行点阵的图形[*(s+2)].
//-------------------------------------------------------------------------------
void putimage(unsigned char code *s)    //显示图形子程序
{   
	unsigned char XSIZE,YSIZE;
    XSIZE=*s;
    s++;
    YSIZE=*s;
    s++;
    putsizeimage(XSIZE,YSIZE,s);
}

//-------------------------------------------------------------------------------
//以下point为绘图操作的基本子程序,line,rect,triangle为绘图操作的扩充子程序.
//-------------------------------------------------------------------------------
//子程序名称:void point(bit b).
//功能:按b的数据在(XPOS,YPOS)位置绘制点.
//-------------------------------------------------------------------------------
void point(bit b)
{   
	unsigned char i,Lcd_Mask,j;
    Lcd_Mask=0x80;
    i=XPOS&0x7;
    while(i>0)
    {   
		Lcd_Mask>>=1;
        i--;
    }
    lcdpos();
    j=lcdrd();
    lcdpos();
    if(b)
        lcdwd(j|Lcd_Mask);
    else
        lcdwd(j&(~Lcd_Mask));
}

//-------------------------------------------------------------------------------
//子程序名称:void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//功能:按b的数据绘制(x0,y0)-(x1,y1)的直线
//-------------------------------------------------------------------------------
void line(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
{   
	unsigned char dx,dy;
	unsigned int dk;
	XPOS=x0;
	YPOS=y0;
	point(b);
	dx=(x1>=x0)?x1-x0:x0-x1;
	dy=(y1>=y0)?y1-y0:y0-y1;
	if(dx==dy)
	{
		while(XPOS!=x1)
		{
			if(x1>x0) XPOS++;else XPOS--;
            if(y1>y0) YPOS++;else YPOS--;
			point(b);
        }
    }
    else if (dx>dy)
    {   
		dk=dy;
        dy=0;
        while(XPOS!=x1)
        {   
			if(x1>x0) XPOS++;else XPOS--;
            dy++;
            if(y1>y0) YPOS=y0+(dk*dy+dx/2)/dx;else YPOS=y0-(dk*dy+dx/2)/dx;
            point(b);
        }
    }
    else
    {   
		dk=dx;
        dx=0;
        while(YPOS!=y1)
        {   
			if(y1>y0) YPOS++;else YPOS--;
            dx++;
            if(x1>x0) XPOS=x0+(dk*dx+dy/2)/dy;else XPOS=x0-(dk*dx+dy/2)/dy;
            point(b);
        }
    }
}

//-------------------------------------------------------------------------------
//子程序名称:void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
//功能:按b的数据绘制(x0,y0)-(x1,y1)的矩形
//-------------------------------------------------------------------------------
void rect(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,bit b)
{   
	line(x0,y0,x1,y0,b);
	line(x1,y0,x1,y1,b);
	line(x1,y1,x0,y1,b);
	line(x0,y1,x0,y0,b);
}

//-------------------------------------------------------------------------------
//子程序名称:void triangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit b)
//功能:按b的数据绘制(x0,y0)-(x1,y1)-(x2,y2)的三角形
//-------------------------------------------------------------------------------
void triangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,bit b)
{   
	line(x0,y0,x1,y1,b);
	line(x1,y1,x2,y2,b);
	line(x2,y2,x0,y0,b);
}

//-------------------------------------------------------------------------------
//以下lcdfill,lcdpos,lcdreset为T6963型硬件接口的240128液晶显示模块的基本子程序
//-------------------------------------------------------------------------------
//子程序名称:void lcdfill(unsigned char d).
//功能:整屏显示d表示的字节数据.
//-------------------------------------------------------------------------------
void lcdfill(unsigned char d)          	//整屏显示d代表的字节数据子程序
{   
	unsigned char i,j;
	lcdwc3(0x24,0x00,0x00);            	//设置内部RAM地址为0
	lcdwc(0xb0);						//进入自动写方式，一定要加上“自动写模式”的代码，才能正真的满屏显示！！！！！！！！！！
	for(i=0;i<128;i++)    				//128行
		for(j=0;j<30;j++)               //30个字节数据共240列
			lcdwd(d);                   //送图形数据
	lcdwc(0xb2);						//退出自动读写方式
}

//-------------------------------------------------------------------------------
//子程序名称:void lcdpos(void).
//功能:设置坐标点(XPOS,YPOS)位置对应的内部RAM地址.
//-------------------------------------------------------------------------------
void lcdpos(void)                       //设置坐标点(XPOS,YPOS)内部RAM地址的子程序
{  
	unsigned int CurrentAddress;
	//LCDRAM地址=30*YPOS+(XPOS/8)+3840(or+0x0f00)，因为初始化的时候设置图形显示区首地址为lcdwc3(0x42,0x00,0x0f)（先写第八位再写高八位）
	CurrentAddress=YPOS*30+XPOS/8+0x0f00;//加3840（3840是0x0f00的十进制表达）可以避免图形和文本同时显示时乱码！！！！！！！！！！
	lcdwc3(0x24,CurrentAddress&0xff,CurrentAddress/256);
}

//-------------------------------------------------------------------------------
//子程序名称:void lcdreset(void)
//功能:液晶显示控制器初始化
//-------------------------------------------------------------------------------
void lcdreset(void)					//液晶显示控制器初始化子程序
{   
	CEPIN=0;						//使能片选信号
	WRPIN=1;						//禁止写
	RDPIN=1;						//禁止读
	lcdwc3(0x24,0x00,0x00);			//配置文本显示首地址
	lcdwc3(0x40,0x00,0x00);			//设置文本显示区首地址
	lcdwc3(0x41,0x1e,0x00);			//设置文本显示区宽度
	lcdwc3(0x42,0x00,0x0f);			//设置图形显示区首地址，文本区地址和图形区地址要错开，文本的话128*30=3840=0x0f00，所以图形首地址用0x0f00！！！！！！！！！！
	lcdwc3(0x43,0x1e,0x00);			//设置图形显示区宽度为30字节共240点
	lcdwc3(0x22,0x02,0x00);			//CGRAM偏置地址设置
	lcdwc(0x80);                    //OR MODE
	lcdwc(0x9c);					//启用文本显示，启用图形显示
}

//-------------------------------------------------------------------------------
//以下lcdwd,lcdrd,lcdwc,lcdwc2,lcdwc3为T6963C型硬件接口[MCS51模拟口线接口方式]的
//    基本子程序;lcdwdata,lcdrdata,lcdwaitidle为内部子程序.
//-------------------------------------------------------------------------------
//子程序名称:void lcdwd(unsigned char d).
//功能:向液晶显示控制器写图形数据.
//-------------------------------------------------------------------------------
void lcdwd(unsigned char d)             //送图形数据子程序
{   
	lcdwc2(0xc0,d);
}

//-------------------------------------------------------------------------------
//子程序名称:unsigned char lcdrd(void).
//功能:从液晶显示控制器中读图形数据.
//-------------------------------------------------------------------------------
unsigned char lcdrd(void)               //读图形数据子程序
{   
	lcdwc(0xc1);                        //送读图形数据指令
	return lcdrdata();                  //读数据
}

//-------------------------------------------------------------------------------
//子程序名称:lcdwc(unsigned char c).
//功能:向液晶显示控制器送指令.
//-------------------------------------------------------------------------------
void lcdwc(unsigned char c)             //送指令子程序
{   
	lcdwaitidle();                      //检测液晶显示控制器是否空闲
	lcd_DATA=c;
	CDPIN=1;                            //CD=1 CE=0 WR=0
	CEPIN=0;
	WRPIN=0;
	_nop_();
	WRPIN=1;
	CEPIN=1;
}

//-------------------------------------------------------------------------------
//子程序名称:lcdwc2(unsigned char c,unsigned char d).
//功能:向液晶显示控制器送2字节指令.
//-------------------------------------------------------------------------------
void lcdwc2(unsigned char c,unsigned char d) //送2字节指令子程序
{   
	lcdwdata(d);                        //送指令参数
	lcdwc(c);                           //送指令码
}

//-------------------------------------------------------------------------------
//子程序名称:lcdwc3(unsigned char c,unsigned char d1,unsigned char d2).
//功能:向液晶显示控制器送3字节指令.
//-------------------------------------------------------------------------------
void lcdwc3(unsigned char c,unsigned char d1,unsigned char d2) //送3字节指令子程序
{   
	lcdwdata(d1);                        //送指令参数1
	lcdwdata(d2);                        //送指令参数2
	lcdwc(c);                            //送指令码
}

//-------------------------------------------------------------------------------
//子程序名称:unsigned char lcdrdata(void).
//功能:从液晶显示控制器中读数据.
//-------------------------------------------------------------------------------
unsigned char lcdrdata(void)            //读数据子程序
{   
	unsigned char d;
	lcdwaitidle();                      //检测液晶显示控制器是否空闲
	lcd_DATA=0xff;
	CDPIN=0;                            //CD=0 CE=0 RD=0
	CEPIN=0;
	RDPIN=0;
	_nop_();
	d=lcd_DATA;
	RDPIN=1;
	CEPIN=1;
	return d;
}

//-------------------------------------------------------------------------------
//子程序名称:void lcdwdata(unsigned char d).
//功能:向液晶显示控制器送数据.
//-------------------------------------------------------------------------------
void lcdwdata(unsigned char d)          //送数据子程序
{   
	lcdwaitidle();                      //检测液晶显示控制器是否空闲
	lcd_DATA=d;
	CDPIN=0;                            //CD=0 CE=0 WR=0
	CEPIN=0;
	WRPIN=0;
	_nop_();
	WRPIN=1;
	CEPIN=1;
}

//-------------------------------------------------------------------------------
//子程序名称:voidlcdwaitidle(void).
//功能:忙检测,在对液晶显示控制器操作的每一条指令之前,需检测液晶显示器是否空闲.
//-------------------------------------------------------------------------------
void lcdwaitidle(void)                  //忙检测子程序
{   
	unsigned char i,d;
	lcd_DATA=0xff;
	CDPIN=1;                            //CD=1 CE=0 RD=0
	CEPIN=0;
	for(i=0;i<20;i++)                   //
	{   RDPIN=0;
		_nop_();
		d=lcd_DATA;
		RDPIN=1;
		if( (d&0x3) == 0x3 ) break;     //D0D1=11 空闲退出
	}
	CEPIN=1;
}

//-------------------------------------------------------------------------------
//以下CharLocateXY,DirectDisplayChar,DirectDisplayString为直接显示字符串的基本子程序
//-------------------------------------------------------------------------------
//子程序名称:void CharLocateXY(uchar x,uchar y)
//功能:直接显示时设定显示地址函数
void CharLocateXY(uchar x,uchar y)
{
	uint  temp;
	temp= XDOTS/8*y + x;
	lcdwc3(0x24,temp&0xff,temp/256);
}

//-------------------------------------------------------------------------------
//子程序名称:void DirectDisplayChar(uchar x,uchar y,uchar CharByte)
//功能:直接显示一个ASCII码函数
void DirectDisplayChar(uchar x,uchar y,uchar CharByte)
{
	CharLocateXY(x,y);
	lcdwc2(0xC4,CharByte-0x20);	//内部带的英文字库是从0x00开始,也就是说空格0x20的字库值是0x00,所以-0x20
}

//-------------------------------------------------------------------------------
//子程序名称:void DirectDisplayString(uchar x,uchar y,uchar *str)
//功能:直接显示一个字符串
void DirectDisplayString(uchar x,uchar y,uchar *str)
{
	while(*str != 0)
	{
		if(x> (XDOTS/8-1))//自动换行
		{
			x=0;
			y++;
		}
		DirectDisplayChar(x,y,*str);
		++x;
		++str;
	}
}
