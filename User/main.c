#include "stm32f10x.h"                  // Device header
#include "ExternFile.h"

volatile uint8_t UI_State = 0;
extern uint32_t Last_Active_Time;
extern uint8_t  System_Is_Sleeping ; 

int main()
{  
	Systick_Init();
 
    TempSensorADC_Init();
    LED_Init();
    MYEXTI_Init();
	Buzzer_Init();
	TempTIM_Init();
	DCMotor_Init();
	LED_PWM();
	Key_Init();
	OLED_Init();
	Start_POST();
	System_Boot_Check();
	Systick_Init();
   
    MYRTC_Init();
    Store_Read_Params();
    Last_Active_Time = Get_Tick();
    MY_IWDG_Init();

	
    while(1)
		
    {
		if(System_Is_Sleeping == 0){
		
		Temp_Status_Task();
        Key_Task();
	    Show_Task();
         System_defense();
	     MYIWDG_Feed();

			float current_temp = Get_Temperaure();
			
			if (current_temp >= 25.0f) {
                Last_Active_Time = Get_Tick(); 
            }
						
			
		if(Get_Tick() -Last_Active_Time >= 20000 ){
			
					
			System_Is_Sleeping = 1;
			
			System_Enter_Sleep();

			
	
		
		}

    }
}
}
