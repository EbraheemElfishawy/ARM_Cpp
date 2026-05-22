/*
 * RTOS_private.h
 *
 *  Created on: Apr 25, 2022
 *      Author: Ebraheem El-Feshawy
 */

#ifndef RTOS_INC_RTOS_PRIVATE_H_
#define RTOS_INC_RTOS_PRIVATE_H_

typedef struct
{
	uint16_t Periodicity;
	void (*TaskHandler)(void);

}Task;


#endif /* RTOS_INC_RTOS_PRIVATE_H_ */
