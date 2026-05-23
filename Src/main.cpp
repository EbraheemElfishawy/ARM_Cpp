/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : IR Protocol Analyzer (AC Receiver Simulator) - LONG CAPTURE
 * @author         : Embedded Expert Mentor
 ******************************************************************************
 */

#include <cstdint>
#include "../Inc/CPP_MCAL/Rcc.hpp"
#include "../Inc/CPP_MCAL/Gpio.hpp"
#include "../Inc/CPP_MCAL/Stk.hpp"
#include "../Inc/CPP_MCAL/Nvic.hpp"
#include "../Inc/CPP_MCAL/Exti.hpp"
#include "../Inc/CPP_MCAL/Syscfg.hpp"
#include "../Inc/CPP_MCAL/Tim.hpp"
#include "../Inc/CPP_RTOS/Rtos.hpp"
#include "../Inc/CPP_MCAL/Usart.hpp"

// ----------------------------------------------------------------------------
// Pin Definitions
// ----------------------------------------------------------------------------
using IrRxPin    = MCAL::GpioPin<MCAL::Port::A, 0>;  
using IrTxPin    = MCAL::GpioPin<MCAL::Port::A, 8>;  
using UserButton = MCAL::GpioPin<MCAL::Port::C, 13>; 
using StatusLed  = MCAL::GpioPin<MCAL::Port::A, 5>;  

// ----------------------------------------------------------------------------
// Global IR Buffer - Increased to 1500 for complex AC frames
// ----------------------------------------------------------------------------
constexpr uint16_t MAX_IR_PULSES = 1500; 
volatile uint32_t irBuffer[MAX_IR_PULSES];
volatile uint16_t irPulseCount = 0;

enum class SnifferState { IDLE, RECORDING, CAPTURED };
volatile SnifferState snifferState = SnifferState::IDLE;
volatile uint32_t deafTimer = 0; 

extern "C" {
    void SystemInit(void) {
        volatile uint32_t* CPACR = (volatile uint32_t*)0xE000ED88;
        *CPACR |= ((3UL << 20) | (3UL << 22)); 
        volatile uint32_t* VTOR = (volatile uint32_t*)0xE000ED08;
        *VTOR = 0x08000000;
    }
}

void DelayMicroseconds(uint32_t us) {
    MCAL::TimManager::ResetStopwatch();
    while(MCAL::TimManager::GetStopwatchValue() < us) {}
}

void DecodeIrSignal() {
    LOG("--- AC SIGNAL MICROSCOPE ---");
    if (irPulseCount < 20) return;

    LOG_VAL("Total Pulses Captured", irPulseCount);

    // Log the first 20 spaces to find the 0/1 threshold
    LOG("Raw Space Timings (us):");
    for (uint16_t i = 1; i < 41; i += 2) {
        LOG_VAL(" S", irBuffer[i]);
    }

    uint8_t currentByte = 0;
    uint8_t bitIndex = 0;
    LOG("Binary Data: ");

    for (uint16_t i = 2; i < irPulseCount - 1; i += 2) {
        uint32_t space = irBuffer[i+1];
        if (space > 10000) { LOG(" [GAP] "); continue; }

        // Temporary threshold for viewing
        bool bit = (space > 1000); 
        MCAL::UsartManager::SendChar(bit ? '1' : '0');

        bitIndex++;
        if (bitIndex % 8 == 0) MCAL::UsartManager::SendChar(' ');
    }
    LOG("\r\n--- End of Analysis ---");
}

void OnIrPulseEdge() {
    uint32_t duration = MCAL::TimManager::GetStopwatchValue();
    MCAL::TimManager::ResetStopwatch();

    if (deafTimer > 0) return;
    if (snifferState == SnifferState::CAPTURED) return;

    if (snifferState == SnifferState::IDLE) {
        irPulseCount = 0;
        snifferState = SnifferState::RECORDING;
        StatusLed::SetHigh();
    } 
    else if (snifferState == SnifferState::RECORDING) {
        if (duration < 50) return; 
        if (irPulseCount < MAX_IR_PULSES) {
            irBuffer[irPulseCount++] = duration;
        }
    }
}

void PlayIrSignal() {
    if (irPulseCount == 0) return;
    LOG_VAL("TX Start", irPulseCount);
    StatusLed::SetHigh();
    MCAL::NvicManager::DisableInterrupt(6); 
    MCU::STK->CTRL &= ~(1 << 1); 

    for(uint16_t i = 0; i < irPulseCount; i++) {
        if (i % 2 == 0) MCAL::TimManager::StartPwm();
        else MCAL::TimManager::StopPwm();
        DelayMicroseconds(irBuffer[i]);
    }
    
    MCAL::TimManager::StopPwm();
    MCAL::Exti::ClearPending(0);
    MCAL::NvicManager::EnableInterrupt(6);
    MCU::STK->CTRL |= (1 << 1); 
    StatusLed::SetLow();
    deafTimer = 500; 
}

void CheckButtonTask() {
    static bool lastButtonState = true;
    static uint32_t pressTime = 0;
    bool currentButtonState = UserButton::Read();

    if (currentButtonState == false) {
        pressTime += 100;
        if (pressTime >= 2000) {
            snifferState = SnifferState::IDLE;
            irPulseCount = 0;
            LOG("CLEARED.");
            for(int i=0; i<6; i++) { StatusLed::Toggle(); MCAL::SysTick::DelayMs(50); }
            pressTime = 0;
        }
    } else {
        if (lastButtonState == false && pressTime < 2000) {
            if (snifferState == SnifferState::CAPTURED) PlayIrSignal();
            else LOG("READY.");
        }
        pressTime = 0;
    }
    lastButtonState = currentButtonState;
}

void IdleTimeoutTask() {
    if (snifferState == SnifferState::RECORDING && irPulseCount > 10) {
        if (MCAL::TimManager::GetStopwatchValue() > 1000000) {
            snifferState = SnifferState::CAPTURED;
            StatusLed::SetLow();
            DecodeIrSignal();
        }
    }
    if (deafTimer > 0) {
        if (deafTimer >= 50) deafTimer -= 50;
        else deafTimer = 0;
    }
}

void HeartbeatTask() {
    if (snifferState == SnifferState::IDLE) StatusLed::Toggle();
    else if (snifferState == SnifferState::CAPTURED) StatusLed::SetLow();
}

int main()
{
    MCU::RCC->AHB1ENR |= (1 << 0);
    MCU::GPIOA->MODER &= ~(3UL << 10);
    MCU::GPIOA->MODER |= (1UL << 10);
    MCU::GPIOA->BSRR = (1UL << 5); 

    MCAL::RccManager::EnableHSI();
    MCAL::TimManager::InitStopwatch(); 
    MCAL::UsartManager::InitLogging(); 
    
    LOG("--- AC ANALYZER BOOTED ---");
    MCU::RCC->CSR |= (1 << 24); 

    MCAL::RccManager::EnablePortA();
    MCAL::RccManager::EnablePortC();
    MCAL::SyscfgManager::EnableClock();

    StatusLed::SetMode(MCAL::Mode::Output);
    UserButton::SetMode(MCAL::Mode::Input); 
    IrTxPin::SetMode(MCAL::Mode::Alternate); 
    IrTxPin::SetAlternateFunction(1); 
    IrTxPin::SetHighSpeed(); 
    IrRxPin::SetMode(MCAL::Mode::Input); 
    IrRxPin::SetPullUp(); 

    MCAL::SyscfgManager::RouteExtiLine(0, 0); 
    MCAL::Exti::ConfigureLine(0, MCAL::Trigger::Both);
    MCAL::Exti::RegisterCallback(0, OnIrPulseEdge);
    MCAL::NvicManager::EnableInterrupt(6); 
    MCAL::TimManager::InitPwm38kHz();  

    RTOS::Scheduler::CreateTask(0, 100, &CheckButtonTask); 
    RTOS::Scheduler::CreateTask(1, 50,  &IdleTimeoutTask); 
    RTOS::Scheduler::CreateTask(2, 500, &HeartbeatTask);   
    RTOS::Scheduler::Start();

    StatusLed::SetLow();
    for(;;) { __asm("wfi"); }
}
