#pragma once
#include <cstdint>

namespace RTOS {
    constexpr uint8_t MAX_TASKS = 3;

    struct Task {
        uint16_t periodicity;
        void (*handler)(void);
    };

    class Scheduler {
    private:
        static inline Task tasks[MAX_TASKS] = {};
        static inline uint16_t tickCounter = 0;

    public:
        static void CreateTask(uint8_t priority, uint16_t periodicity, void(*handler)(void)) {
            if (priority < MAX_TASKS) {
                tasks[priority].periodicity = periodicity;
                tasks[priority].handler = handler;
            }
        }

        static void Start();
        static void Tick();
    };
}
