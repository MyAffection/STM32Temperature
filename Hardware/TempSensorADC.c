#include "stm32f10x.h"                  // Device header
#include <math.h>
//热名传感器测值
void TempSensorADC_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE);
	GPIO_InitTypeDef GPIO_Initstruct;
	GPIO_Initstruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Initstruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //至少进行6分频
	
	ADC_InitTypeDef ADC_InitStruct;

	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;  //使用硬件触发
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);

    ADC_ExternalTrigConvCmd(ADC1,ENABLE);

	ADC_Cmd(ADC1,ENABLE);
	
	//校准值
	 ADC_ResetCalibration(ADC1);
     while(ADC_GetResetCalibrationStatus(ADC1));
     ADC_StartCalibration(ADC1);
     while(ADC_GetCalibrationStatus(ADC1));
 


}

// 获取数值函数 (核心修正)
uint16_t Get_TempSensorADCValue(void){
    
    uint32_t TempADC = 0; // 【修正1】必须初始化为0，且换成32位防止溢出
    uint16_t ResultValue;
    
    for (uint8_t i = 0 ; i < 10; i++){
        // 【修正2】把等待语句放进循环里。
        // 死等，直到 TIM3 触发了 ADC 并且转换完成
        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
        
        // 拿到数据并累加。
        // (注：读取 ADC_DR 寄存器这个动作，在硬件底层会自动把 EOC 标志位清零。
        // 所以下一次循环时，while 又能卡住，继续等下一个 TIM3 触发)
        TempADC += ADC_GetConversionValue(ADC1); 
    }
    
    ResultValue = TempADC / 10;
    return ResultValue;
}

//ADC转换为电压值，注意这里是NTC的电压值
float Get_Voltage(void){
	uint16_t TempADC = Get_TempSensorADCValue();


  return (float)TempADC / 3750.0f * 3.3f;


}
//电压值转化为电阻值，利用串联分压原理，总电压和等于多个电压之和，电流处处相等。

float Get_RNTC(void){
	float TempVoltage = Get_Voltage();
    float RNTC ;
    RNTC = 10000.0f * TempVoltage / (3.3f - TempVoltage);   
	return RNTC;

}
//B值获取参数  公式 ：R = R0 × e^(B(1/T - 1/T0)) 或者 1 / T = (1 / T0)+(1/B0)*ln(R/R0)
float Get_Temperaure(void){
float RNTC =   Get_RNTC();
	
static float last_Temp = 0;
static uint8_t is_first_time = 1; // 标记是不是刚开机
	
float Temperature = 1.0f / ((1.0f / 298.15f) +  (1.0f / 3950.0f)*log(RNTC / 10000));

	float Raw_Temp = Temperature -273.15f;
	if(is_first_time == 1){
	
	last_Temp = Raw_Temp;
		
	is_first_time = 0;
	return last_Temp;
	
	}

	//核心滤波公式 这里的 0.05 就是滤波系数，越小越平滑，但反应越慢。你可以自己改！
	last_Temp = (0.05f * Raw_Temp) + (0.95f * last_Temp);
	

    return last_Temp;
	

}














