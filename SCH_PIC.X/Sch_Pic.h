#ifndef _SCH__PIC_H_
#define _SCH__PIC_H_

#include "Main.h"

#define SCH_MAX_TASKS  (2)

typedef struct 
   {
       void (* pTask)(void);
       uint16 Delay;
       uint16 Period;
       uchar8 RunMe;       
   } sTask; 


void  Sch_Dispatch_Tasks(void);
uchar8 Sch_Add_Task(void (*) (void), const uint16, const uint16);  
bit   Sch_Delete_Task(const uchar8);
void  Sch_Report_Status(void);

sTask Sch_tasks_G[SCH_MAX_TASKS]; //外部要调用

uchar8 Error_code_G; //外部要调用


#endif