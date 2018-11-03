#ifndef __PORT_CONFIG_H__
#define __PORT_CONFIG_H__

#include "Main.h"

// PIC16F873A Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//74HC595的OE引脚直接硬件接地
#define DI_595     RA2
#define RCK_595    RA1
#define SCK_595    RA0

//按键
// #define STOP          RC0 //手动停止
// #define START_UP      RC1 //手动启动
// #define MOTOR_START   RC2 //主轴启动
// #define MOTOR_STOP    RC3 //主轴停止
// #define MANUAL_AUTO   RC4 //手动/自动
// #define UP            RC5 //上升
// #define DOWN          RC6 //下降
// #define URGENT_STOP   RC7 //急停
// #define OK            RA3 //确认

#define KEY_VAL       PORTC

#define STOP_VAL          0x01 //手动停止
#define START_UP_VAL      0x02 //手动启动
#define MOTOR_START_VAL   0x04 //主轴启动
#define MOTOR_STOP_VAL    0x08 //主轴停止
#define MANUAL_AUTO_VAL   0x10 //手动/自动
#define UP_VAL            0x20 //上升
#define DOWN_VAL          0x40 //下降
#define URGENT_STOP_VAL   0x80 //急停
#define OK_VAL            0x08 //确认

//输出
#define UP_DOWN       RB0 //上升/下降
#define MOTOR_ST_SP   RB1 //主轴启动/停止
#define HEAD         RB2 //刹车

//蜂鸣器输出
#define  BEEP  RB6


void Port_Init(void);

#endif