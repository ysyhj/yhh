
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      本程序经过测试完全正常, 不提供电话技术支持, 如不能理解, 请自行补充相关基础.  */


/*************	本程序功能说明	**************

用STC的MCU的SPI方式控制74HC595驱动8位数码管。

用户可以修改宏来选择时钟频率, 可以修改寄存器定义是STC12C5A60S2系列 还是 STC12C5628AD  STC12C5410AD  STC12C4052AD系列.

用户可以在显示函数里修改成共阴或共阳.推荐尽量使用共阴数码管.

显示效果为: 8个数码管循环显示0,1,2...,A,B..F,消隐.

******************************************/

#include	"reg52.h"


/****************************** 用户定义宏 ***********************************/

#define MAIN_Fosc		11059200UL	//定义主时钟
//#define		MAIN_Fosc	22118400UL	//定义时钟

/*****************************************************************************/

/*
sfr SPSTAT = 0xCD;	//STC12C5A60S2系列
sfr SPCTL  = 0xCE;	//STC12C5A60S2系列
sfr SPDAT  = 0xCF;	//STC12C5A60S2系列
*/
sfr SPSTAT  = 0x84;	//STC12C5628AD  STC12C5410AD  STC12C4052AD系列
sfr SPCTL   = 0x85;	//STC12C5628AD  STC12C5410AD  STC12C4052AD系列
sfr SPDAT   = 0x86;	//STC12C5628AD  STC12C5410AD  STC12C4052AD系列


/******************** 下面的宏自动生成, 用户不可修改 **************************/

#define	Timer0_Reload	(MAIN_Fosc / 12000)

/*****************************************************************************/

//SPCTL		SPI控制寄存器
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: 忽略SS脚，由MSTR位决定主机还是从机		0: SS脚用于决定主从机。
#define	SPEN		1	//1: 允许SPI，								0：禁止SPI，所有SPI管脚均为普通IO
#define	DORD		0	//1：LSB先发，								0：MSB先发
#define	MSTR		1	//1：设为主机								0：设为从机
#define	CPOL		1	//1: 空闲时SCLK为高电平，					0：空闲时SCLK为低电平
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//SPSTAT	SPI状态寄存器
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI传输完成标志。写入1清0。
#define	WCOL	0x40		//SPI写冲突标志。写入1清0。


/*************	本地常量声明	**************/
unsigned char code t_display[]={
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   消隐
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};	//段码

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};		//位码



/*************	本地变量声明	**************/

sbit	SPI_SCL      = P1^7;	//SPI同步时钟		P_HC595_SRCLK	pin 11	SRCLK	Shift data clock
//sbit	SPI_MISO     = P1^6;	//SPI同步数据输入	本例不用
sbit	SPI_MOSI     = P1^5;	//SPI同步数据输出	P_HC595_SER		pin 14	SER		data input
sbit	P_HC595_RCLK = P1^4;	//SPI片选(任意IO)	pin 12	RCLk	store (latch) clock


unsigned char 	LED8[8];		//显示缓冲
unsigned char	display_index;	//显示位索引
bit				B_1ms;			//1ms标志


/**********************************************/
void main(void)		//主函数
{
	unsigned char	i,k;
	unsigned int	j;
	
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_4;	//配置SPI

	TMOD = 0x01;	//Timer 0 config as 16bit timer, 12T
	TH0  = (65536 - Timer0_Reload) / 256;
	TL0  = (65536 - Timer0_Reload) % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;

	for(i=0; i<8; i++)	LED8[i] = 0x10;	//上电消隐
	j = 0;
	k = 0;
	
	while(1)
	{
		while(!B_1ms)	;	//等待1ms到
		B_1ms = 0;
		if(++j >= 500)		//500ms到
		{
			j = 0;
			for(i=0; i<8; i++)	LED8[i] = k;	//刷新显示
			if(++k > 0x10)	k = 0;				//8个数码管循环显示0,1,2...,A,B..F,消隐.
		}
	}
} 
/**********************************************/



/**********************************************/
void SPI_SendByte(unsigned char dat)	//SPI发送一个字节
{		
	SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
	SPDAT = dat;					//发送一个字节
	while((SPSTAT & SPIF) == 0)	;	//等待发送完成
	SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
}

/**********************************************/
void DisplayScan(void)	//显示扫描函数
{	
//	SPI_SendByte(~T_COM[display_index]);			//共阴	输出位码
//	SPI_SendByte(t_display[LED8[display_index]]);	//共阴	输出段码
	SPI_SendByte(T_COM[display_index]);				//共阳	输出位码
	SPI_SendByte(~t_display[LED8[display_index]]);	//共阳	输出段码
	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//锁存输出数据
	if(++display_index >= 8)	display_index = 0;	//8位结束回0
}


/**********************************************/
void timer0 (void) interrupt 1	//Timer0 1ms中断函数
{
	TH0  = (65536 - Timer0_Reload) / 256;	//重装定时值
	TL0  = (65536 - Timer0_Reload) % 256;

	DisplayScan();	//1ms扫描显示一位
	B_1ms = 1;		//1ms标志
}


