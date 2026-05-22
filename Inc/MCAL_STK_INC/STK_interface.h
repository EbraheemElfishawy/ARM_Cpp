/*
 * STK_interface.h
 *
 *  Created on: Mar 10, 2022
 *      Author: Ebraheem El-Feshawy
 */

#ifndef MCAL_STK_INC_STK_INTERFACE_H_
#define MCAL_STK_INC_STK_INTERFACE_H_
#include "STK_config.h"
#include "STK_private.h"
#include "STK_register.h"
#include "../LIB/STM32F446xx.h"
typedef enum
{
	AHB_8,
	AHB
}STK_Source_t;
typedef enum
{
	TICKINT_ENABLED,
	TICKINT_DISABLED
}STK_Interrupt_t;
typedef struct
{
	STK_Source_t      ClockSource;
	STK_Interrupt_t   SysTickInterrupt;
}SysTickConfig_t;
typedef enum
{
	COUNTER_ENABLED,
	COUNTER_DISABLED
}STK_Counter_t;

uint8_t SysTick_Init(SysTickConfig_t *SysTickConfigs );
uint8_t SysTick_Start(uint32_t u32CopyValue_ms );
uint8_t SysTick_Delay_ms(uint32_t u32CopyDelayValue_ms );
void SysTick_voidSetCallBack(void (*Copy_Ptr)(void));
//void SysTick_Handler(void);

#endif /* MCAL_STK_INC_STK_INTERFACE_H_ */
