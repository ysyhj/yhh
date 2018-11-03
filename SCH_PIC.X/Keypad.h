#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "Main.h"
#include "Port_Config.h"

void KEYPAD_Init(void);
void KEYPAD_Update(void);

bit  KEYPAD_Get_Data_From_Buffer(char* const, char* const); //外部调用取得键值
void KEYPAD_Clear_Buffer(void);

#endif