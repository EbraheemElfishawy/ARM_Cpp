#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    class NvicManager {
    public:
        static void EnableInterrupt(uint8_t irq) {
            MCU::NVIC->ISER[irq / 32] = (1UL << (irq % 32));
        }

        static void DisableInterrupt(uint8_t irq) {
            MCU::NVIC->ICER[irq / 32] = (1UL << (irq % 32));
        }

        static void SetPending(uint8_t irq) {
            MCU::NVIC->ISPR[irq / 32] = (1UL << (irq % 32));
        }

        static void ClearPending(uint8_t irq) {
            MCU::NVIC->ICPR[irq / 32] = (1UL << (irq % 32));
        }

        static bool GetActive(uint8_t irq) {
            return (MCU::NVIC->IABR[irq / 32] & (1UL << (irq % 32))) != 0;
        }

        static void SetPriority(int8_t irq, uint8_t groupPriority, uint8_t subPriority, uint32_t groupConfiguration) {
            uint8_t priority = subPriority | (groupPriority << ((groupConfiguration - 0x05FA0300) / 256));

            if (irq < 0) {
                if (irq == -1) MCU::SCB->SHP[11] = (priority << 4); // SysTick
                else if (irq == -2) MCU::SCB->SHP[10] = (priority << 4); // PendSV
                else if (irq == -5) MCU::SCB->SHP[7] = (priority << 4); // SVCall
            } else {
                MCU::NVIC->IP[irq] = (priority << 4);
            }
            MCU::SCB->AIRCR = groupConfiguration;
        }
    };
}
