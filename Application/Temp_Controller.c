#include "stm32f10x.h"                  // Device header
#include "ExternFile.h"



//这里来处理逻辑判断处理

//解决疯狂闪缩的问题，定义系统转态
volatile uint8_t System_State = 0;

void Temp_Status_Task(void){
float base_high= (float)sys_params.High_Temp;
 float  GlobalTemp = Get_Temperaure();
	
	if(System_State == 0){	
	if(GlobalTemp > base_high + 0.5f ){
	System_State =1;
		
	}else if(GlobalTemp <24.5f ){
	System_State =6;
	}
	}else if(System_State == 1){
	if(GlobalTemp > base_high + 2.5f){
		
	Add_New_Log((uint16_t)GlobalTemp);

	System_State = 2;
	}else if(GlobalTemp < base_high - 0.5f){
	System_State = 0;
	  }
	}
	else if(System_State == 2){
		
		if ((uint16_t)GlobalTemp > sys_params.Log_Temp[0]) {
        sys_params.Log_Temp[0] = (uint16_t)GlobalTemp; // 内存里的值跟着火苗一起飙升
	
			
    }
	if(GlobalTemp  < base_high + 1.5f ){
	Flash_Save_Params();
	System_State = 1;

	}
	
	} else if(System_State ==6){
	if(GlobalTemp >25.5){
	System_State = 0;
	
	}
	
	
	}

	
	
if(System_State == 6){
DCMotor_Speed_OFF();
Safe_LED_ON();
Warring_LED_OFF();
Buzzer_ON(2);
LED_STOPBreath();
 
	}else if(System_State == 0	){
	Buzzer_ON(2);
	Safe_LED_ON();
	Warring_LED_OFF();
	DCMotor_Speed_LOW();
    LED_STOPBreath();
	}
	
else if(System_State == 1 ){
		
	Safe_LED_OFF();
	Warring_LED_ON();
	Buzzer_ON(0);
	DCMotor_Speed_MID();
	LED_STOPBreath();

}else if(System_State == 2){
	
	GPIO_InitTypeDef GPIO_Initstruct;
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
	 LED_PWM();

	LED_Breath();
    DCMotor_Speed_MAX();
	
	Safe_LED_OFF();
	Warring_LED_OFF();
  	Buzzer_ON(1);
	
		
}	
}



















