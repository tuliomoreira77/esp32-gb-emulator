#include "timer.h"

GameboyTimer::GameboyTimer(MemoryBus* bus) {
    this->bus = bus;
}

void GameboyTimer::step(uint16_t cycles) {
    stepDivider(cycles);
    stepTimer(cycles);
}


void GameboyTimer::stepDivider(uint16_t cycles) {
    dividerCycle += cycles;
    if (dividerCycle > dividerStepLimit) {
        dividerCycle = dividerCycle - dividerStepLimit;
        bus->incTimerDiv();
    }
} 


void GameboyTimer::stepTimer(uint16_t cycles) {
    timerCounterCycle += cycles;

    uint8_t timerCounterControl = bus->readVRam(TIMER_CONTROL);
    bool enableInc = (timerCounterControl >> 2) & 0x1;

    timerStepLimit = 4194304u/(clockMap[timerCounterControl & 0b11]);

    if (timerCounterCycle > timerStepLimit) {
        timerCounterControl = timerCounterControl - timerStepLimit;
        if(enableInc) {
            bus->incTimerCounter();
        }
    }

}