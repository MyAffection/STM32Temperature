#include "stm32f10x.h"                  // Device header
#include "ExternFile.h"

void MYRTC_Init(void){
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
PWR_BackupAccessCmd(ENABLE);
	if(BKP_ReadBackupRegister(BKP_DR1) != 0x5050){
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) ==RESET){};
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
    RTC_WaitForLastTask();
	RTC_SetPrescaler(32768 - 1 ); // 1秒走1下
     RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1, 0x5050);
	}else {
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();

	}
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    RTC_ITConfig(RTC_IT_ALR, ENABLE);
    RTC_WaitForLastTask();
}

void Set_RTC_ALarm(uint32_t seconds){

    RTC_WaitForLastTask();
    RTC_SetAlarm(RTC_GetCounter() + seconds); 
    RTC_WaitForLastTask();

}


void RTCAlarm_IRQHandler(void){
if(RTC_GetITStatus(RTC_IT_ALR) != RESET){
EXTI_ClearITPendingBit(EXTI_Line17);
RTC_WaitForLastTask();
RTC_ClearITPendingBit(RTC_IT_ALR);
RTC_WaitForLastTask();

}



}











