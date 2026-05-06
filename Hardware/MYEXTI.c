#include "stm32f10x.h"                  // Device header
#include "DCMotor.h"
#include "Buzzer.h"
#include "Delay.h"
#include "OLED.h"
#include "MYIWDG.h"
#include "LED.h"
#include "Systick.h"
#include "Key.h"
extern uint8_t UI_State;
volatile uint8_t Tamper_Flag = 0;
void MYEXTI_Init(void){
Key_Init();
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 


//配置中断通道
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);

   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);

   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);


	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line13 | EXTI_Line10 | EXTI_Line8;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //疑问
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; //为什么是下降沿
    EXTI_Init(&EXTI_InitStruct);
	
	
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;//子优先级
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//子优先级
	NVIC_Init(&NVIC_InitStruct);
	

}


extern volatile uint32_t Last_Active_Time ; 
extern volatile uint8_t System_Is_Sleeping ; // 0=醒着, 1=睡着




//配置中断函数
void EXTI15_10_IRQHandler(void){

	
if(EXTI_GetITStatus(EXTI_Line13) == SET){
uint32_t current_time = Get_Tick();
static uint32_t last_time = 0;

	if(current_time - last_time >= 50){
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 1) {
                GPIO_SetBits(GPIOA, GPIO_Pin_4);
                Tamper_Flag = 1; 
			last_time = current_time; 
		
		  System_Is_Sleeping = 0;
		  Last_Active_Time = Get_Tick();
		
            }

		
			
	}
	EXTI_ClearITPendingBit(EXTI_Line13);
}

if(EXTI_GetITStatus(EXTI_Line10) == SET) {


System_Is_Sleeping = 0;
  Last_Active_Time = Get_Tick();
	
	
EXTI_ClearITPendingBit(EXTI_Line10); 
	
	
	
}

	

}


void EXTI9_5_IRQHandler(void){



if(EXTI_GetITStatus(EXTI_Line8) == SET) {
        
      
        System_Is_Sleeping = 0;
        Last_Active_Time = Get_Tick();
        
    
  
        EXTI_ClearITPendingBit(EXTI_Line8); 
    }








}














//对标志位进行处理

void System_defense(void){

if(Tamper_Flag == 1){

    OLED_Clear();
    OLED_ShowString(2, 2, "!! TAMPERED !!"); 
    DCMotor_Speed_OFF();
	Safe_LED_OFF();
	Warring_LED_OFF();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	for(uint16_t i = 0; i < 20; i++) { 
    GPIO_SetBits(GPIOA, GPIO_Pin_4); 
    GPIO_SetBits(GPIOA, GPIO_Pin_7); 
		// 强行通电
    Delay_ms(100);                     // 稍微延时久一点，50ms太短了
    GPIO_ResetBits(GPIOA, GPIO_Pin_4); // 强行断电
    GPIO_ResetBits(GPIOA, GPIO_Pin_7); // 强行断电

    Delay_ms(100);
    MYIWDG_Feed();
        }
	
	
	while(1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_7); 
		OLED_ShowString(4, 1, "SYSTEM LOCKED.");
        MYIWDG_Feed(); // 继续喂狗维持死亡画面
        }
	
	
	
	
	
	
}


}

