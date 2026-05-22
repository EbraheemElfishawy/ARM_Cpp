/*
 * RCC_prog.c
 *
 *  Created on: Feb 28, 2022
 *      Author: Ebraheem El-Feshawy
 */
#include <stdio.h>
#include<stdlib.h>
#include"../../Inc/LIB/STM32F446xx.h"
#include "../../Inc/LIB/BIT_MATH.h"
#include "../../Inc/MCAL_RCC_INC/RCC_config.h"
#include "../../Inc/MCAL_RCC_INC/RCC_private.h"
#include "../../Inc/MCAL_RCC_INC/RCC_interface.h"

void RCC_Set_HSI(const RCC_HSI_Config_t* RCC_HSI_Config)
{
	RCC_RegDef_t *RCC_Ptr = RCC;
	switch(RCC_HSI_Config->HSI_State)
		{
			case HSION:
				RCC_Ptr->RCC_CR |=(1<<0);
			break;
			case HSIOFF:
				RCC_Ptr->RCC_CR &=~(1<<0);
			break;
			default:break;
		}
	while(GET_BIT( (RCC_Ptr->RCC_CR),1 )==0 );
}
void RCC_Set_HSE(const RCC_HSE_Config_t* RCC_HSE_Config)
{
	RCC_RegDef_t *RCC_Ptr = RCC;
	switch(RCC_HSE_Config->HSE_State)
		{
			case HSEON:

				switch(RCC_HSE_Config->HSEBYP_State)
				{
				case HSEBYPON:
					RCC_Ptr->RCC_CR |=(1<<18);
					break;
				case HSEBYPOFF:
					RCC_Ptr->RCC_CR |=(1<<18);
					break;
				default:break;
				}
				RCC_Ptr->RCC_CR |=(1<<16);
			break;
			case HSEOFF:
				RCC_Ptr->RCC_CR &=~(1<<16);
			break;
			default:break;
		}
	while(GET_BIT( (RCC_Ptr->RCC_CR),17 )==0 );
}
void RCC_Set_PLL  (const RCC_PLL_Config_t* RCC_PLL_Config)
{

}
void RCC_GPIO_Init(const RCC_GPIO_Config_t* RCC_GPIO_Config)
{
	RCC_RegDef_t *RCC_Ptr = RCC;
	switch(RCC_GPIO_Config->GPIOA_State)
	{
		case GPIOA_ENABLED:
			RCC_Ptr->RCC_AHB1ENR |=(1<<0);
		break;
		case GPIOA_DISABLED:
			RCC_Ptr->RCC_AHB1ENR &=~(1<<0);
		break;
		default:break;
	}
	switch(RCC_GPIO_Config->GPIOB_State)
		{
			case GPIOB_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<1);
			break;
			case GPIOB_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<1);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOC_State)
		{
			case GPIOC_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<2);
			break;
			case GPIOC_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<2);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOD_State)
		{
			case GPIOD_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<3);
			break;
			case GPIOD_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<3);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOE_State)
		{
			case GPIOE_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<4);
			break;
			case GPIOE_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<4);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOF_State)
		{
			case GPIOF_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<5);
			break;
			case GPIOF_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<5);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOG_State)
		{
			case GPIOG_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<6);
			break;
			case GPIOG_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<6);
			break;
			default:break;
		}
	switch(RCC_GPIO_Config->GPIOH_State)
		{
			case GPIOH_ENABLED:
				RCC_Ptr->RCC_AHB1ENR |=(1<<7);
			break;
			case GPIOH_DISABLED:
				RCC_Ptr->RCC_AHB1ENR &=~(1<<7);
			break;
			default:break;
		}

}
