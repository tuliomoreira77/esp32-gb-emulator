#include "timer.h"

GameboyTimer::GameboyTimer(MemoryBus* bus) {
    this->bus = bus;
}

void IRAM_ATTR GameboyTimer::step(uint16_t cycles) {
    dividerCycle += cycles;
    bus->writeHighMemory(TIMER_DIV, dividerCycle >> 8);

    timerCounterCycle += cycles;

    uint8_t timerCounterControl = bus->readHighRam(TIMER_CONTROL);
    bool enableInc = (timerCounterControl >> 2) & 0x1;

    timerStepLimit = clockMap[timerCounterControl & 0b11];

    while (timerCounterCycle >= timerStepLimit) {
        timerCounterCycle -= timerStepLimit;

        if (enableInc)
            bus->incTimerCounter();
    }
}