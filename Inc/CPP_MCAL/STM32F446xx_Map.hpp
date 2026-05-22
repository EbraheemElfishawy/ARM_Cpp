#pragma once
#include <cstdint>

namespace MCU {

    // STM32F446xx Specific Peripherals
    struct RCC_Regs {
        volatile uint32_t CR;
        volatile uint32_t PLLCFGR;
        volatile uint32_t CFGR;
        volatile uint32_t CIR;
        volatile uint32_t AHB1RSTR;
        volatile uint32_t AHB2RSTR;
        volatile uint32_t AHB3RSTR;
        uint32_t Reserved0;
        volatile uint32_t APB1RSTR;
        volatile uint32_t APB2RSTR;
        uint32_t Reserved1[2];
        volatile uint32_t AHB1ENR;
        volatile uint32_t AHB2ENR;
        volatile uint32_t AHB3ENR;
        uint32_t Reserved2;
        volatile uint32_t APB1ENR;
        volatile uint32_t APB2ENR;
    };

    struct GPIO_Regs {
        volatile uint32_t MODER;
        volatile uint32_t OTYPER;
        volatile uint32_t OSPEEDR;
        volatile uint32_t PUPDR;
        volatile uint32_t IDR;
        volatile uint32_t ODR;
        volatile uint32_t BSRR;
        volatile uint32_t LCKR;
        volatile uint32_t AFRL;
        volatile uint32_t AFRH;
    };

    // Base Addresses
    inline RCC_Regs*  const RCC   = reinterpret_cast<RCC_Regs*>(0x40023800);
    inline GPIO_Regs* const GPIOA = reinterpret_cast<GPIO_Regs*>(0x40020000);
    inline GPIO_Regs* const GPIOB = reinterpret_cast<GPIO_Regs*>(0x40020400);
    inline GPIO_Regs* const GPIOC = reinterpret_cast<GPIO_Regs*>(0x40020800);
    inline GPIO_Regs* const GPIOD = reinterpret_cast<GPIO_Regs*>(0x40020C00);

    struct EXTI_Regs {
        volatile uint32_t IMR;
        volatile uint32_t EMR;
        volatile uint32_t RTSR;
        volatile uint32_t FTSR;
        volatile uint32_t SWIER;
        volatile uint32_t PR;
    };

    struct SYSCFG_Regs {
        volatile uint32_t MEMRMP;
        volatile uint32_t PMC;
        volatile uint32_t EXTICR[4];
        uint32_t RESERVED[2];
        volatile uint32_t CMPCR;
    };

    struct TIM_Regs {
        volatile uint32_t CR1;
        volatile uint32_t CR2;
        volatile uint32_t SMCR;
        volatile uint32_t DIER;
        volatile uint32_t SR;
        volatile uint32_t EGR;
        volatile uint32_t CCMR1;
        volatile uint32_t CCMR2;
        volatile uint32_t CCER;
        volatile uint32_t CNT;
        volatile uint32_t PSC;
        volatile uint32_t ARR;
        volatile uint32_t RCR;
        volatile uint32_t CCR1;
        volatile uint32_t CCR2;
        volatile uint32_t CCR3;
        volatile uint32_t CCR4;
        volatile uint32_t BDTR;
        volatile uint32_t DCR;
        volatile uint32_t DMAR;
        volatile uint32_t OR;
    };

    inline EXTI_Regs*   const EXTI   = reinterpret_cast<EXTI_Regs*>(0x40013C00);
    inline SYSCFG_Regs* const SYSCFG = reinterpret_cast<SYSCFG_Regs*>(0x40013800);
    inline TIM_Regs*    const TIM1   = reinterpret_cast<TIM_Regs*>(0x40010000);
    inline TIM_Regs*    const TIM2   = reinterpret_cast<TIM_Regs*>(0x40000000);

}
