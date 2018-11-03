
#include "Hc595_io.h"
#include "Port_Config.h"

const unsigned char t_display[]={
//0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   ����
0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00};  //����

const unsigned char T_COM[]={0x02,0x04,0x08,0x10,0x20,0x40,0x80};  //λ��

static unsigned char	display_index;	//��ʾλ����

void Send_595(unsigned char dat)	//����һ���ֽ�
{
    unsigned char i;
    for(i=0; i<8; i++)
    {
        if(dat & 0x80)
        {
            DI_595 = 1;
        }
        else
        {
            DI_595 = 0;
        }
        
        SCK_595 = 1;
        dat <<= 1;
        SCK_595 = 0;
    }
}

void DisplayScan(void)	//��ʾɨ�躯��
{
    Send_595(T_COM[display_index]);                //�������λ��
    Send_595(t_display[LED[display_index]]);	//�����������
//	Send_595(~T_COM[display_index]);                 //�������λ��
//	Send_595(~t_display[LED[display_index]]);       //�����������
    RCK_595 = 1;                                  //�����������
    if(++display_index >= LED_COM)
    {
        display_index = 0;	//������0
    }
    RCK_595 = 0;  
}
