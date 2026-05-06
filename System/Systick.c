#include "stm32f10x.h"                  // Device header
 volatile uint32_t tick = 0;

void Systick_Init(void){
	
SysTick_Config(SystemCoreClock / 1000);  // 1ms中断
	
}


uint32_t Get_Tick(void)
{
    return tick;
}
