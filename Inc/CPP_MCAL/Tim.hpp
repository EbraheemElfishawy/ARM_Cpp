/**
 ******************************************************************************
 * @file           : Tim.hpp
 * @brief          : Hardware Timer Driver (TIM1, TIM2)
 * @author         : Embedded Expert Mentor
 * 
 * Provides a 1us precision stopwatch (TIM2) for recording IR pulses, 
 * and a 38kHz PWM generator (TIM1) for IR transmission.
 ******************************************************************************
 */
#pragma once
#include <cstdint>
#include "Device.hpp"

namespace MCAL {
    class TimManager {
    public:
        TimManager() = delete;

        static void EnableTim2() {
            MCU::RCC->APB1ENR |= (1 << 0); // TIM2EN
        }

        static void EnableTim1() {
            MCU::RCC->APB2ENR |= (1 << 0); // TIM1EN
        }

        /**
         * @brief Configures TIM2 as a 1-microsecond precision stopwatch.
         * Assumes HSI is the system clock (16 MHz).
         */
        static void InitStopwatch() {
            EnableTim2();
            MCU::TIM2->CR1 = 0;
            // Prescaler: (16MHz / 16) = 1MHz -> 1 tick = 1us.
            MCU::TIM2->PSC = 16 - 1;
            MCU::TIM2->ARR = 0xFFFFFFFF; // Free running 32-bit counter
            MCU::TIM2->CR1 |= (1 << 0);  // Enable counter
        }

        static uint32_t GetStopwatchValue() {
            return MCU::TIM2->CNT;
        }

        static void ResetStopwatch() {
            MCU::TIM2->CNT = 0;
        }

        /**
         * @brief Configures TIM1 CH1 (PA8) to generate a 38 kHz PWM signal.
         * Used to modulate the IR LED during transmission.
         * Assumes HSI is the system clock (16 MHz).
         */
        static void InitPwm38kHz() {
            EnableTim1();
            MCU::TIM1->CR1 = 0;
            
            // For 38kHz from 16MHz clock: ARR = (16,000,000 / 38000) = 421
            MCU::TIM1->PSC = 0;
            MCU::TIM1->ARR = 421 - 1;
            
            // 50% duty cycle for maximum IR power output
            MCU::TIM1->CCR1 = 421 / 2;

            // Configure CH1 in PWM Mode 1
            MCU::TIM1->CCMR1 &= ~(0xFF);
            MCU::TIM1->CCMR1 |= (0x6 << 4); // PWM mode 1 (OC1M = 110)
            MCU::TIM1->CCMR1 |= (1 << 3);   // Output compare 1 preload enable
            
            MCU::TIM1->CCER |= (1 << 0);    // CC1E: Enable CH1 output
            MCU::TIM1->BDTR |= (1 << 15);   // MOE: Main output enable (Required for advanced timers)
        }

        /**
         * @brief Starts transmitting the 38kHz carrier wave.
         */
        static void StartPwm() {
            MCU::TIM1->CR1 |= (1 << 0); // CEN (Counter Enable)
        }

        /**
         * @brief Stops the 38kHz carrier wave (Outputs a Space).
         */
        static void StopPwm() {
            MCU::TIM1->CR1 &= ~(1 << 0); // Clear CEN
            MCU::TIM1->CNT = 0;          // Reset counter for next burst
        }
    };
}
