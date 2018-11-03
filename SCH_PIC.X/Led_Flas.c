
#include "LED_Flas.h"

extern sTask SCH_tasks_G[SCH_MAX_TASKS];

extern uchar8 Error_code_G;


static uchar8 LED_state_G;

void LED_Flash_Init(void)
   {
   LED_state_G = 0;
   }

void LED_Flash_Update(void)
   {
   if (LED_state_G == 1)
      {
      LED_state_G = 0;
      LED_pin = 1;
      }
   else
      {
      LED_state_G = 1;
      LED_pin = 0;
      }
   }
   
void interrupt SCH_Update(void) 
   {
   uchar8 Index;

   TMR2IF = 0;

   for (Index = 0; Index < SCH_MAX_TASKS; Index++)
      {
      if (SCH_tasks_G[Index].pTask)
         {
         if (SCH_tasks_G[Index].Delay == 0)
            {
            SCH_tasks_G[Index].RunMe += 1;
            if (SCH_tasks_G[Index].Period)
               {
               SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
               }
            }
         else
            {
            SCH_tasks_G[Index].Delay -= 1;
            }
         }         
      }
   }   
  
void SCH_Init_T2(void) 
   {
   uchar8 i;

   for (i = 0; i < SCH_MAX_TASKS; i++) 
      {
      SCH_Delete_Task(i);
      }

   Error_code_G = 0;  

   T2CON = 0x02;
   PR2    = 0xF9;
   
   TMR2IF = 0;
   TMR2IE   = 1;
//   GIE = 1;
   PEIE = 1;
   TMR2ON   = 1;
   
    }

  
void PIC_Init(void)
{
    PORTD = 0xFF;
    TRISD = 0x00;  
}
   
   