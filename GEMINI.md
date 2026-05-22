# Project Guidelines: MyNucleof446re

This document outlines the architectural standards, coding conventions, and development workflows for the MyNucleof446re project.

## 1. Project Overview
A modern C++ firmware for the STM32F446RE (Nucleo-64) development board. The project features a custom Microcontroller Abstraction Layer (MCAL) and a lightweight Real-Time Operating System (RTOS) wrapper.

## 2. Architecture
The project follows a layered architecture:
- **Application Layer**: Located in `Src/main.cpp`. Contains the high-level logic (e.g., IR recording/playback).
- **RTOS Layer**: Located in `Inc/CPP_RTOS`. Manages task scheduling.
- **MCAL Layer**: Located in `Inc/CPP_MCAL`. Provides type-safe, template-based access to hardware peripherals.
- **Hardware Mapping**: Located in `Inc/CPP_MCAL/Device.hpp` and associated map files. Defines register structures and base addresses.

## 3. Coding Standards
To maintain consistency, adhere to the following naming conventions:
- **Namespaces**: Use `MCAL`, `RTOS`, and `MCU` for clear separation of concerns.
- **Classes/Enums**: `PascalCase` (e.g., `GpioPin`, `Mode`).
- **Methods/Functions**: `PascalCase` (e.g., `SetMode`, `CreateTask`).
- **Variables/Data Members**: `camelCase` (e.g., `tickCounter`, `tasks`).
- **Constants/Macros**: `SCREAMING_SNAKE_CASE` (e.g., `MAX_TASKS`).
- **Templates**: Use `camelCase` for template parameters where appropriate.

### Style
- **Indentation**: 4 spaces.
- **Braces**: K&R style (opening brace on the same line).
- **Files**: Use `.hpp` for C++ headers and `.cpp` for source files.

## 4. Hardware Configuration
- **MCU**: STM32F446RET6
- **Core**: ARM Cortex-M4 with FPU
- **Clock**: HSI (16MHz) or HSE (8MHz Bypass) as configured in `RccManager`.

## 5. Build and Tooling
- **Build System**: `Makefile`. Run `make` to compile.
- **IDE**: Optimized for STM32CubeIDE but compatible with any C++11 cross-compiler.
- **Linter**: Use the `code-reviewer` skill for style checks.

## 6. Development Workflow
- **MCAL Updates**: When adding a new peripheral, use template-based classes in `Inc/CPP_MCAL` to ensure zero-overhead abstraction.
- **Task Management**: Register all high-level application logic as RTOS tasks with defined periodicities.
- **Testing**: Validate changes on physical hardware whenever possible, especially for time-sensitive features like IR signal processing.
