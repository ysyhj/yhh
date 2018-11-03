
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

��STC��MCU��IO��ʽ����74HC595����8λ����ܡ�

�û������޸ĺ���ѡ��ʱ��Ƶ��.

�û���������ʾ�������޸ĳɹ�������.�Ƽ�����ʹ�ù��������.

��ʾЧ��Ϊ: 8�������ѭ����ʾ0,1,2...,A,B..F,����.

******************************************/

#include	"reg52.h"


/****************************** �û������ ***********************************/

#define MAIN_Fosc		11059200UL	//������ʱ��
//#define		MAIN_Fosc	22118400UL	//����ʱ��

/*****************************************************************************/


/******************** ����ĺ��Զ�����, �û������޸� **************************/

#define	Timer0_Reload	(MAIN_Fosc / 12000)

/*****************************************************************************/

/*************	���س�������	**************/
unsigned char code t_display[]={
//	 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   ����
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};	//����

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};		//λ��



/*************	���ر�������	**************/
//sbit	P_HC595_SER   = P3^2;	//pin 14	SER		data input
//sbit	P_HC595_RCLK  = P3^4;	//pin 12	RCLk	store (latch) clock
//sbit	P_HC595_SRCLK = P3^3;	//pin 11	SRCLK	Shift data clock

sbit	P_HC595_SER   = P1^5;	//pin 14	SER		data input
sbit	P_HC595_RCLK  = P1^4;	//pin 12	RCLk	store (latch) clock
sbit	P_HC595_SRCLK = P1^7;	//pin 11	SRCLK	Shift data clock

unsigned char 	LED8[8];		//��ʾ����
unsigned char	display_index;	//��ʾλ����
bit				B_1ms;			//1ms��־


/**********************************************/
void main(void)
{
	unsigned char	i,k;
	unsigned int	j;
	
	TMOD = 0x01;	//Timer 0 config as 16bit timer, 12T
	TH0  = (65536 - Timer0_Reload) / 256;
	TL0  = (65536 - Timer0_Reload) % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;

	for(i=0; i<8; i++)	LED8[i] = 0x10;	//�ϵ�����
	j = 0;
	k = 0;
//	for(i=0; i<8; i++)	LED8[i] = i;
	
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
void Send_595(unsigned char dat)	//����һ���ֽ�
{		
	unsigned char	i;
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)	P_HC595_SER = 1;
		else			P_HC595_SER = 0;
		P_HC595_SRCLK = 1;
		P_HC595_SRCLK = 0;
		dat = dat << 1;
	}
}

/**********************************************/
void DisplayScan(void)	//��ʾɨ�躯��
{	
//	Send_595(~T_COM[display_index]);			//����	���λ��
//	Send_595(t_display[LED8[display_index]]);	//����	�������
	Send_595(T_COM[display_index]);				//����	���λ��
	Send_595(~t_display[LED8[display_index]]);	//����	�������
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


