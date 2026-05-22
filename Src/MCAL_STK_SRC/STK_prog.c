/*
 * STK_prog.c
 *
 *  Created on: Mar 10, 2022
 *      Author: Ebraheem El-Feshawy
 */
/* from Cortex -M4 Devices Generic User Guide chapter 4.4 */
#include "../../Inc/MCAL_STK_INC/STK_interface.h"

void (*Systick_CallBack)(void)=NULL;

uint8_t SysTick_Init(SysTickConfig_t *SysTickConfigs)
{
	SYSTICK_RegDef_t *STK_Ptr = SYSTICK;
	uint8_t Local_u8ErrorState = OK;
	//STK_Ptr ->STK_CTRL= 0x00000002;

	if(SysTickConfigs != NULL)
	{
		//Checking Clock Source
		if(SysTickConfigs->ClockSource == AHB_8)
		{
			STK_Ptr ->STK_CTRL &=~(STCR_MASK<<STCR_CLKSOURCE_PIN_ACCESS);
		}
		else if(SysTickConfigs->ClockSource == AHB)
		{
			STK_Ptr ->STK_CTRL |= (STCR_MASK<<STCR_CLKSOURCE_PIN_ACCESS);
		}
		//Checking Interrupt State
		if(SysTickConfigs->SysTickInterrupt == TICKINT_DISABLED)
		{
			STK_Ptr ->STK_CTRL &=~(STCR_MASK<<STCR_TICKINT_PIN_ACCESS);
		}
		else if(SysTickConfigs->SysTickInterrupt == TICKINT_ENABLED)
		{
			STK_Ptr ->STK_CTRL |= (STCR_MASK<<STCR_TICKINT_PIN_ACCESS);
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR;
	}
	return Local_u8ErrorState;
}
uint8_t SysTick_Start(uint32_t u32CopyValue_ms )
{
	SYSTICK_RegDef_t *STK_Ptr = SYSTICK;
	//uint32_t ReloadValue = 16777216UL*(1000- ((u32CopyDelayValue_ms/16000000UL)*(16777216UL);
	uint8_t Local_u8ErrorState = OK;
	//if(ReloadValue<0x00FFFFFF)
	//{
		/*Disable counter*/
		//STK_Ptr->STK_CTRL &=~ (STCR_MASK<<STCR_COUNTER_PIN_ACCESS);
		/*Put Value in STK Load register*/
		STK_Ptr->STK_LOAD = u32CopyValue_ms;
		/*STK counter = 0*/
		STK_Ptr->STK_VAL=0;
		/*Enable counter*/
		STK_Ptr->STK_CTRL |= (STCR_MASK<<STCR_COUNTER_PIN_ACCESS);// To start counting down
		/*Wait untill STK_VAL reaches (ReloadValue -1)*/
		//while( (STK_Ptr->STK_VAL)!=0 );
	//}
	//else
	//{
	//	Local_u8ErrorState = NULL;
	//}
	//return Local_u8ErrorState;
}

uint8_t SysTick_Delay_ms(uint32_t u32CopyDelayValue_ms )
{
	SYSTICK_RegDef_t *STK_Ptr = SYSTICK;
	//uint32_t ReloadValue = 16777216UL*(1000- ((u32CopyDelayValue_ms/16000000UL)*(16777216UL);
	uint8_t Local_u8ErrorState = OK;
	//if(ReloadValue<0x00FFFFFF)
	//{
		/*Disable counter*/
		STK_Ptr->STK_CTRL &=~ (STCR_MASK<<STCR_COUNTER_PIN_ACCESS);
		/*Put Value in STK Load register*/
		STK_Ptr->STK_LOAD = u32CopyDelayValue_ms;
		/*STK counter = 0*/
		STK_Ptr->STK_VAL=0;
		/*Enable counter*/
		STK_Ptr->STK_CTRL |= (STCR_MASK<<STCR_COUNTER_PIN_ACCESS);
		/*Wait until STCVR reaches (0)*/
		while( (STK_Ptr->STK_VAL)!=0 );
	//}
	//else
	//{
	//	Local_u8ErrorState = NULL;
	//}
	//return Local_u8ErrorState;
}

void SysTick_voidSetCallBack(void (*Copy_Ptr)(void))
{
	if(Copy_Ptr!=NULL)
	{
		Systick_CallBack= Copy_Ptr;
	}
}

/*ISR of systick interrupt*/
void SysTick_Handler(void)
{
	if(Systick_CallBack!=NULL)
	{
		Systick_CallBack();
	}
}
