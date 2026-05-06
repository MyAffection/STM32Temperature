#ifndef __LED_H
#define __LED_H
//开灯 GPIOA1和GPIOA2作为信息灯 
void Safe_LED_ON(void);
void Safe_LED_OFF(void);
void Warring_LED_ON(void);
void Warring_LED_OFF(void);

void Danger_LED_ON(void);
void Danger_LED_OFF(void);

void LED_Init(void);
void LED_PWM(void);


//void Danger_LED_ON(void);

//void Danger_LED_OFF(void);
void LED_Breath(void);
void LED_STOPBreath(void);



#endif
