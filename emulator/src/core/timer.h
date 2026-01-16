#ifndef TIMER_H
#define TIMER_H

#include "calculator.h"
#include "bus.h"

class GameboyTimer {
    private:
        uint16_t dividerStepLimit = 256;
        uint16_t dividerCycle = 0;
        uint16_t timerCounterCycle = 0;
        uint16_t timerStepLimit = 0;

        uint32_t clockMap[4] = {4096, 262144, 65536, 16384};

        MemoryBus* bus = nullptr;

        void stepDivider(uint16_t cycles);
        void stepTimer(uint16_t cycles);

    public:
        GameboyTimer(MemoryBus* bus);
        void step(uint16_t cycles);

};


#endif