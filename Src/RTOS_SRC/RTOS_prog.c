/*
 * RTOS_prog.c
 *
 *  Created on: Apr 25, 2022
 *      Author: Ebraheem El-Feshawy
 */
#include "../../Inc/LIB/STM32F446xx.h"
#include "../../Inc/LIB/STD_TYPES.h"

#include "../../Inc/MCAL_STK_INC/STK_interface.h"

#include "../../Inc/RTOS_INC/RTOS_interface.h"

#define NULL 0
static Task SysTasks[TASKS_NUMBER]={{NULL}};
static SysTickConfig_t SysTickConfigs = {AHB_8,TICKINT_ENABLED};
void RTOS_Start(void)
{
	SysTick_Init(&SysTickConfigs);
	SysTick_voidSetCallBack(&RTOS_Scheduler);
	SysTick_Start(1000);

}

void RTOS_Scheduler(void)
{
	static uint16_t Local_u16TickCounter=0;
	uint8_t Local_u8TaskCounter;
	/** increment of scheduler tick counter */
	Local_u16TickCounter++;
	for(Local_u8TaskCounter=0;Local_u8TaskCounter<TASKS_NUMBER;Local_u8TaskCounter++)
	{
		if(Local_u16TickCounter % SysTasks[Local_u8TaskCounter].Periodicity)
		{
			/*invoking task function*/
			SysTasks[Local_u8TaskCounter].TaskHandler();
		}
	}
}

void RTOS_CreateTask(uint8_t Copy_u8Priority, uint16_t Copy_u16Periodicity, void(*Copy_pvTask)(void))
{
	SysTasks[Copy_u8Priority].Periodicity= Copy_u16Periodicity;
	SysTasks[Copy_u8Priority].TaskHandler= Copy_pvTask;
}



