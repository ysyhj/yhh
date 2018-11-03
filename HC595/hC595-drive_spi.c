
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


/*************	��������˵��	**************

��STC��MCU��SPI��ʽ����74HC595����8λ����ܡ�

�û������޸ĺ���ѡ��ʱ��Ƶ��, �����޸ļĴ���������STC12C5A60S2ϵ�� ���� STC12C5628AD  STC12C5410AD  STC12C4052ADϵ��.

�û���������ʾ�������޸ĳɹ�������.�Ƽ�����ʹ�ù��������.

��ʾЧ��Ϊ: 8�������ѭ����ʾ0,1,2...,A,B..F,����.

******************************************/

#include	"reg52.h"


/****************************** �û������ ***********************************/

#define MAIN_Fosc		11059200UL	//������ʱ��
//#define		MAIN_Fosc	22118400UL	//����ʱ��

/*****************************************************************************/

/*
sfr SPSTAT = 0xCD;	//STC12C5A60S2ϵ��
sfr SPCTL  = 0xCE;	//STC12C5A60S2ϵ��
sfr SPDAT  = 0xCF;	//STC12C5A60S2ϵ��
*/
sfr SPSTAT  = 0x84;	//STC12C5628AD  STC12C5410AD  STC12C4052ADϵ��
sfr SPCTL   = 0x85;	//STC12C5628AD  STC12C5410AD  STC12C4052ADϵ��
sfr SPDAT   = 0x86;	//STC12C5628AD  STC12C5410AD  STC12C4052ADϵ��


/******************** ����ĺ��Զ�����, �û������޸� **************************/

#define	Timer0_Reload	(MAIN_Fosc / 12000)

/*****************************************************************************/

//SPCTL		SPI���ƼĴ���
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: ����SS�ţ���MSTRλ�����������Ǵӻ�		0: SS�����ھ������ӻ���
#define	SPEN		1	//1: ����SPI��								0����ֹSPI������SPI�ܽž�Ϊ��ͨIO
#define	DORD		0	//1��LSB�ȷ���								0��MSB�ȷ�
#define	MSTR		1	//1����Ϊ����								0����Ϊ�ӻ�
#define	CPOL		1	//1: ����ʱSCLKΪ�ߵ�ƽ��					0������ʱSCLKΪ�͵�ƽ
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//SPSTAT	SPI״̬�Ĵ���
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI������ɱ�־��д��1��0��
#define	WCOL	0x40		//SPIд��ͻ��־��д��1��0��


/*************	���س�������	**************/
unsigned char code t_display[]={
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   ����
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};	//����

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};		//λ��



/*************	���ر�������	**************/

sbit	SPI_SCL      = P1^7;	//SPIͬ��ʱ��		P_HC595_SRCLK	pin 11	SRCLK	Shift data clock
//sbit	SPI_MISO     = P1^6;	//SPIͬ����������	��������
sbit	SPI_MOSI     = P1^5;	//SPIͬ���������	P_HC595_SER		pin 14	SER		data input
sbit	P_HC595_RCLK = P1^4;	//SPIƬѡ(����IO)	pin 12	RCLk	store (latch) clock


unsigned char 	LED8[8];		//��ʾ����
unsigned char	display_index;	//��ʾλ����
bit				B_1ms;			//1ms��־


/**********************************************/
void main(void)		//������
{
	unsigned char	i,k;
	unsigned int	j;
	
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_4;	//����SPI

	TMOD = 0x01;	//Timer 0 config as 16bit timer, 12T
	TH0  = (65536 - Timer0_Reload) / 256;
	TL0  = (65536 - Timer0_Reload) % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;

	for(i=0; i<8; i++)	LED8[i] = 0x10;	//�ϵ�����
	j = 0;
	k = 0;
	
	while(1)
	{
		while(!B_1ms)	;	//�ȴ�1ms��
		B_1ms = 0;
		if(++j >= 500)		//500ms��
		{
			j = 0;
			for(i=0; i<8; i++)	LED8[i] = k;	//ˢ����ʾ
			if(++k > 0x10)	k = 0;				//8�������ѭ����ʾ0,1,2...,A,B..F,����.
		}
	}
} 
/**********************************************/



/**********************************************/
void SPI_SendByte(unsigned char dat)	//SPI����һ���ֽ�
{		
	SPSTAT = SPIF + WCOL;			//��0 SPIF��WCOL��־
	SPDAT = dat;					//����һ���ֽ�
	while((SPSTAT & SPIF) == 0)	;	//�ȴ��������
	SPSTAT = SPIF + WCOL;			//��0 SPIF��WCOL��־
}

/**********************************************/
void DisplayScan(void)	//��ʾɨ�躯��
{	
//	SPI_SendByte(~T_COM[display_index]);			//����	���λ��
//	SPI_SendByte(t_display[LED8[display_index]]);	//����	�������
	SPI_SendByte(T_COM[display_index]);				//����	���λ��
	SPI_SendByte(~t_display[LED8[display_index]]);	//����	�������
	P_HC595_RCLK = 1;
	P_HC595_RCLK = 0;							//�����������
	if(++display_index >= 8)	display_index = 0;	//8λ������0
}


/**********************************************/
void timer0 (void) interrupt 1	//Timer0 1ms�жϺ���
{
	TH0  = (65536 - Timer0_Reload) / 256;	//��װ��ʱֵ
	TL0  = (65536 - Timer0_Reload) % 256;

	DisplayScan();	//1msɨ����ʾһλ
	B_1ms = 1;		//1ms��־
}


