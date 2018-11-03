
#include "Main.h"
#include "Sch_Pic.h"
#include "Sch_Timer.h"
#include "Port_Config.h"
#include "Hc595_io.h"
#include "Keypad.h"
#include "Yhh.h"

void main(void)
{
    Port_Init();
    KEYPAD_Init();
    YHH_Init();
    Sch_Init_Timer();
    Sch_Add_Task(DisplayScan, 0, 2);
    Sch_Add_Task(KEYPAD_Update, 1, 50);
    Sch_Add_Task(YHH_Update, 2, 50);
  //Sch_Start();
   while(1)
   {
       Sch_Dispatch_Tasks();
   }
}