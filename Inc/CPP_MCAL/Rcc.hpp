#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    class RccManager {
    public:
        static void EnableHSI() {
            MCU::RCC->CR |= (1 << 0); // HSION
            while ((MCU::RCC->CR & (1 << 1)) == 0) {} // Wait for HSIRDY
            
            // Set HSI as system clock
            MCU::RCC->CFGR &= ~0x3; 
            while ((MCU::RCC->CFGR & 0xC) != 0) {} // Wait until HSI is used
        }

        static void EnableHSE() {
            MCU::RCC->CR |= (1 << 16); // HSEON
            while ((MCU::RCC->CR & (1 << 17)) == 0) {} // Wait for HSERDY
        }
        
        static void EnablePortA() {
            MCU::RCC->AHB1ENR |= (1 << 0);
        }
        
        static void EnablePortC() {
            MCU::RCC->AHB1ENR |= (1 << 2);
        }
    };
}
