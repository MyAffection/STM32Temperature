#include "stm32f10x.h"                  // Device header
#include "ExternFile.h"


volatile uint32_t Last_Active_Time = 0;
volatile uint8_t  System_Is_Sleeping = 0; 

void System_Enter_Sleep(void){
OLED_Clear();
GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 开启电源时钟

while(System_Is_Sleeping == 1){


Set_RTC_ALarm(2);
PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);

SystemInit();

MYIWDG_Feed();
	TempSensorADC_Init();
	for (int i = 0; i < 20; i++) {
            Get_Temperaure(); 
        }
	
	float current_temp = Get_Temperaure(); 
	if(current_temp >= 25.0f){
	System_Is_Sleeping = 0;
		
	Last_Active_Time = Get_Tick();
	
	
	
	}
	

}
OLED_Init();


}
