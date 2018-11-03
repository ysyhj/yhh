
#ifndef _LED__MAX4_H_
#define _LED__MAX4_H_

#include "Main.h"
#include "Port_Config.h"

#define LED_NUM_DIGITS (4)

#ifndef ON
#define OFF (1)
#define ON (!OFF)
#endif

////数码管的引脚定义示意
//#define  LED_DIGIT_1  RC0
//#define  LED_DIGIT_2  RC1
//#define  LED_DIGIT_3  RC2
//#define  LED_DIGIT_4  RC5

//数码管显示数据
uchar8 Led_Mx4_Data_G[LED_NUM_DIGITS];//数码管显示数据,外部要调用 数码管从左至右 0->3

//共阳数码管段码表,数码管译码数据,外部要调用
 const uchar8 LED_Table_G[20] =
// 0     1      2     3     4      5      6     7      8     9
{0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
// 0.    1.     2.    3.    4.     5.     6.    7.     8.    9.
 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10};

//共阴数码管段码表,数码管译码数据,外部要调用
// const uchar8 LED_Table_G[20] =
//  0     1     2     3     4     5     6     7     8     9
//{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
// 0.    1.    2.    3.    4.    5.    6.    7.    8.    9.
// 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};


void Led_MX4_Display_Update(void);

#endif