
#include "Battery_Charging.h"

extern sTask Sch_tasks_G[SCH_MAX_TASKS];

extern uchar8 Error_code_G;
 
  
void Sch_Init_T2(void) 
   {
       uchar8 i;

       for (i = 0; i < SCH_MAX_TASKS; i++) 
          {
             Sch_Delete_Task(i);
          }

       Error_code_G = 0;  

     /*************************************************************************
           *
           * TIMER2初始化
           *
           * T2CON：TIMER2 控制寄存器
           * bit 7 未实现：读为0
           * bit 6-3 TOUTPS<3:0>：Timer2 输出后分频比选择位
           * 0000 = 1:1 后分频比
           * 0001 = 1:2 后分频比
           * 0010 = 1:3 后分频比
           * 0011 = 1:4 后分频比
           * 0100 = 1:5 后分频比
           * 0101 = 1:6 后分频比
           * 0110 = 1:7 后分频比
           * 0111 = 1:8 后分频比
           * 1000 = 1:9 后分频比
           * 1001 = 1:10 后分频比
           * 1010 = 1:11 后分频比
           * 1011 = 1:12 后分频比
           * 1100 = 1:13 后分频比
           * 1101 = 1:14 后分频比
           * 1110 = 1:15 后分频比
           * 1111 = 1:16 后分频比
           * 
           * bit 2 TMR2ON：Timer2 使能位
           * 1 = 使能Timer2
           * 0 = 关闭Timer2
           *
           * bit 1-0 T2CKPS<1:0>：Timer2 时钟预分频比选择位
           * 00 = 预分频比为1
           * 01 = 预分频比为4
           * 1x = 预分频比为16
           * 
          *************************************************************************/

        T2CON = 0x49; // Fosc/4, 1:4预分频比1us(16Mhz) 1:10后分频比 10us b01001001 0x49
                      // Fosc/4, 1:16预分频比4us(16Mhz) 1:10后分频比 40us b01001010 0x4A
        PR2 = 0x63; // 1ms时标 0x63 0x18  //5ms 0x7C    //2ms  0xC7  0x31
        TMR2IF = 0;
        TMR2IE = 1;
        PEIE = 1;
        
        GIE = 1;
        TMR2ON = 1;
   
   }
   
void Sch_Start(void) 
   {
       GIE = 1;
   }
   

void interrupt Sch_Update(void) 
   {
       uchar8 Index;

       TMR2IF = 0;

       for (Index = 0; Index < SCH_MAX_TASKS; Index++)
          {
          if (Sch_tasks_G[Index].pTask)
             {

             if (Sch_tasks_G[Index].Delay == 0)
                {
                Sch_tasks_G[Index].RunMe += 1;
                
                if (Sch_tasks_G[Index].Period)
                   {
                   Sch_tasks_G[Index].Delay = Sch_tasks_G[Index].Period;
                   }
                }
             else
                {
                Sch_tasks_G[Index].Delay -= 1;
                }
             }         
          }
   }     
   
   