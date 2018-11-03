
#include "Port_Config.h"

void Port_Init(void)
{

    ADCON1 = 0x8E;		//设置PORTA0为模拟输入，其他为IO口
    PORTA = 0xFF;
	TRISA = 0x38; 		//RA5，RA4，RA3为输入，RA2,RA1为输出
    RB5 = 1;
	TRISB5 = 0;	
    
    PORTB = 0xFF;
    TRISB &= 0xDD;

    RC0 = 1;
    RC1 = 1;
    RC2 = 1;
    RC5 = 1;
    TRISC &= 0xD8;

    PORTD = 0xFF;
    TRISD = 0x00;

}
