#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "Main.h"
#include "Port_Config.h"

uchar8 Key_val; //键值
uchar8 Key_cont;    // 连续按下

void KEYPAD_Init(void);
void KEYPAD_Update(void);

#endif