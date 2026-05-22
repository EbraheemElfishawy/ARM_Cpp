#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    class SysTick {
    public:
        using Callback_t = void(*)();
        static inline Callback_t Callback = nullptr;

        static void Init(uint8_t clockSource, bool enableInterrupt) {
            MCU::STK->CTRL = 0; // Disable first
            if (clockSource == 1) { // AHB
                MCU::STK->CTRL |= (1 << 2);
            } else { // AHB/8
                MCU::STK->CTRL &= ~(1 << 2);
            }

            if (enableInterrupt) {
                MCU::STK->CTRL |= (1 << 1);
            } else {
                MCU::STK->CTRL &= ~(1 << 1);
            }
        }

        static void Start(uint32_t reloadValue) {
            MCU::STK->LOAD = reloadValue;
            MCU::STK->VAL = 0;
            MCU::STK->CTRL |= (1 << 0); // Enable counter
        }

        static void DelayMs(uint32_t ms) {
            MCU::STK->CTRL &= ~(1 << 0);
            MCU::STK->LOAD = ms * 2000; // Simplistic approximation
            MCU::STK->VAL = 0;
            MCU::STK->CTRL |= (1 << 0);
            while (MCU::STK->VAL != 0) {}
        }
    };
}
