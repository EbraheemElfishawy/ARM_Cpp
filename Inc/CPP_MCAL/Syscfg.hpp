/**
 ******************************************************************************
 * @file           : Syscfg.hpp
 * @brief          : System Configuration (SYSCFG) Driver
 * @author         : Embedded Expert Mentor
 * 
 * Manages the routing of GPIO pins to EXTI lines.
 ******************************************************************************
 */
#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    class SyscfgManager {
    public:
        SyscfgManager() = delete;
        
        /**
         * @brief Enables the clock for the SYSCFG peripheral on APB2.
         * Must be called before modifying EXTI routing.
         */
        static void EnableClock() {
            MCU::RCC->APB2ENR |= (1 << 14); // SYSCFGEN
        }
        
        /**
         * @brief Routes a specific port pin to the corresponding EXTI line.
         * @param port 0 for Port A, 1 for Port B, 2 for Port C, etc.
         * @param line The EXTI line number (0 to 15).
         */
        static void RouteExtiLine(uint8_t port, uint8_t line) {
            uint8_t regIndex = line / 4;
            uint8_t shift = (line % 4) * 4;
            MCU::SYSCFG->EXTICR[regIndex] &= ~(0xF << shift);
            MCU::SYSCFG->EXTICR[regIndex] |= (port << shift);
        }
    };
}
