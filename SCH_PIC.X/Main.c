
#include "Main.h"
#include "Sch_Pic.h"
#include "Sch_Timer.h"
#include "Led_Mx4.h"
#include "Key_Led.h"
#include "Keypad.h"


void main(void)
   {
    
    Port_Init();
    
    KEYPAD_Init();
    
    Key_Led_Init();
     
    Sch_Init_Timer();

    Sch_Add_Task(Led_MX4_Display_Update, 0, 3);
    
    Sch_Add_Task(Key_Led_Update, 0, 50);
   
  //Sch_Start();
   
   while(1)
      {
      Sch_Dispatch_Tasks();
      }
   }