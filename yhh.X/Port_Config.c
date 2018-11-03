#include "Port_Config.h"

void Port_Init(void)
{
    ADCON1 = 0x06;
    PORTA = 0x00;
    TRISA = 0x08;
    PORTB = 0x00;
    TRISB = 0x00;
    PORTC = 0xFF;
    TRISC = 0xFF;
}
