/*
 * RCC_interface.h
 *
 *  Created on: Feb 28, 2022
 *      Author: Ebraheem El-Feshawy
 */

#ifndef MCAL_RCC_INC_RCC_INTERFACE_H_
#define MCAL_RCC_INC_RCC_INTERFACE_H_
/***** RCC_CR options ****/
typedef enum
{
	PLLSAION,
	PLLSAIOFF,
	OTHERS0
}RCC_PLLSAI_t;
typedef enum
{

	PLLI2SON,
	PLLI2SOFF,
	OTHERS1
}RCC_PLLI2S_t;
typedef enum
{
	PLLON,
	PLLOFF,
	OTHERS2
}RCC_PLL_t;
typedef enum
{
	CSSON,
	CSSOFF
}RCC_CSS_t;
typedef enum
{
	HSEON,
	HSEOFF,
	HSEBYPON,
	HSEBYPOFF
}RCC_HSE_t;
typedef enum
{
	HSION,
	HSIOFF,
	OTHERS
}RCC_HSI_t;
/**************/
/***** RCC_PLLCFGR options ****/

/**************/

/***** RCC_CFGR options ****/

/**************/

/***** RCC_AHB1ENR options ****/
/*** GPIO A:H ***/
typedef enum
{
	GPIOA_ENABLED,
	GPIOA_DISABLED,
	GPIOB_ENABLED,
	GPIOB_DISABLED,
	GPIOC_ENABLED,
	GPIOC_DISABLED,
	GPIOD_ENABLED,
	GPIOD_DISABLED,
	GPIOE_ENABLED,
	GPIOE_DISABLED,
	GPIOF_ENABLED,
	GPIOF_DISABLED,
	GPIOG_ENABLED,
	GPIOG_DISABLED,
	GPIOH_ENABLED,
	GPIOH_DISABLED
}GPIO_Control_t;
/**************/
/*
 *
 *
 *
 */
typedef struct
{
	RCC_HSI_t HSI_State;
}RCC_HSI_Config_t;
typedef struct
{
	RCC_HSE_t HSE_State;
	RCC_HSE_t HSEBYP_State;
}RCC_HSE_Config_t;
typedef struct
{
	RCC_PLL_t PLL_State;
	RCC_PLL_t PLL_Choice;
}RCC_PLL_Config_t;
typedef struct
{
	GPIO_Control_t        GPIOA_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOB_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOC_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOD_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOE_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOF_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOG_State;// GPIO_ENABLED/GPIO_DESABLED
	GPIO_Control_t        GPIOH_State;// GPIO_ENABLED/GPIO_DESABLED
}RCC_GPIO_Config_t;



void RCC_Set_HSI  (const RCC_HSI_Config_t* RCC_HSI_Config);
void RCC_Set_HSE  (const RCC_HSE_Config_t* RCC_HSE_Config);
void RCC_Set_PLL  (const RCC_PLL_Config_t* RCC_PLL_Config);
void RCC_GPIO_Init(const RCC_GPIO_Config_t* RCC_GPIO_Config);




#endif /* MCAL_RCC_INC_RCC_INTERFACE_H_ */
