
#ifndef _PORT__CONFIG_H_
#define _PORT__CONFIG_H_

#include "Main.h"


// PIC16F877A Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



//数码管字段
#define LED_DATA_PORT PORTD

//数码管的引脚
#define  LED_DIGIT_1  RC0
#define  LED_DIGIT_2  RC1
#define  LED_DIGIT_3  RC2
#define  LED_DIGIT_4  RC5

//蜂鸣器输出
#define  BEEP  RB1

#define Column1  RB5
#define Column2  RA1
#define Column3  RA2

#define Row1  RA5
#define Row2  RA4
#define Row3  RA3


void Port_Init(void);

#endif