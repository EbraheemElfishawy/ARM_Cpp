/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : Professional AC Cloner - HARDWARE TIMED STABLE
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

using IrRxPin    = MCAL::GpioPin<MCAL::Port::A, 0>;  
using IrTxPin    = MCAL::GpioPin<MCAL::Port::A, 8>;  
using UserButton = MCAL::GpioPin<MCAL::Port::C, 13>; 
using StatusLed  = MCAL::GpioPin<MCAL::Port::A, 5>;  
using VerifyLed  = MCAL::GpioPin<MCAL::Port::C, 0>;  

constexpr uint16_t MAX_IR_PULSES = 1500; 
volatile uint32_t bufferON[MAX_IR_PULSES], bufferOFF[MAX_IR_PULSES], bufferSniff[MAX_IR_PULSES];
volatile uint16_t countON = 0, countOFF = 0, countSniff = 0;

enum class AppMode { AWAITING_ON, RECORDING_ON, AWAITING_OFF, RECORDING_OFF, READY, VERIFYING };
volatile AppMode appMode = AppMode::AWAITING_ON;
volatile uint32_t deafTimer = 0; 
volatile bool nextPlayIsON = true; 
volatile bool triggerPlayback = false;

extern "C" {
    void SystemInit(void) {
        volatile uint32_t* CPACR = (volatile uint32_t*)0xE000ED88;
        *CPACR |= ((3UL << 20) | (3UL << 22)); 
        volatile uint32_t* VTOR = (volatile uint32_t*)0xE000ED08;
        *VTOR = 0x08000000;
    }
}

// ----------------------------------------------------------------------------
// PWM-Compatible Bit Extraction
// ----------------------------------------------------------------------------
uint16_t ExtractBits(volatile uint32_t* buffer, uint16_t count, uint8_t* bitBuffer) {
    if (count < 10) return 0;
    uint16_t bIdx = 0;
    for (uint16_t i = 3; i < count - 1; i += 2) {
        uint32_t p = buffer[i];
        uint32_t s = buffer[i+1];
        bitBuffer[bIdx++] = (p > 15000 || s > 15000) ? 1 : 0;
        if (bIdx >= 200) break;
    }
    return bIdx;
}

void LogSignalData(const char* label, volatile uint32_t* buffer, uint16_t count) {
    MCAL::UsartManager::SendString("\r\n--- SIGNAL: ");
    MCAL::UsartManager::SendString(label);
    MCAL::UsartManager::SendString(" ---\r\n");
    LOG_VAL("Pulses", count);
    uint8_t bits[200];
    uint16_t n = ExtractBits(buffer, count, bits);
    if (n > 0) {
        MCAL::UsartManager::SendString("Bits (LSB First): ");
        for(uint16_t i=0; i<n; i++) {
            MCAL::UsartManager::SendChar(bits[i] ? '1' : '0');
            if ((i+1)%8 == 0) MCAL::UsartManager::SendChar(' ');
        }
        MCAL::UsartManager::SendString("\r\n");
    }
    MCAL::UsartManager::SendString("------------------------------------\r\n");
}

bool VerifySignal(bool wasON) {
    volatile uint32_t* orig = wasON ? bufferON : bufferOFF; 
    uint16_t oCnt = wasON ? countON : countOFF;
    if (oCnt == 0 || countSniff == 0) return false;
    uint8_t bitsO[200], bitsS[200];
    uint16_t nO = ExtractBits(orig, oCnt, bitsO);
    uint16_t nS = ExtractBits(bufferSniff, countSniff, bitsS);
    if (nO == 0 || nS == 0) return false;
    uint16_t matches = 0, limit = (nO < nS) ? nO : nS;
    for(uint16_t i=0; i<limit; i++) { if (bitsO[i] == bitsS[i]) matches++; }
    uint16_t score = (matches * 100) / limit;
    LOG_VAL("Bit-Match Accuracy (%)", score);
    return (score >= 90);
}

void OnIrPulseEdge() {
    uint32_t duration = MCAL::TimManager::GetStopwatchValue();
    MCAL::TimManager::ResetStopwatch();
    if (deafTimer > 0) return;
    if (appMode == AppMode::AWAITING_ON) { countON = 0; appMode = AppMode::RECORDING_ON; StatusLed::SetHigh(); } 
    else if (appMode == AppMode::AWAITING_OFF) { countOFF = 0; appMode = AppMode::RECORDING_OFF; StatusLed::SetHigh(); }
    else if (appMode == AppMode::VERIFYING) { if (duration > 10 && countSniff < MAX_IR_PULSES) bufferSniff[countSniff++] = duration; }
    else if (appMode == AppMode::RECORDING_ON) { if (duration > 50 && countON < MAX_IR_PULSES) bufferON[countON++] = duration; }
    else if (appMode == AppMode::RECORDING_OFF) { if (duration > 50 && countOFF < MAX_IR_PULSES) bufferOFF[countOFF++] = duration; }
}

void PlayStoredSignal(bool useON) {
    volatile uint32_t* buffer = useON ? bufferON : bufferOFF;
    uint16_t count = useON ? countON : countOFF;
    if (count == 0) return;

    if (useON) { LOG("TX: Sending [ON]..."); } else { LOG("TX: Sending [OFF]..."); }
    
    appMode = AppMode::VERIFYING; countSniff = 0; MCAL::TimManager::ResetStopwatch(); StatusLed::SetHigh();
    MCU::STK->CTRL &= ~(1 << 1); 
    for(uint16_t i = 0; i < count; i++) {
        if (i % 2 == 0) MCAL::TimManager::StartPwm(); else MCAL::TimManager::StopPwm();
        MCAL::TimManager::WaitUs(buffer[i]); // TIM5 Hardware Precision
    }
    MCAL::TimManager::StopPwm();
    MCU::STK->CTRL |= (1 << 1); 
    StatusLed::SetLow();

    MCAL::SysTick::DelayMs(400); 
    LogSignalData("LOOPBACK SAMPLE", bufferSniff, countSniff);
    if (VerifySignal(useON)) {
        LOG("[VERIFY] BIT-MATCH SUCCESS!"); VerifyLed::SetHigh(); MCAL::SysTick::DelayMs(1000); VerifyLed::SetLow();
    } else {
        LOG("[VERIFY] BIT-MATCH FAILED!"); for(int i=0; i<10; i++) { VerifyLed::Toggle(); MCAL::SysTick::DelayMs(50); }
    }
    appMode = AppMode::READY; deafTimer = 500;
}

void CheckButtonTask() {
    static bool last = true; bool cur = UserButton::Read();
    bool trigger = (cur == false && last == true);
    if (MCAL::UsartManager::DataAvailable() && MCAL::UsartManager::ReceiveChar() == 'p') trigger = true;
    if (trigger && appMode == AppMode::READY) triggerPlayback = true;
    last = cur;
}

void IdleTimeoutTask() {
    if (MCAL::TimManager::GetStopwatchValue() > 1000000) {
        if (appMode == AppMode::RECORDING_ON) { appMode = AppMode::AWAITING_OFF; StatusLed::SetLow(); LogSignalData("REMOTE [ON]", bufferON, countON); LOG("Next: [OFF]..."); } 
        else if (appMode == AppMode::RECORDING_OFF) { appMode = AppMode::READY; StatusLed::SetLow(); LogSignalData("REMOTE [OFF]", bufferOFF, countOFF); LOG("=== READY TO TOGGLE ==="); }
    }
    if (deafTimer > 0) { if (deafTimer >= 50) deafTimer -= 50; else deafTimer = 0; }
}

void HeartbeatTask() {
    static uint16_t counter = 0; static uint32_t uptime = 0;
    counter++;
    if (counter % 40 == 0) { uptime += 2; LOG_VAL("System Ping (s)", uptime); }
    if (appMode == AppMode::AWAITING_ON) { if (counter % 10 == 0) StatusLed::Toggle(); }
    else if (appMode == AppMode::AWAITING_OFF) { if (counter % 2 == 0) StatusLed::Toggle(); }
    else if (appMode == AppMode::READY) {
        uint16_t p = counter % 20;
        if (nextPlayIsON) { if (p == 0) StatusLed::SetHigh(); if (p == 2) StatusLed::SetLow(); } 
        else { if (p == 0 || p == 4) StatusLed::SetHigh(); if (p == 2 || p == 6) StatusLed::SetLow(); }
    }
}

int main() {
    MCU::RCC->AHB1ENR |= (1 << 0) | (1 << 2); MCU::GPIOA->MODER &= ~(3UL << 10); MCU::GPIOA->MODER |= (1UL << 10); MCU::GPIOA->BSRR = (1UL << 5); 
    MCAL::RccManager::EnableHSI(); MCAL::TimManager::InitStopwatch(); MCAL::TimManager::InitDelayTimer(); MCAL::UsartManager::InitLogging(); 
    LOG("--- FINAL PRO CLONER (HARDWARE TIMED) ---");
    MCAL::SyscfgManager::EnableClock();
    StatusLed::SetMode(MCAL::Mode::Output); VerifyLed::SetMode(MCAL::Mode::Output); UserButton::SetMode(MCAL::Mode::Input); 
    IrTxPin::SetMode(MCAL::Mode::Alternate); IrTxPin::SetAlternateFunction(1); IrTxPin::SetHighSpeed(); 
    IrRxPin::SetMode(MCAL::Mode::Input); IrRxPin::SetPullUp(); 
    MCAL::SyscfgManager::RouteExtiLine(0, 0); MCAL::Exti::ConfigureLine(0, MCAL::Trigger::Both);
    MCAL::Exti::RegisterCallback(0, OnIrPulseEdge); MCAL::NvicManager::EnableInterrupt(6); 
    MCAL::TimManager::InitPwm38kHz();  
    RTOS::Scheduler::CreateTask(0, 50, &CheckButtonTask); RTOS::Scheduler::CreateTask(1, 50, &IdleTimeoutTask); RTOS::Scheduler::CreateTask(2, 50, &HeartbeatTask);   
    RTOS::Scheduler::Start();
    for(;;) { if (triggerPlayback) { triggerPlayback = false; PlayStoredSignal(nextPlayIsON); nextPlayIsON = !nextPlayIsON; } __asm("wfi"); }
}
