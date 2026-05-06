#ifndef __Systick_H
#define __Systick_H
#include "stm32f10x.h"
extern volatile   uint32_t tick;
void Systick_Init(void);

uint32_t Get_Tick(void);

#endif
