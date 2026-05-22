
#include "../../Inc/MCAL_NVIC_INC/NVIC_interface.h"
#include "../../Inc/MCAL_NVIC_INC/NVIC_register.h"

#define OK		0
#define NOK		1
#define NULL	0
#define NULL_POINTER 2

void    NVIC_VoidEnablePeripheral  ( uint8_t Copy_uint8_tINTID )
{
	NVIC->NVIC_ISER[ Copy_uint8_tINTID / 32 ] = 1 << ( Copy_uint8_tINTID % 32 ) ;
}

void NVIC_VoidDisablePeripheral ( uint8_t Copy_uint8_tINTID ){

	NVIC->NVIC_ICER[ Copy_uint8_tINTID / 32 ] = 1 << ( Copy_uint8_tINTID % 32 ) ;
}
void NVIC_VoidSetPending ( uint8_t Copy_uint8_tINTID ){

	NVIC->NVIC_ISPR[ Copy_uint8_tINTID / 32 ] = 1 << ( Copy_uint8_tINTID % 32 ) ;
}
void NVIC_VoidClearPending ( uint8_t Copy_uint8_tINTID ){

	NVIC->NVIC_ICPR[ Copy_uint8_tINTID / 32 ] = 1 << ( Copy_uint8_tINTID % 32 ) ;
}
uint8_t   NVIC_uint8_tGetActive( uint8_t Copy_uint8_tINTID ){

	uint8_t LOC_uint8_tActive = GET_BIT( (NVIC->NVIC_IABR[ Copy_uint8_tINTID/32 ]) , ( Copy_uint8_tINTID % 32 ) );
	return  LOC_uint8_tActive ;
}
void NVIC_VidSetPriority( int8_t Copy_uint8_tINTID , uint8_t Copy_uint8_tGroupPriority , uint8_t Copy_uint8_tSubPriority , uint32_t Copy_uint8_tGroup )
{
	//page 229 in f446 programming manual
	uint8_t Priority =  Copy_uint8_tSubPriority | Copy_uint8_tGroupPriority << ( (Copy_uint8_tGroup - 0x05FA0300) / 256 )  ;
	//Core Peripheral
	if( Copy_uint8_tINTID < 0  )
	{
		if( Copy_uint8_tINTID == MEMORY_MANAGE || Copy_uint8_tINTID == BUS_FAULT || Copy_uint8_tINTID == USAGE_FAULT )
		{
			Copy_uint8_tINTID += 3;
			SCB->SCB_SHPR1 = ( Priority ) << ( (8 * Copy_uint8_tINTID) + 4 );
		}
		else if ( Copy_uint8_tINTID == SV_CALL )
		{
			Copy_uint8_tINTID += 7;
			SCB->SCB_SHPR2 = ( Priority ) << ( (8 * Copy_uint8_tINTID) + 4 );
		}
		else if( Copy_uint8_tINTID == PEND_SV || Copy_uint8_tINTID == SYSTICK )
		{
			Copy_uint8_tINTID += 8;
			SCB->SCB_SHPR3 = ( Priority ) << ( (8 * Copy_uint8_tINTID) + 4 );
		}
	}
    //External Peripheral
	else if( Copy_uint8_tINTID >= 0 )
	{
		NVIC->NVIC_IPR[ Copy_uint8_tINTID ] = Priority << 4 ;
	}
	SCB->SCB_AIRCR = Copy_uint8_tGroup ;
}
