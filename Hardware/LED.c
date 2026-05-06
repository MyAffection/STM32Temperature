#include "stm32f10x.h"                  // Device header
#include "TempTIM.h"
#include "Systick.h"
uint32_t CurrentTime = 0;
uint32_t CycleTime = 0;

void LED_Init(void){


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitTypeDef GPIO_Initstruct;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);

   
	
	
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_2 );

}

//开灯 GPIOA1和GPIOA2作为信息灯 
void Safe_LED_ON(void){

GPIO_SetBits(GPIOA,GPIO_Pin_1);

}
void Safe_LED_OFF(void){

GPIO_ResetBits(GPIOA,GPIO_Pin_1);

}

void Warring_LED_ON(void){

GPIO_SetBits(GPIOA,GPIO_Pin_2);

}
void Warring_LED_OFF(void){

GPIO_ResetBits(GPIOA,GPIO_Pin_2);

}
void Danger_LED_ON(void){
GPIO_SetBits(GPIOA,GPIO_Pin_7);

}

void Danger_LED_OFF(void){

GPIO_ResetBits(GPIOA,GPIO_Pin_7);


}
//使用定时器来产生呼吸灯的效果

//
void LED_PWM(void){
	//设置输出比较

	TIM_OCInitTypeDef TIM_OCInitstruct;
	TIM_OCStructInit(&TIM_OCInitstruct);
	TIM_OCInitstruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitstruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitstruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitstruct.TIM_Pulse = 0; 
	
    TIM_OC2Init(TIM3,&TIM_OCInitstruct);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);




}
// 放在 while(1) 里的非阻塞呼吸灯函数
void LED_Breath(void) 
{
    static uint32_t last_tick = 0;
    static uint8_t  pwm_val = 0;    // 当前亮度 (0~100)
    static uint8_t  direction = 1;  // 1代表变亮，0代表变暗

    // 每 15 毫秒改变一次亮度，不仅顺滑，而且绝不卡顿 CPU！
    if (Get_Tick() - last_tick >= 15) {
        last_tick = Get_Tick();

        // 1. 计算下一步的亮度
        if (direction == 1) {
            pwm_val++;
            if (pwm_val >= 100) direction = 0; // 亮到头了，该变暗了
        } else {
            pwm_val--;
            if (pwm_val == 0) direction = 1;   // 暗到头了，该变亮了
        }

        // 2. 写入寄存器 (假设用的是 TIM3 的通道2)
        TIM_SetCompare2(TIM3, pwm_val);
    }
}

void LED_STOPBreath(void){

        TIM_SetCompare2(TIM3, 0);

}

