
#ifndef _LCD__1602_H_
#define _LCD__1602_H_

#include "Main.h"
#include "Port_Config.h"
#include "Delay_T1.h"

void LCD_Init(const bit);

void LCD_Update(void);

//���ƹ��
void LCD_Control_Cursor(const bit, const bit, const uchar8);
//�Զ���CGRAM
void LCD_Create_Character(const uchar8, const uchar8* const);


// 2 x 16 ��ʾģʽ
#define LCD_LINES 2
#define LCD_CHARACTERS 16

// �û��Զ����ַ�
#define LCD_UDC_POUNDS 1
#define LCD_UDC_DEGREES_C 2 

char LCD_data_G[LCD_LINES][LCD_CHARACTERS+1];//��ʾ���ݵ�����,�ⲿ����

#endif