#include "../../Inc/CPP_MCAL/Stk.hpp"

extern "C" void SysTick_Handler(void) {
    if (MCAL::SysTick::Callback) {
        MCAL::SysTick::Callback();
    }
}
