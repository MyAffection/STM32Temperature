#include "stm32f10x.h"
#include "Systick.h"
#include "Store.h"
#include "OLED.h"






#define KEY_NONE   0
#define KEY1_SHORT 1  // PA8 短按
#define KEY1_LONG  2  // PA8 长按
#define KEY2_SHORT 3  // PA10 短按


// 引入 main.c 里的变量名片
extern uint8_t UI_State;
extern uint8_t Tamper_Flag;

void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO , ENABLE);
    GPIO_InitTypeDef GPIO_Initstruct;
    
    // ：下拉输入
    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IPD; 
	   GPIO_Initstruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_8; // 减号与SET
    GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOA, &GPIO_Initstruct);

}





uint8_t Key_Scan(void){
    
    static uint32_t Last_Time = 0;
    
    // PA8 专用的“秒表”和“防连发锁”
    static uint16_t key1_hold_time = 0;
    static uint8_t  key1_long_triggered = 0;
    
    // PA10 专用的“单发狙击枪锁” (就是你原来写的那个)
    static uint32_t key2_lock = 1;

    // 核心心跳：每 10ms 进来扫描一次
    if(Get_Tick() - Last_Time >= 10){
        Last_Time = Get_Tick();
        
        uint8_t Key1 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
        uint8_t Key2 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);
        

        if(Key1 == 1){ 
            key1_hold_time++; // 只有按下了，秒表才开始走！
            
            if(key1_hold_time >= 200){ // 达到 2 秒
                if(key1_long_triggered == 0){
                    key1_long_triggered = 1; // 触发长按，上锁！
                    return KEY1_LONG;        // 返回 2
                }
            }
        } 
        else { // PA8 松开了
            // 如果秒表大于 5 (消抖)，并且没触发过长按，那就是短按！
            if(key1_hold_time > 5 && key1_long_triggered == 0){
                key1_hold_time = 0;
                return KEY1_SHORT;           // 返回 1
            }
            // 彻底复位 PA8 的状态
            key1_hold_time = 0;
            key1_long_triggered = 0;
        }

        if(Key2 == 1){
            if(key2_lock == 1){    // 如果子弹上膛了
                key2_lock = 0;     // 扣动扳机，打完子弹！
                return KEY2_SHORT; // 返回 3
            }
        } 
        else {
            key2_lock = 1; // PA10 松开，重新上膛！
        }
    }
    
    return KEY_NONE; // 啥事没发生，返回 0
}

extern uint8_t UI_State;

void Key_Task(void){
    uint8_t Key_Val = Key_Scan();

    if(Key_Val == 0){
        return;
    }

  
    if(Key_Val == 1 ){
        if(UI_State == 0){
            UI_State = 1; // 在大厅，进控制室
        }
        else if(UI_State == 1){
            UI_State = 0; // 在控制室，退回大厅
			
            Flash_Save_Params();
        }
    }

    else if(Key_Val == 3){
		
        if(UI_State == 1){ 
			
            sys_params.High_Temp++;
		
            
            if(sys_params.High_Temp > 29){ 
				
                sys_params.High_Temp = 27;
				
            }
        }
    }else if(Key_Val == 2){
		if(UI_State == 0){
		UI_State =2;
		}else if(UI_State ==2){
		
			
				UI_State =0;
			
		
		
		}

	}
	
	
	
}


















