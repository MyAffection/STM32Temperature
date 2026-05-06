#include "stm32f10x.h"                  // Device header
#include "Store.h"

#define STORE_ADDRESS 0x0800FC00

Sys_Params_t sys_params;


//保存数据
void Flash_Save_Params(void) {
	
	//首要计算我们写入的次数
    uint16_t *pRam = (uint16_t *)&sys_params; 
    uint16_t num_of_halfwords = sizeof(Sys_Params_t) / 2; 
    __disable_irq();  //屏蔽中断
    FLASH_Unlock();//解锁
   /*                         完成标志       编程错误        保护错误*/
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); //清除所有标志
    FLASH_ErasePage(STORE_ADDRESS); //擦除
    for(uint16_t i = 0; i < num_of_halfwords; i++) {
		
        FLASH_ProgramHalfWord(STORE_ADDRESS + i * 2, pRam[i]);//把结构体的值写入FLash
    }
    FLASH_Lock();  //上锁
    __enable_irq();   
	
}


void Store_Read_Params(void){
//解剖结构体
	uint16_t *pRam = (uint16_t *)(&sys_params);
	uint16_t num_of_halfwords = sizeof(Sys_Params_t) / 2;  //这就是for循环的条件
	
	
	
	for(uint16_t i = 0 ; i < num_of_halfwords; i++){	
	
	pRam[i] = *(__IO uint16_t *)(STORE_ADDRESS + i *2);
			
	}
	if(sys_params.High_Temp == 0xFFFF){
	
	sys_params.High_Temp = 28;
		
		sys_params.Log_Count = 0;
		
		for(int i = 0; i < 5; i++) {
			
			sys_params.Log_Temp[i] = 0;
		
		}
	
	Flash_Save_Params();
	
	}

}
