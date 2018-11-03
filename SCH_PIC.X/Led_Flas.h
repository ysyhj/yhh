
#ifndef _LED__FLAS_H_
#define _LED__FLAS_H_

#include "Main.h"
#include "Sch_Pic.h"


#define LED_pin  RD0

void SCH_Init_T2(void);
void LED_Flash_Init(void);
void LED_Flash_Update(void);
void PIC_Init(void);

#endif


