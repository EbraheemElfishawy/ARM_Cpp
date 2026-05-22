/**
 ******************************************************************************
 * @file           : Exti.hpp
 * @brief          : External Interrupt (EXTI) Driver
 * @author         : Embedded Expert Mentor
 * 
 * Configures the EXTI lines to detect rising/falling edges and manage 
 * callback functions triggered by hardware events.
 ******************************************************************************
 */
#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    enum class Trigger { Rising, Falling, Both };

    class Exti {
    public:
        using Callback_t = void(*)();
        
        // Array of function pointers for each of the 16 EXTI lines
        static inline Callback_t Callbacks[16] = {nullptr};

        Exti() = delete;

        /**
         * @brief Unmasks an EXTI line and configures its edge trigger.
         */
        static void ConfigureLine(uint8_t line, Trigger trigger) {
            MCU::EXTI->IMR |= (1 << line); // Unmask interrupt
            
            // Clear both triggers first
            MCU::EXTI->RTSR &= ~(1 << line);
            MCU::EXTI->FTSR &= ~(1 << line);

            if (trigger == Trigger::Rising || trigger == Trigger::Both) {
                MCU::EXTI->RTSR |= (1 << line);
            }
            if (trigger == Trigger::Falling || trigger == Trigger::Both) {
                MCU::EXTI->FTSR |= (1 << line);
            }
        }
        
        /**
         * @brief Clears the Pending Register flag for a specific line.
         */
        static void ClearPending(uint8_t line) {
            MCU::EXTI->PR = (1 << line); // rc_w1 (Write 1 to clear)
        }
        
        /**
         * @brief Registers a callback function for a specific EXTI line.
         */
        static void RegisterCallback(uint8_t line, Callback_t cb) {
            if(line < 16) {
                Callbacks[line] = cb;
            }
        }
    };
}
