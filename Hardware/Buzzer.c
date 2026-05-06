#include "stm32f10x.h"                  // Device header
#include "Systick.h"
uint16_t BuzzerWork_LastTime = 0;
uint16_t TEMPSecond = 0;
//控制蜂鸣器
void Buzzer_Init(void){

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

     GPIO_SetBits(GPIOA,GPIO_Pin_4);




}

 //传入参数控制蜂鸣器的频率
void Buzzer_ON(uint8_t Status){
uint32_t CurrentTime = 0;
static uint8_t LastStatus = 10;
static uint32_t LastTime  ;
uint32_t PassTime = 0;	
//每5秒响一秒，也就是4秒空闲和1秒的工作时间合起来就是一个周期。
CurrentTime = Get_Tick();

	if(Status != LastStatus){
	
	LastStatus = Status;
	LastTime = CurrentTime;
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
	}
	PassTime = CurrentTime - LastTime;	

	
	if(Status == 0){
		if(PassTime >= 5000){
	LastTime = CurrentTime;
	PassTime = 0;
	}if(PassTime <1111 )
	{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
	}else if(PassTime >=957 ){
	    GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	
	}else if(Status ==1){
	
	if(PassTime >= 500 ){
	LastTime = CurrentTime;
	PassTime = 0;
	}if(PassTime >= 250){
	
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}else if(PassTime < 250){
	
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	}

}else if (Status == 2){

GPIO_SetBits(GPIOA,GPIO_Pin_4);

}

}


void Buzzer_OFF(void ){


GPIO_SetBits(GPIOA,GPIO_Pin_4);



}
