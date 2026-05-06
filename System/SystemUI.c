#include "stm32f10x.h"                  // Device header
#include "ExternFile.h"


//系统菜单
extern uint8_t UI_State;
extern uint16_t Log_Temp[];
extern uint8_t Write_Index;
extern uint8_t Log_Count ;
float tempC; //测量出来的温度
extern  uint8_t System_State;
void Show_Task(void){
	

    static uint32_t last_time = 0;
    static uint8_t blink_flag = 0; 

 
    if(Get_Tick() - last_time >= 500) {
        last_time = Get_Tick();
        
        // 每次进门，把开关反转一下 (实现 0.5秒亮，0.5秒灭)
        blink_flag = !blink_flag; 
        if(UI_State == 0) {
			
            OLED_ShowString(1, 1, "Normal Mode     "); 
            OLED_ShowString(2, 1, "Temp:     C    "); //
		    OLED_ShowNum(2, 6, sys_params.High_Temp, 2); 

            OLED_ShowString(3, 1, "                "); 
			  uint16_t ADCVALUE = Get_TempSensorADCValue();
                // ⚠️ 确保 Get_Temperaure() 拼写和定义一致！
                tempC = Get_Temperaure(); 
                uint16_t temp_int = (uint16_t)tempC;
                uint16_t temp_dec = (uint16_t)((tempC - temp_int) * 100.0f + 0.5f);
                OLED_ShowString(4,1,"00.00C");

            
                OLED_ShowNum(4,1,temp_int,2);
                OLED_ShowNum(4,4,temp_dec,2);
        
            
          
                if(System_State == 6) OLED_ShowString(4, 10, "OFF   ");
                else if(System_State == 0) OLED_ShowString(4, 10, "LOW   ");
                else if(System_State == 1) OLED_ShowString(4, 10, "MID   ");
                else if(System_State == 2) OLED_ShowString(4, 10, "MAX   ");    
			
			
			
			
			
			
        }

        else if(UI_State == 1) {
            // 第1行：标题
            OLED_ShowString(1, 1, "Set MAX Temp:   ");
            // 第3行：彻底清空
            if(blink_flag == 0) {
                // 亮：画出真实的数字
				OLED_ShowString(2, 1, "Temp:     C    "); //
                OLED_ShowNum(2, 6, sys_params.High_Temp, 2); 
            } else {
                // 灭：用空格盖住数字，实现闪烁感
                OLED_ShowString(2, 6, "     ");            
            }
			OLED_ShowString(3, 1, "                ");
			
			
			
			  uint16_t ADCVALUE = Get_TempSensorADCValue();
                // ⚠️ 确保 Get_Temperaure() 拼写和定义一致！
                tempC = Get_Temperaure(); 
                uint16_t temp_int = (uint16_t)tempC;
                uint16_t temp_dec = (uint16_t)((tempC - temp_int) * 100.0f + 0.5f);
                OLED_ShowString(4,1,"00.00C");

            
                OLED_ShowNum(4,1,temp_int,2);
                OLED_ShowNum(4,4,temp_dec,2);
        
            
          
                if(System_State == 6) OLED_ShowString(4, 10, "OFF   ");
                else if(System_State == 0) OLED_ShowString(4, 10, "LOW   ");
                else if(System_State == 1) OLED_ShowString(4, 10, "MID   ");
                else if(System_State == 2) OLED_ShowString(4, 10, "MAX   ");    
			
			
			
			
			
			
			
        }else if (UI_State == 2){
		
		
		OLED_ShowString(1,1,"OVERHEAT_LOG");

	   OLED_ShowString(2, 1, " 1  2  3  4  5  ");
 
       OLED_ShowNum(3,1,sys_params.Log_Temp[0],2);
			
       OLED_ShowNum(3,4,sys_params.Log_Temp[1],2);

	   OLED_ShowNum(3,7,sys_params.Log_Temp[2],2);

	   OLED_ShowNum(3,10,sys_params.Log_Temp[3],2);

       OLED_ShowNum(3,13,sys_params.Log_Temp[4],2);
	
      OLED_ShowString(4, 1, "Count:          ");
      OLED_ShowNum(4, 8, sys_params.Log_Count, 4);
      OLED_ShowString(4, 12, "     "); 
    
}
}
	}
//
void Start_POST(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

OLED_Clear();
OLED_ShowString(2, 3, "System V1.0");
OLED_ShowString(3, 4, "Booting...");


Danger_LED_OFF();
 Warring_LED_OFF();
    Safe_LED_OFF();


   Danger_LED_ON();   
    Delay_ms(150); 
    Danger_LED_OFF();


Warring_LED_ON();  
    Delay_ms(150); 
    Warring_LED_OFF();


Safe_LED_ON();     
    Delay_ms(150); 
    Safe_LED_OFF();
	
	
	Danger_LED_ON();   
    Delay_ms(150); 
    Danger_LED_OFF();
	
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4); 
    Delay_ms(200);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4); 

    // 4. 停顿一下，让用户的眼睛能看清屏幕上的 "V1.0"
    Delay_ms(800); 
    
    // 5. 擦除黑板，准备进入主界面
    OLED_Clear();
}






