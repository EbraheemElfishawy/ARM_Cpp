#include "../../Inc/CPP_RTOS/Rtos.hpp"
#include "../../Inc/CPP_MCAL/Stk.hpp"

namespace RTOS {
    void Scheduler::Start() {
        MCAL::SysTick::Callback = Scheduler::Tick;
        MCAL::SysTick::Init(0, true); // AHB/8, interrupt enabled
        MCAL::SysTick::Start(2000); // Approximation for 1ms on 16MHz AHB/8
    }

    void Scheduler::Tick() {
        tickCounter++;
        for (uint8_t i = 0; i < MAX_TASKS; i++) {
            if (tasks[i].periodicity != 0 && (tickCounter % tasks[i].periodicity) == 0) {
                if (tasks[i].handler) {
                    tasks[i].handler();
                }
            }
        }
    }
}
