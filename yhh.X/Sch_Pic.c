
#include "Main.h"
#include "Sch_Pic.h"

uchar8 Error_code_G = 0;

static uchar8 Last_error_code_G;

void Sch_Dispatch_Tasks(void)
{
    uchar8 Index;
    
    for (Index = 0; Index < SCH_MAX_TASKS; Index++)
    {
        if (Sch_tasks_G[Index].RunMe > 0)
        {
            //运行任务
            (*Sch_tasks_G[Index].pTask)();
            Sch_tasks_G[Index].RunMe -= 1;
            
            if (Sch_tasks_G[Index].Period == 0)
            {
                //Sch_Delete_Task(Index);
                Sch_tasks_G[Index].pTask = 0;
            }
         }
      }
   
  // Sch_Report_Status();

}


/*------------------------------------------------------------------*-

  Task_ID = SCH_Add_Task(Do_X,1000,0);
  函数 Do_X() 在 1000 个时标后运行一次后删除.            

  Task_ID = SCH_Add_Task(Do_X,0,1000);
  函数 Do_X() 每隔 1000 个时标运行一次.            

  Task_ID = SCH_Add_Task(Do_X,300,1000);
  函数 Do_X() 每隔 1000 个时标运行一次.
  第 300 时标后开始运行, 以后 1300, 2300, 等.            
 
-*------------------------------------------------------------------*/

uchar8 Sch_Add_Task(void (* pFunction)(), 
                      uint16 DELAY, 
                      uint16 PERIOD)
{
    uchar8 Index = 0;
    
    while ((Sch_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
    {
        Index++;
    }
    
    if (Index == SCH_MAX_TASKS)
    {
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        return SCH_MAX_TASKS;
    }

   Sch_tasks_G[Index].pTask  = pFunction;   
   Sch_tasks_G[Index].Delay  = DELAY;
   Sch_tasks_G[Index].Period = PERIOD;
   Sch_tasks_G[Index].RunMe  = 0;

   return Index;
}

__bit Sch_Delete_Task(const uchar8 TASK_INDEX)
{
    static __bit Return_code;
    
    if (Sch_tasks_G[TASK_INDEX].pTask == 0)
    {
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        Return_code = RETURN_ERROR;
    }
    else
    {
        Return_code = RETURN_NORMAL;
    }      
   
   Sch_tasks_G[TASK_INDEX].pTask   = 0x0000;
   Sch_tasks_G[TASK_INDEX].Delay   = 0;
   Sch_tasks_G[TASK_INDEX].Period  = 0;
   Sch_tasks_G[TASK_INDEX].RunMe   = 0;

   return Return_code;
}

void Sch_Report_Status(void)
{
#ifdef SCH_REPORT_ERRORS

#endif
}
