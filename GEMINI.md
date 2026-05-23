# MyNucleof446re: IR Protocol Analyzer & Cloner

## Core Overview
A high-precision C++11 firmware for the STM32F446RE (Nucleo-64) designed for sniffing, analyzing, and replaying IR signals (TV/AC).

## Architecture
- **MCAL Layer**: Template-based, zero-overhead hardware abstraction (`Inc/CPP_MCAL`).
- **RTOS Layer**: Lightweight task scheduler (`Inc/CPP_RTOS`).
- **Application**: Real-time sniffer and protocol decoder (`Src/main.cpp`).

## Key Features
- **Sniffing**: 1us precision timing using TIM2 and EXTI0.
- **Playback**: 38kHz carrier generation via TIM1 with modulated microsecond delays.
- **Decoding**: Pulse-distance decoding for NEC and complex AC protocols (continuous logging).
- **Stability**: Software-based noise filtering and calibrated CPU-independent delays.

## Hardware Mapping
- **PA0**: IR Receiver Input (HX1838)
- **PA8**: IR Transmitter Output (TIM1_CH1)
- **PC13**: User Button (Short-press: Play | Long-press: Clear)
- **PA5**: Status LED (Heartbeat / State Indicator)
- **PA2/PA3**: UART Logging (9600 Baud, connected to Virtual COM)

## Tools
- **Build**: `make OPT="-O2"`
- **Flash**: `STM32_Programmer_CLI` or `openocd`
- **Metadata**: Centralized in `config.json`.
