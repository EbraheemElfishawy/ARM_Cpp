#pragma once

// 1. Include ARM Core Architecture (common across all Cortex-M4)
#include "CortexM4_Map.hpp"
#define STM32F446xx
// 2. Select Vendor Specific Implementation based on compiler flags
#if defined(STM32F446xx)
    #include "STM32F446xx_Map.hpp"
#elif defined(STM32F401xC)
    // For when you compile for the Blackpill
    // #include "STM32F401xx_Map.hpp"
#else
    #error "Please select the target MCU (e.g. -DSTM32F446xx)"
#endif
