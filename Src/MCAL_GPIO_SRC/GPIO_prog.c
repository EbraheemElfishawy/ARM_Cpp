
/*	@file 		 : STM32F44xx.h
*	@author      : Ebraheem Elfeshawy
*	@brief		 : GPIO main source file, including function definitions
************************************************************************/

#include <stdio.h>
#include<stdlib.h>
#include "../../Inc/LIB/BIT_MATH.h"
#include"../../Inc/MCAL_GPIO_INC/GPIO_interface.h"


static GPIO_RegDef_t* GPIO_u8Port[MAX_GPIO_PERIPHERALS]= {GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH};

/***************************************************************************/
/* @fn		GPIO_u8PinInit
 * @brief	the function  initialize the GPIO pin according to the input parameter
 * @param[in]	PinConfig, the initialization values of the pin
 * @retval	Errorstatus
 */
uint8_t GPIO_u8PinInit(PinConfig_t* PinConfig)
{
	uint8_t Local_u8ErrorState = OK;
	if(PinConfig != NULL)
	{
		if((PinConfig->Port  <= MAX_GPIO_PERIPHERALS) && (PinConfig->PinNum <= PIN15))
		{
			/*1- Select GPIO Mode Input, Output, Analog, Alternate Function*/
			(GPIO_u8Port[PinConfig->Port])->MODER &= ~(MODER_MASK << (PinConfig->PinNum)*MODER_PIN_ACCESS ); //clear MODER pins
			(GPIO_u8Port[PinConfig->Port])->MODER |= ((PinConfig->Mode) << (PinConfig->PinNum)*MODER_PIN_ACCESS);

			/*2- Select GPIO Pull State: Pullup, Pulldown, Nopull state*/
			(GPIO_u8Port[PinConfig->Port])->PUPDR &= ~(PUPDR_MASK << (PinConfig->PinNum)*PUPDR_PIN_ACCESS ); //clear MODER pins
			(GPIO_u8Port[PinConfig->Port])->PUPDR |= ((PinConfig->PullUpDown) << (PinConfig->PinNum)*PUPDR_PIN_ACCESS);

			/*3- Select Output Type, Output speed in case of general purpose output mode*/
			if(PinConfig->Mode == OUTPUT || PinConfig->Mode == ALTERNATE_FUNCTION)
			{
				/*Select output type: Push-pull, Output drain*/
				(GPIO_u8Port[PinConfig->Port])->OTYPER  &= ~(OTYPER_MASK << (PinConfig->PinNum)*OTYPER_PIN_ACCESS ); //clear MODER pins
				(GPIO_u8Port[PinConfig->Port])->OTYPER  |= ((PinConfig->PullUpDown) << (PinConfig->PinNum)*OTYPER_PIN_ACCESS);

				/*Select output speed: Low, Medium, High, Very High*/
				(GPIO_u8Port[PinConfig->Port])->OSPEEDR &= ~(OSPEEDR_MASK << PinConfig->PinNum ); //clear MODER pins
				(GPIO_u8Port[PinConfig->Port])->OSPEEDR |= ((PinConfig->Speed) << (PinConfig->PinNum)*OSPEEDR_PIN_ACCESS);

				/*Select The Pin Alternate Function*/
				if (PinConfig->Mode == ALTERNATE_FUNCTION)
				{
					uint8_t Local_u8RegNum = (PinConfig->PinNum)/8;
					uint8_t Local_u8PinNum = (PinConfig->PinNum)%8;

					(GPIO_u8Port[PinConfig->Port])->AFR[Local_u8RegNum] &= ~(AFR_MASK << Local_u8PinNum *AFR_PIN_ACCESS); //clear MODER pins
					(GPIO_u8Port[PinConfig->Port])->AFR[Local_u8RegNum] |= ((PinConfig->AltFunc) << (Local_u8PinNum * AFR_PIN_ACCESS));
				}
			}
		}
		else
		{
			Local_u8ErrorState = NOK;
		}

	}
	else
	{
		Local_u8ErrorState = NULL_PTR;
	}


	return Local_u8ErrorState;
}

/*************************************************************************************************************/
/* @fn		GPIO_u8SetPinValue
 * @brief	the function outputs a certain vlue on an output pin
 * @param[in]	Copy_u8Port, the port number, get options @Port_t enum
 * @param[in]	Copy_u8PinNum, the pin number, get options @Pin_t enum
 * @param[in]	Copy_u8Value, the output value, get options @Val_t enum
 * @retval	Errorstatus
 */
uint8_t GPIO_u8SetPinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum, Val_t Copy_u8Value)
{
	uint8_t Local_u8ErrorState = OK;

	if((Copy_u8Port <= PORTH) && (Copy_u8PinNum <= PIN15))
	{
		if (Copy_u8Value == LOWVAL)
		{
			(GPIO_u8Port[Copy_u8Port])->ODR &= ~(1<<Copy_u8PinNum);
		}
		else if(Copy_u8Value == HIGHVAL)
		{
			(GPIO_u8Port[Copy_u8Port])->ODR |= (1<<Copy_u8PinNum);
		}
	}
	else
	{
		Local_u8ErrorState = NOK;
	}
	return Local_u8ErrorState;
}

uint8_t GPIO_u8GetPinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum, Val_t *Copy_u8Value)
{
	uint8_t Local_u8ErrorState = OK;

	if((Copy_u8Port <= PORTH) && (Copy_u8PinNum <= PIN15))
	{
		*Copy_u8Value = (GPIO_u8Port[Copy_u8Port]->IDR >> Copy_u8PinNum) & 0x01;
	}
	else
	{
		Local_u8ErrorState = NOK;
	}
	return Local_u8ErrorState;
}

uint8_t GPIO_u8TogglePinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum)
{
	uint8_t Local_u8ErrorState = OK;

	if((Copy_u8Port <= PORTH) && (Copy_u8PinNum <= PIN15))
	{
		(GPIO_u8Port[Copy_u8Port])->ODR ^= (1<<Copy_u8PinNum);
	}
	else
	{
		Local_u8ErrorState = NOK;
	}
	return Local_u8ErrorState;
}
/**********************************************************************
 * @fn        : GPIO_u8PinInit
 * @breief    : This function intializes the GPIO pin according to the input parameter
 * @param[in] : PinConfig, the initialization values of the pin
 * @retval	  : Errorstatus
 */
/*
void GPIO_u8PinInit(const PinConfig_t* PinConfig)
{
	GPIO_RegDef_t *GPIO_Ptr ;
	switch(PinConfig->Port)
	{
		case PORTA: GPIO_Ptr= GPIOA;break;
		case PORTB: GPIO_Ptr= GPIOB;break;
		case PORTC: GPIO_Ptr= GPIOC;break;
		case PORTD: GPIO_Ptr= GPIOD;break;
		case PORTE: GPIO_Ptr= GPIOE;break;
		case PORTF: GPIO_Ptr= GPIOF;break;
		case PORTG: GPIO_Ptr= GPIOG;break;
		case PORTH: GPIO_Ptr= GPIOH;break;
		default:break;
	}
	switch(PinConfig->Mode)
	{
		case INPUT:
			GPIO_Ptr->MODER &=~(3<< (2*(PinConfig->PinNum)) );break;
		case OUTPUT:
			GPIO_Ptr->MODER |= (1<< (2*(PinConfig->PinNum)) );break;
		case ALTERNATE_FUNCTION:
			GPIO_Ptr->MODER |= (2<< (2*(PinConfig->PinNum)) );
			if(PinConfig->PinNum <=7)//pins from 0:7
			{
				//Write on low register
				GPIO_Ptr->AFR[0] |= ( (PinConfig->AltFunc)<<(4*(PinConfig->PinNum)) );
			}
			else if(PinConfig->PinNum >7)//pins from 8:15
			{
				//Write on high register //pins from 8:15 but subtract 8 from every pin
				GPIO_Ptr->AFR[1] |= ( ((PinConfig->AltFunc)-8)<<(4*(PinConfig->PinNum)) );
			}
			break;
		case ANALOG://0x11
			GPIO_Ptr->MODER |= (3<< (2*(PinConfig->PinNum)) );
			break;
	}
	switch (PinConfig->Speed) {
			case LOW:
				//GPIO_Ptr->OSPEEDR &=~(0<< (2*(PinConfig->PinNum)) );
				break;
			case MEDIUM:
				GPIO_Ptr->OSPEEDR |= (1<< (2*(PinConfig->PinNum)) );
				break;
			case FAST:
				GPIO_Ptr->OSPEEDR |= (2<< (2*(PinConfig->PinNum)) );
			break;
			case HIGH:
				GPIO_Ptr->OSPEEDR |= (3<< (2*(PinConfig->PinNum)) );
			break;
			default:break;
				}
	switch (PinConfig->Type) {
			case PUSH_PULL:
				GPIO_Ptr->OTYPER &=~ (1<< (PinConfig->PinNum) );
				break;
			case OPEN_DRAIN:
				//GPIO_Ptr->OTYPER |= (1<< (PinConfig->PinNum) );
				break;
			default:break;
		}
	/// choose pullup or pulldown
	switch (PinConfig->PullUpDown)
	{
		case NOPULL:
			GPIO_Ptr->PUPDR &=~ (3<< (2*(PinConfig->PinNum)) );
			break;
		case PULLUP:
			GPIO_Ptr->PUPDR |= (1<< (2*(PinConfig->PinNum)) );
			break;
		case PULLDOWN:
			//GPIO_Ptr->PUPDR |= (2<< (2*(PinConfig->PinNum)) );
			break;
		default:break;
	}
}
*/
/*
void GPIO_SetPinValue(uint8_t uint8_CopyPortNumber, uint8_t uint8_CopyPinNumber,uint8_t uint8_CopyPinValue)
{
	GPIO_RegDef_t *GPIO_DataPtr ;
		switch(uint8_CopyPortNumber)
		{
			case PORTA: GPIO_DataPtr= GPIOA;break;
			case PORTB: GPIO_DataPtr= GPIOB;break;
			case PORTC: GPIO_DataPtr= GPIOC;break;
			case PORTD: GPIO_DataPtr= GPIOD;break;
			case PORTE: GPIO_DataPtr= GPIOE;break;
			case PORTF: GPIO_DataPtr= GPIOF;break;
			case PORTG: GPIO_DataPtr= GPIOG;break;
			case PORTH: GPIO_DataPtr= GPIOH;break;
			default:break;
		}
		if(uint8_CopyPinValue == 1)
		{
			SET_BIT((GPIO_DataPtr->ODR),uint8_CopyPinNumber);
		}
		else if (uint8_CopyPinValue == 0)
		{
			CLR_BIT((GPIO_DataPtr->ODR),uint8_CopyPinNumber);
		}
}
*/
/*
uint8_t GPIO_u8GetPinValue(uint8_t uint8_CopyPortNumber, uint8_t uint8_CopyPinNumber)
{
	GPIO_RegDef_t *GPIO_DataPtr ;
		switch(uint8_CopyPortNumber)
		{
			case PORTA: GPIO_DataPtr= GPIOA;break;
			case PORTB: GPIO_DataPtr= GPIOB;break;
			case PORTC: GPIO_DataPtr= GPIOC;break;
			case PORTD: GPIO_DataPtr= GPIOD;break;
			case PORTE: GPIO_DataPtr= GPIOE;break;
			case PORTF: GPIO_DataPtr= GPIOF;break;
			case PORTG: GPIO_DataPtr= GPIOG;break;
			case PORTH: GPIO_DataPtr= GPIOH;break;
			default:break;
		}
		return GET_BIT((GPIO_DataPtr->IDR),uint8_CopyPinNumber);
}
*/
