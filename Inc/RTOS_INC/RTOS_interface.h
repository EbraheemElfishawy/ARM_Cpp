/*
 * RTOS_interface.h
 *
 *  Created on: Apr 25, 2022
 *      Author: Ebraheem El-Feshawy
 */

#ifndef RTOS_INC_RTOS_INTERFACE_H_
#define RTOS_INC_RTOS_INTERFACE_H_
#include "RTOS_config.h"
#include "RTOS_private.h"

void RTOS_Start(void);

void RTOS_Scheduler(void);

void RTOS_CreateTask(uint8_t Copy_u8Priority, uint16_t Copy_u16Periodicity, void(*Copy_pvTask)(void));


#endif /* RTOS_INC_RTOS_INTERFACE_H_ */
