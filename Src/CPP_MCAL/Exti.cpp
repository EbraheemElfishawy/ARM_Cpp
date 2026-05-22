/**
 ******************************************************************************
 * @file           : Exti.cpp
 * @brief          : External Interrupt Service Routines
 * @author         : Embedded Expert Mentor
 ******************************************************************************
 */
#include "../../Inc/CPP_MCAL/Exti.hpp"

// EXTI Line 0 Interrupt Service Routine (Used for PA0 IR Receiver)
extern "C" void EXTI0_IRQHandler(void) {
    // Verify that EXTI0 triggered the interrupt
    if (MCU::EXTI->PR & (1 << 0)) {
        // Clear the pending flag immediately to prevent re-triggering
        MCAL::Exti::ClearPending(0);
        
        // Execute the registered callback if it exists
        if (MCAL::Exti::Callbacks[0]) {
            MCAL::Exti::Callbacks[0]();
        }
    }
}
