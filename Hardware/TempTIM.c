#include "stm32f10x.h"                  // Device header
///做定时任务，产生一个1s的定时中断
void TempTIM_Init(void){

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//选择定时器的时钟源  
	// TIM_InternalClockConfig(TIM3);

	TIM_TimeBaseInitTypeDef  TIM_Initstruct;
	
	TIM_Initstruct.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频
	TIM_Initstruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Initstruct.TIM_Period =     100-1;         //arr,自动重装载值
	TIM_Initstruct.TIM_Prescaler =  720-1;        //psc预分频值
	TIM_Initstruct.TIM_RepetitionCounter = 0;    //仅仅高级的定时器有效
    TIM_TimeBaseInit(TIM3,&TIM_Initstruct);


	//选择触发源定时器中断更新中断
	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	//开启中断CPU中断当前工作
	//TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//配置NVIC
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitTypeDef  NVIC_Initstruct;
//	NVIC_Initstruct.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_Initstruct.NVIC_IRQChannelCmd  = ENABLE;
//	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_Initstruct.NVIC_IRQChannelSubPriority = 1;
//	
//	NVIC_Init(&NVIC_Initstruct);
//	
	
	
	
	//开启输出比较的通道
	
	TIM_Cmd(TIM3,ENABLE);
	

}





