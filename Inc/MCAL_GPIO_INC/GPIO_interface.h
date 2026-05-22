/*******************************************************************************/
/**************											    ********************/
/**************		Author : Ebraheem El-Feshawy		    ********************/
/**************		Date   : 26-2-2022					    ********************/
/**************		Version: 1.00						    ********************/
/**************		Brief  : RGPIO main header file         ********************/
/**************				 						 		********************/
/*******************************************************************************/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_
#include"../../Inc/LIB/STM32F446xx.h"
#include "GPIO_private.h"
#define MAX_GPIO_PERIPHERALS     8

typedef enum
{
	PORTA=0,
	PORTB,
	PORTC,
	PORTD,
	PORTE,
	PORTF,
	PORTG,
	PORTH
}Port_t;

typedef enum
{
	PIN0=0,
	PIN1,
	PIN2,
	PIN3,
	PIN4,
	PIN5,
	PIN6,
	PIN7,
	PIN8,
	PIN9,
	PIN10,
	PIN11,
	PIN12,
	PIN13,
	PIN14,
	PIN15
}Pin_t;

typedef enum
{
	INPUT=0,
	OUTPUT,
	ALTERNATE_FUNCTION,
	ANALOG
}Mode_t;// p187

typedef enum
{
	LOW=0,
	MEDIUM,
	FAST,
	HIGH
}OutputSpeed_t;//p187

typedef enum
{
	PUSH_PULL=0,
	OPEN_DRAIN
}OutputType_t;//p188

typedef enum
{
	NOPULL=0,
	PULLUP,
	PULLDOWN
}PullUpDown_t;//p189

typedef enum
{
	AF0=0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15
}AltFunc_t;//p189
typedef enum
{
	LOWVAL,
	HIGHVAL
}Val_t;

typedef struct
{
	Port_t        Port;// PORTA --> PORTH
	Pin_t         PinNum;// 0 --> 15
	Mode_t        Mode;//INPUT/OUTPUT/ALTER/ANALOG
	OutputSpeed_t Speed;//LOW=0,MEDIUM,FAST,HIGH
	OutputType_t  Type;//PUSH_PULL, OPEN_DRAIN
	PullUpDown_t  PullUpDown;//NOPULL=0,PULLUP,PULLDOWN
	AltFunc_t     AltFunc;//AF0 --> AF15
}PinConfig_t;
typedef struct
{
	Port_t        Port;
	Pin_t         PinNum;
}PinValueConfig_t;

/**********************************************************************
 * @fn        : GPIO_u8PinInit
 * @breief    : This function intializes the GPIO pin according to the input parameter
 * @param[in] : PinConfig, the initialization values of the pin
 * @retval	  : Errorstatus
 */
uint8_t GPIO_u8PinInit(PinConfig_t* PinConfig);
/*************************************************************************************************************/
/* @fn		GPIO_u8SetPinValue
 * @brief	the function outputs a certain vlue on an output pin
 * @param[in]	Copy_u8Port, the port number, get options @Port_t enum
 * @param[in]	Copy_u8PinNum, the pin number, get options @Pin_t enum
 * @param[in]	Copy_u8Value, the output value, get options @Val_t enum
 * @retval	Errorstatus
 */
uint8_t GPIO_u8SetPinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum, Val_t Copy_u8Value);

uint8_t GPIO_u8GetPinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum, Val_t *Copy_u8Value);

uint8_t GPIO_u8TogglePinValue(Port_t Copy_u8Port, Pin_t Copy_u8PinNum);

/******
void GPIO_SetPinValue(uint8_t uint8_CopyPortNumber, uint8_t uint8_CopyPinNumber,uint8_t uint8_CopyPinValue);
uint8_t GPIO_u8GetPinValue(uint8_t uint8_CopyPortNumber, uint8_t uint8_CopyPinNumber);//uint8_t GPIO_u8GTogglePinValue(parameter list);
******/

#endif
