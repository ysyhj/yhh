
#include "Key_Led.h"

 extern uchar8 Led_Mx4_Data_G[4];
 extern const uchar8 LED_Table_G[20];
 

 
 void Key_Led_Init(void)
 {

  KEYPAD_Clear_Buffer();
  
 }
 
 
 void Key_Led_Update(void)
 {
  char Key, FnKey;

   KEYPAD_Update();
   
   Led_Mx4_Data_G[1] = LED_Table_G[Key];

   if (KEYPAD_Get_Data_From_Buffer(&Key, &FnKey) == 0)
      {
     return;
      }
       
       Led_Mx4_Data_G[3] = LED_Table_G[Key];
   }