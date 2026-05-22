#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    enum class Port { A, B, C, D };
    enum class Mode : uint32_t { Input = 0, Output = 1, Alternate = 2, Analog = 3 };

    template<Port port, uint8_t pin>
    class GpioPin {
    private:
        static MCU::GPIO_Regs* GetPort() {
            if constexpr (port == Port::A) return MCU::GPIOA;
            if constexpr (port == Port::B) return MCU::GPIOB;
            if constexpr (port == Port::C) return MCU::GPIOC;
            if constexpr (port == Port::D) return MCU::GPIOD;
            return nullptr;
        }
    public:
        static void SetMode(Mode mode) {
            auto p = GetPort();
            p->MODER &= ~(3UL << (pin * 2));
            p->MODER |= (static_cast<uint32_t>(mode) << (pin * 2));
        }

        static void SetAlternateFunction(uint8_t af) {
            auto p = GetPort();
            if constexpr (pin < 8) {
                p->AFRL &= ~(0xFUL << (pin * 4));
                p->AFRL |= (static_cast<uint32_t>(af) << (pin * 4));
            } else {
                p->AFRH &= ~(0xFUL << ((pin - 8) * 4));
                p->AFRH |= (static_cast<uint32_t>(af) << ((pin - 8) * 4));
            }
        }

        /**
         * @brief Sets the output speed of the pin to High Speed (50MHz).
         * Necessary for clean high-frequency signals like 38kHz PWM.
         */
        static void SetHighSpeed() {
            auto p = GetPort();
            p->OSPEEDR |= (3UL << (pin * 2)); // 11: High speed
        }
        
        static void Toggle() {
            auto p = GetPort();
            p->ODR ^= (1UL << pin);
        }
        
        static void SetHigh() {
            GetPort()->BSRR = (1UL << pin);
        }
        
        static void SetLow() {
            GetPort()->BSRR = (1UL << (pin + 16));
        }

        /**
         * @brief Reads the current logic state of the pin.
         * Reads from the Input Data Register (IDR).
         * @return true if HIGH (Logic 1), false if LOW (Logic 0).
         */
        static bool Read() {
            return (GetPort()->IDR & (1UL << pin)) != 0;
        }
    };
}
