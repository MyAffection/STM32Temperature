#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Store.h"

uint8_t Write_Index = 0;
void Add_New_Log(uint16_t new_temp){
	//先移动数据，再添加数据
   
	for(uint16_t i = 4; i > 0 ;i--){
		sys_params.Log_Temp[i] = sys_params.Log_Temp[i - 1];
	}
		 sys_params.Log_Temp[0] = new_temp; 

if(sys_params.Log_Count < 5){
	
sys_params.Log_Count ++;
	
}



Flash_Save_Params();

}




void Show_History_Logs(void) {
    if(sys_params.Log_Count == 0){
        return; 
    }

    for(uint16_t i = 0; i < sys_params.Log_Count; i++){ 
	 OLED_ShowNum(i + 1, 1, sys_params.Log_Temp[i], 2);
		}
		
}







