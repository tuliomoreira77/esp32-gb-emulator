#include "motherboard.h"


Motherboard::Motherboard(Joypad* joypad, Screen* screen, FileSystem* fileSystem, MemoryMap* memMap) {
    this->fileSystem = fileSystem;
    this->joypad = joypad;
    this->memoryBus = new MemoryBus(joypad, fileSystem, memMap);
    this->ppu = new PPU(this->memoryBus, screen);
    this->cpu = new CPUOpt(this->memoryBus);
    this->timer = new GameboyTimer(this->memoryBus);

    this->memoryBus->writeByte(0xFF00, 0x3F);
}

void Motherboard::insertCartridge() {
    memoryBus->insertCartridge();
}

uint32_t Motherboard::runCycle() {
    if (joypad->keyPressed == true) {
        memoryBus->requestJoypadInterrupt();
        joypad->keyPressed = false;
    }

    uint16_t globalCycles = 0;
    globalCycles += cpu->executeStep();
    globalCycles += cpu->executeStep();

    globalCycles = globalCycles << 1;
    timer->step(globalCycles);
    ppu->step(globalCycles);

    return globalCycles;
}