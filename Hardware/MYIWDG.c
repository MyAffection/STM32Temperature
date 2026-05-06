#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Buzzer.h"

void MY_IWDG_Init(void){

//取消写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

//慢调时钟 4khz/64 = 625
	IWDG_SetPrescaler(IWDG_Prescaler_64);

//设定重装值 625
	IWDG_SetReload(1875);

//喂狗
	IWDG_ReloadCounter();

      IWDG_Enable();

}


void MYIWDG_Feed(void){


IWDG_ReloadCounter();

}

void System_Boot_Check(void){

if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) !=RESET ){
    OLED_ShowString(1, 1, "Watchdog Reset!");
        Buzzer_ON(1);         
        Delay_ms(1500); // 
        RCC_ClearFlag(); // 

}else {
OLED_ShowString(1, 1, "System V1.0    ");
        OLED_ShowString(2, 1, "POST OK...     ");
        Delay_ms(1000); // 留给人类欣赏的开机动画


}

}
