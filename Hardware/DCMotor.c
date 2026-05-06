#include "stm32f10x.h"                  // Device header
#include "TempTIM.h"


void DCMotor_Init(void){
	


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_Initstruct;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);

	
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstruct);


	
	
	//配置TIM3生成了PWM波形 ，功能是输出比较OC
	TIM_OCInitTypeDef TIM_OCInitStruct;
	//配置默认的结构体，因为不需要所有的参数
    TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //高低电平有效
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);

//开启预装载功能
   TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

}



//电机档位

//直流电机运行

void DCMotor_Speed_OFF(void){
	
GPIO_SetBits(GPIOB, GPIO_Pin_1);   // PB0 = 1
GPIO_ResetBits(GPIOB, GPIO_Pin_0); // PB1 = 0

TIM_SetCompare1(TIM3,0);


}

void DCMotor_Speed_LOW(void){
	
GPIO_SetBits(GPIOB, GPIO_Pin_1);   // PB0 = 1
GPIO_ResetBits(GPIOB, GPIO_Pin_0); // PB1 = 0

TIM_SetCompare1(TIM3,30);


}

void DCMotor_Speed_MID(void){
	
GPIO_SetBits(GPIOB, GPIO_Pin_1);   // PB0 = 1
GPIO_ResetBits(GPIOB, GPIO_Pin_0); // PB1 = 0

TIM_SetCompare1(TIM3,60);


}


void DCMotor_Speed_MAX(void){
	
GPIO_SetBits(GPIOB, GPIO_Pin_1);   // PB0 = 1
GPIO_ResetBits(GPIOB, GPIO_Pin_0); // PB1 = 0

TIM_SetCompare1(TIM3,60);


}
