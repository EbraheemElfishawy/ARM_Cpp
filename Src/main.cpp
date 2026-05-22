/**
 ******************************************************************************
 * @file           : main.cpp
 * @brief          : IR Record and Playback Application (AC Remote Simulator)
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

// ----------------------------------------------------------------------------
// Pin Definitions
// ----------------------------------------------------------------------------
using IrRxPin    = MCAL::GpioPin<MCAL::Port::A, 0>;  // HX1838 Receiver (EXTI0)
using IrTxPin    = MCAL::GpioPin<MCAL::Port::A, 8>;  // IR Transmitter LED (TIM1_CH1)
using UserButton = MCAL::GpioPin<MCAL::Port::C, 13>; // Nucleo Blue Button
using StatusLed  = MCAL::GpioPin<MCAL::Port::A, 5>;  // Nucleo Green LED

// ----------------------------------------------------------------------------
// Global IR Buffer (The "Memory" of the AC Remote)
// ----------------------------------------------------------------------------
constexpr uint16_t MAX_IR_PULSES = 400; // AC remotes can have 100+ bit frames
volatile uint32_t irBuffer[MAX_IR_PULSES];
volatile uint16_t irPulseCount = 0;
volatile bool isRecording = false;
volatile bool hasRecordedSignal = false;

// ----------------------------------------------------------------------------
// Blocking Microsecond Delay (Safe to use ONLY during playback, not sniffing)
// ----------------------------------------------------------------------------
void DelayMicroseconds(uint32_t us) {
    MCAL::TimManager::ResetStopwatch();
    while(MCAL::TimManager::GetStopwatchValue() < us) {}
}

// ----------------------------------------------------------------------------
// 1. The Sniffer (Record Logic) - Runs in EXTI Interrupt
// ----------------------------------------------------------------------------
void OnIrPulseEdge() {
    // 1. Read how long the previous state lasted
    uint32_t duration = MCAL::TimManager::GetStopwatchValue();
    MCAL::TimManager::ResetStopwatch();

    // 2. State Machine Logic
    // If the duration is massive (>50ms), it means the remote was idle.
    // The current edge is the START of a new transmission.
    if (duration > 50000) {
        irPulseCount = 0;
        isRecording = true;
        hasRecordedSignal = false;
        StatusLed::SetHigh(); // Turn on LED to indicate recording started
    } 
    // If we are actively recording and haven't run out of memory...
    else if (isRecording && irPulseCount < MAX_IR_PULSES) {
        // The HX1838 is ACTIVE LOW. 
        // Index 0: Duration of the Mark (Time between Falling and Rising edge)
        // Index 1: Duration of the Space (Time between Rising and Falling edge)
        irBuffer[irPulseCount++] = duration;
    }
}

// ----------------------------------------------------------------------------
// 2. The Transmitter (Playback Logic)
// ----------------------------------------------------------------------------
void PlayIrSignal() {
    if (irPulseCount == 0) return; // Nothing to play

    StatusLed::SetLow(); // Blink LED off during transmission

    // IMPORTANT: Disable receiver AND the RTOS SysTick to guarantee 
    // microsecond-perfect timing without context-switching interruptions!
    MCAL::NvicManager::DisableInterrupt(6);  // Disable EXTI0
    MCAL::NvicManager::DisableInterrupt(-1); // Disable SysTick (Core Exception)

    // Replay the recorded timings
    for(uint16_t i = 0; i < irPulseCount; i++) {
        if (i % 2 == 0) {
            // Even Indices are "Marks". Turn on the 38kHz PWM.
            MCAL::TimManager::StartPwm();
        } else {
            // Odd Indices are "Spaces". Turn off the PWM (Silence).
            MCAL::TimManager::StopPwm();
        }
        // Wait for the exact recorded duration
        DelayMicroseconds(irBuffer[i]);
    }
    
    // Ensure the transmitter is fully OFF at the end of the frame
    MCAL::TimManager::StopPwm();

    // Re-enable interrupts
    MCAL::Exti::ClearPending(0);
    MCAL::NvicManager::EnableInterrupt(6);
    MCAL::NvicManager::EnableInterrupt(-1); // Re-enable SysTick
    
    StatusLed::SetHigh(); // LED back on
}

// ----------------------------------------------------------------------------
// RTOS Tasks
// ----------------------------------------------------------------------------
void CheckButtonTask() {
    // The Nucleo user button is ACTIVE LOW (pulled high externally)
    // If pressed, play back the recorded signal!
    if (UserButton::Read() == false) {
        if (irPulseCount > 0) {
            PlayIrSignal();
        }
    }
}

void IdleTimeoutTask() {
    // If we were recording, and a long time has passed since the last edge,
    // the transmission is completely finished.
    if (isRecording && MCAL::TimManager::GetStopwatchValue() > 50000) {
        isRecording = false;
        if (irPulseCount > 10) { 
            hasRecordedSignal = true; // Signal successfully captured!
            StatusLed::SetLow(); // Turn off LED to indicate ready
        }
    }
}

int main()
{
    // --- Clock Initialization ---
    MCAL::RccManager::EnableHSI();
    MCAL::RccManager::EnablePortA();
    MCAL::RccManager::EnablePortC();
    MCAL::SyscfgManager::EnableClock();

    // --- GPIO Setup ---
    StatusLed::SetMode(MCAL::Mode::Output);
    StatusLed::SetLow();
    
    UserButton::SetMode(MCAL::Mode::Input); // PC13 Input
    
    IrTxPin::SetMode(MCAL::Mode::Alternate); // PA8 PWM
    IrTxPin::SetAlternateFunction(1); 
    IrTxPin::SetHighSpeed(); // Maximize edge sharpness for PWM
    
    IrRxPin::SetMode(MCAL::Mode::Input); // PA0 Receiver

    // --- EXTI Setup ---
    MCAL::SyscfgManager::RouteExtiLine(0, 0); // PA0 -> EXTI0
    MCAL::Exti::ConfigureLine(0, MCAL::Trigger::Both);
    MCAL::Exti::RegisterCallback(0, OnIrPulseEdge);
    MCAL::NvicManager::EnableInterrupt(6); 

    // --- Timers Setup ---
    MCAL::TimManager::InitStopwatch(); // TIM2 at 1MHz (1us precision)
    MCAL::TimManager::InitPwm38kHz();  // TIM1 at 38kHz (Output disabled initially)

    // --- RTOS Setup ---
    RTOS::Scheduler::CreateTask(0, 100, &CheckButtonTask); // Check button every 100ms
    RTOS::Scheduler::CreateTask(1, 50,  &IdleTimeoutTask); // Check for IR timeout every 50ms
    RTOS::Scheduler::Start();

    // --- Sleep ---
    for(;;)
    {
        __asm("wfi"); 
    }
}
