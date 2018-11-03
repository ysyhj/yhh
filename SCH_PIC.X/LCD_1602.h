
#ifndef _LCD__1602_H_
#define _LCD__1602_H_

#include "Main.h"
#include "Port_Config.h"
#include "Delay_T1.h"

void LCD_Init(const bit);

void LCD_Update(void);

//控制光标
void LCD_Control_Cursor(const bit, const bit, const uchar8);
//自定义CGRAM
void LCD_Create_Character(const uchar8, const uchar8* const);


// 2 x 16 显示模式
#define LCD_LINES 2
#define LCD_CHARACTERS 16

// 用户自定义字符
#define LCD_UDC_POUNDS 1
#define LCD_UDC_DEGREES_C 2 

char LCD_data_G[LCD_LINES][LCD_CHARACTERS+1];//显示数据的数组,外部调用

#endif