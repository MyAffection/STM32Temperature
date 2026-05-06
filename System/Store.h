#ifndef __Store_H
#define __Store_H

#include "stm32f10x.h"
#define STORE_ADDRESS  0x0800FC00

typedef struct {
    uint16_t High_Temp;   
	uint16_t Log_Count;
	uint16_t Log_Temp[5];
} Sys_Params_t;

extern Sys_Params_t sys_params;
void Store_Read_Params(void);
void Flash_Save_Params(void);


#endif
