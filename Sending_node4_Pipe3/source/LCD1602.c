
#include <intrins.h>
#include "LCD1602.h"

unsigned char datas[6] = {0}; //µÚÒ»Î»´æ·ÅÕı¸º·ûºÅ£¬µÚ¶şÎ»µ½µÚÁùÎ»´æ·ÅÎÂ¶ÈÖµ

//ÑÓÊ±³ÌĞò½öÊÊÓÃÓÚSTC89xxx
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

/*uchar LCD1602_Wait(void)		   //¶Á1602ÄÚ²¿×´Ì¬×Ö £¬²é¿´ÊÇ·ñ×¼±¸ºÃ
{
	uchar Sta;
	LcdRs=0;				   //LcdRS=L,LcdRw=H,LcdEn=H Êä³öD0~D7×´Ì¬×Ö
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
	//while(LCD1602_Wait());			//ÈôÃ¦Âµ£¬ÔòµÈ´ı,ÃâµÃÏÔÊ¾ÂÒÂë
	LcdEn = 0;											
	LcdRs = style;					        //ÉèÖÃÊÇÊı¾İ'1',»¹ÊÇÃüÁî'0'
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
	LCD1602_Write(0,0x38);               //8Î»Êı¾İ¶Ë¿Ú,2ĞĞÏÔÊ¾,5*7µãÕó
	LCD1602_Write(0,0x0c);  //¿ªÆôÏÔÊ¾, ÎŞ¹â±ê
	LCD1602_Write(0,0x06);       //ACµİÔö, »­Ãæ²»¶¯
	LCD1602_Write(0,0x01);   //ÇåÆÁ
	Delay10ms();
}

void LCD1602_GotoXY(uchar x, uchar y)	   //XÎªÏÔÊ¾Ö¸ÕëµÄÎ»ÖÃ£¬¼´Îª¸÷ĞĞµÄµÚ¼¸¸öÎ»ÖÃ£¬YÑ¡ĞĞ
{
	if(x==0)
		LCD1602_Write(0,0x80+y);
	if(x==1)
		LCD1602_Write(0,(0x80+0x40+y));
}

void LCD1602_Display_NoXY(uchar *str)					 //ÏòLCDĞ´Èë×Ö·û´®
{
	while(*str!='\0')
	{
		LCD1602_Write(1,*str);
		str++;
	}
}


//1602ÏÔÊ¾ÎÂ¶ÈÖµ
void LcdDisplayTemp(int temp) 	 //lcdÏÔÊ¾
{
	
    
  //unsigned char datas[5] = {0, 0, 0, 0, 0}; //¶¨ÒåÊı×é£¬´æ·ÅÎÂ¶ÈÖµ
	float tp;  
	if(temp< 0)				//µ±ÎÂ¶ÈÖµÎª¸ºÊı
  	{
	  	LCD1602_GotoXY(1,5);
	    LCD1602_Write(1,'-');  		//ÏÔÊ¾¸º
			datas[0]=0x2d;//°Ñ¡°-¡±µÄASCIIÂë´æ·ÅÔÚdatas[]Êı×éµÚÒ»Î»
			
		//ÒòÎª¶ÁÈ¡µÄÎÂ¶ÈÊÇÊµ¼ÊÎÂ¶ÈµÄ²¹Âë£¬ËùÒÔ¼õ1£¬ÔÙÈ¡·´Çó³öÔ­Âë
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//ËãÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
 
  	}
 	else
  	{			
	  	LCD1602_GotoXY(1,05);
	    LCD1602_Write(1,'+');  		//ÏÔÊ¾¸º
		datas[0]=0x2b;//°Ñ¡°+¡±µÄASCIIÂë´æ·ÅÔÚdatas[]Êı×éµÚÒ»Î»
			
		tp=temp;//ÒòÎªÊı¾İ´¦ÀíÓĞĞ¡ÊıµãËùÒÔ½«ÎÂ¶È¸³¸øÒ»¸ö¸¡µãĞÍ±äÁ¿
		//Èç¹ûÎÂ¶ÈÊÇÕıµÄÄÇÃ´£¬ÄÇÃ´ÕıÊıµÄÔ­Âë¾ÍÊÇ²¹ÂëËü±¾Éí
		temp=tp*0.0625*100+0.5;	
		//ÁôÁ½¸öĞ¡Êıµã¾Í*100£¬+0.5ÊÇËÄÉáÎåÈë£¬ÒòÎªCÓïÑÔ¸¡µãÊı×ª»»ÎªÕûĞÍµÄÊ±ºò°ÑĞ¡Êıµã
		//ºóÃæµÄÊı×Ô¶¯È¥µô£¬²»¹ÜÊÇ·ñ´óÓÚ0.5£¬¶ø+0.5Ö®ºó´óÓÚ0.5µÄ¾ÍÊÇ½ø1ÁË£¬Ğ¡ÓÚ0.5µÄ¾Í
		//Ëã¼ÓÉÏ0.5£¬»¹ÊÇÔÚĞ¡ÊıµãºóÃæ¡£
	}
	datas[1] = temp / 10000;
	datas[2] = temp % 10000 / 1000;
	datas[3] = temp % 1000 / 100;
	datas[4] = temp % 100 / 10;
	datas[5] = temp % 10;
	
	LCD1602_GotoXY(1,6);
	LCD1602_Write(1,'0'+datas[1]); //°ÙÎ» 	
	LCD1602_GotoXY(1,7);
	LCD1602_Write(1,'0'+datas[2]); //Ê®Î»
	LCD1602_GotoXY(1,8);
	LCD1602_Write(1,'0'+datas[3]); //¸öÎ» 
	LCD1602_GotoXY(1,9);
	LCD1602_Write(1,'.'); 		   //ÏÔÊ¾ ¡®.¡¯
	LCD1602_GotoXY(1,10);
	LCD1602_Write(1,'0'+datas[4]); //ÏÔÊ¾Ğ¡ÊıµãºóÒ»Î»
	LCD1602_GotoXY(1,11);
	LCD1602_Write(1,'0'+datas[5]); //ÏÔÊ¾Ğ¡ÊıµãºóÁ½Î»
	LCD1602_GotoXY(1,12);
	LCD1602_Write(1,0x43); //ÏÔÊ¾¡°C¡±
}
