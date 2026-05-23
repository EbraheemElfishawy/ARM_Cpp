/**
 ******************************************************************************
 * @file           : Usart.hpp
 * @brief          : USART Driver for Live Logging
 * @author         : Embedded Expert Mentor
 ******************************************************************************
 */
#pragma once
#include <cstdint>
#include "Device.hpp"
#include "Gpio.hpp"

namespace MCAL {
    class UsartManager {
    public:
        /**
         * @brief Configures USART2 for 115200 Baud, 8N1.
         * PA2: TX, PA3: RX (Connected to ST-Link VCOM)
         */
        static void InitLogging() {
            // 1. Enable Clock for USART2 and GPIOA
            MCU::RCC->APB1ENR |= (1 << 17); // USART2EN
            MCU::RCC->AHB1ENR |= (1 << 0);  // GPIOAEN

            // 2. Configure PA2 (TX) and PA3 (RX) as Alternate Function 7
            using TxPin = GpioPin<Port::A, 2>;
            using RxPin = GpioPin<Port::A, 3>;

            TxPin::SetMode(Mode::Alternate);
            TxPin::SetAlternateFunction(7);
            
            RxPin::SetMode(Mode::Alternate);
            RxPin::SetAlternateFunction(7);

            // 3. Set Baud Rate to 9600 (More reliable for initial debugging)
            // UARTDIV = 16,000,000 / (16 * 9600) = 104.166
            // Mantissa = 104 (0x68), Fraction = 0.166 * 16 = 2.6 -> 3 (0x3)
            MCU::USART2->BRR = (104 << 4) | 3;

            // 4. Small delay to let pins stabilize
            for(volatile int i=0; i<1000; i++);

            // 5. Enable Transmitter and USART
            MCU::USART2->CR1 |= (1 << 3);  // TE: Transmitter Enable
            MCU::USART2->CR1 |= (1 << 13); // UE: USART Enable
        }

        static void SendChar(char c) {
            while (!(MCU::USART2->SR & (1 << 7))) {} // Wait for TXE
            MCU::USART2->DR = c;
        }

        static void SendString(const char* s) {
            while (*s) {
                SendChar(*s++);
            }
        }

        static void SendInt(int32_t n) {
            if (n == 0) {
                SendChar('0');
                return;
            }
            if (n < 0) {
                SendChar('-');
                n = -n;
            }
            char buf[12];
            int i = 10;
            buf[i--] = '\0';
            while (n > 0) {
                buf[i--] = (n % 10) + '0';
                n /= 10;
            }
            SendString(&buf[i + 1]);
        }
        
        static void SendHex(uint32_t n) {
            SendString("0x");
            for (int i = 28; i >= 0; i -= 4) {
                uint8_t nibble = (n >> i) & 0xF;
                SendChar(nibble < 10 ? nibble + '0' : nibble - 10 + 'A');
            }
        }
    };
}

#define LOG(msg) MCAL::UsartManager::SendString("[LOG] " msg "\r\n")
#define LOG_VAL(msg, val) { MCAL::UsartManager::SendString("[VAL] " msg ": "); MCAL::UsartManager::SendInt(val); MCAL::UsartManager::SendString("\r\n"); }
#define LOG_HEX(msg, val) { MCAL::UsartManager::SendString("[HEX] " msg ": "); MCAL::UsartManager::SendHex(val); MCAL::UsartManager::SendString("\r\n"); }
