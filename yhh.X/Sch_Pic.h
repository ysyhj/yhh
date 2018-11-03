#ifndef __SCH_PIC_H__
#define __SCH_PIC_H__

#define SCH_MAX_TASKS  (3)

typedef struct
{
    void (* pTask)(void);
    uint16 Delay;
    uint16 Period;
    uchar8 RunMe;
    
} sTask; 

void  Sch_Dispatch_Tasks(void);
uchar8 Sch_Add_Task(void (*) (void), const uint16, const uint16);  
__bit   Sch_Delete_Task(const uchar8);
void  Sch_Report_Status(void);

extern sTask Sch_tasks_G[SCH_MAX_TASKS]; //�ⲿҪ����

extern uchar8 Error_code_G; //�ⲿҪ����

#endif