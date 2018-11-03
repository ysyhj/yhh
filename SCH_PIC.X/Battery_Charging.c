
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
           * TIMER2��ʼ��
           *
           * T2CON��TIMER2 ���ƼĴ���
           * bit 7 δʵ�֣���Ϊ0
           * bit 6-3 TOUTPS<3:0>��Timer2 ������Ƶ��ѡ��λ
           * 0000 = 1:1 ���Ƶ��
           * 0001 = 1:2 ���Ƶ��
           * 0010 = 1:3 ���Ƶ��
           * 0011 = 1:4 ���Ƶ��
           * 0100 = 1:5 ���Ƶ��
           * 0101 = 1:6 ���Ƶ��
           * 0110 = 1:7 ���Ƶ��
           * 0111 = 1:8 ���Ƶ��
           * 1000 = 1:9 ���Ƶ��
           * 1001 = 1:10 ���Ƶ��
           * 1010 = 1:11 ���Ƶ��
           * 1011 = 1:12 ���Ƶ��
           * 1100 = 1:13 ���Ƶ��
           * 1101 = 1:14 ���Ƶ��
           * 1110 = 1:15 ���Ƶ��
           * 1111 = 1:16 ���Ƶ��
           * 
           * bit 2 TMR2ON��Timer2 ʹ��λ
           * 1 = ʹ��Timer2
           * 0 = �ر�Timer2
           *
           * bit 1-0 T2CKPS<1:0>��Timer2 ʱ��Ԥ��Ƶ��ѡ��λ
           * 00 = Ԥ��Ƶ��Ϊ1
           * 01 = Ԥ��Ƶ��Ϊ4
           * 1x = Ԥ��Ƶ��Ϊ16
           * 
          *************************************************************************/

        T2CON = 0x49; // Fosc/4, 1:4Ԥ��Ƶ��1us(16Mhz) 1:10���Ƶ�� 10us b01001001 0x49
                      // Fosc/4, 1:16Ԥ��Ƶ��4us(16Mhz) 1:10���Ƶ�� 40us b01001010 0x4A
        PR2 = 0x63; // 1msʱ�� 0x63 0x18  //5ms 0x7C    //2ms  0xC7  0x31
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
   
   