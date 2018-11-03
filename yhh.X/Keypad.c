#include "Keypad.h"

void KEYPAD_Init(void)
{
    Key_val = 0;
    Key_cont = 0;
}

void KEYPAD_Update(void)
{
  Key_val = KEY_VAL & (KEY_VAL ^ Key_cont);
  Key_cont = KEY_VAL;
}