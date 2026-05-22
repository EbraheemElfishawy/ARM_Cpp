#pragma once
#include <cstdint>

namespace MCU {

    // ARM Cortex-M4 Core Peripherals
    struct STK_Regs {
        volatile uint32_t CTRL;
        volatile uint32_t LOAD;
        volatile uint32_t VAL;
        volatile uint32_t CALIB;
    };

    struct NVIC_Regs {
        volatile uint32_t ISER[8];
        uint32_t RESERVED0[24];
        volatile uint32_t ICER[8];
        uint32_t RSERVED1[24];
        volatile uint32_t ISPR[8];
        uint32_t RESERVED2[24];
        volatile uint32_t ICPR[8];
        uint32_t RESERVED3[24];
        volatile uint32_t IABR[8];
        uint32_t RESERVED4[56];
        volatile uint8_t  IP[240];
        uint32_t RESERVED5[644];
        volatile uint32_t STIR;
    };

    struct SCB_Regs {
        volatile uint32_t CPUID;
        volatile uint32_t ICSR;
        volatile uint32_t VTOR;
        volatile uint32_t AIRCR;
        volatile uint32_t SCR;
        volatile uint32_t CCR;
        volatile uint8_t  SHP[12];
        volatile uint32_t SHCSR;
        volatile uint32_t CFSR;
        volatile uint32_t HFSR;
        volatile uint32_t DFSR;
        volatile uint32_t MMFAR;
        volatile uint32_t BFAR;
        volatile uint32_t AFSR;
    };

    inline STK_Regs*  const STK  = reinterpret_cast<STK_Regs*>(0xE000E010);
    inline NVIC_Regs* const NVIC = reinterpret_cast<NVIC_Regs*>(0xE000E100);
    inline SCB_Regs*  const SCB  = reinterpret_cast<SCB_Regs*>(0xE000ED00);

}
