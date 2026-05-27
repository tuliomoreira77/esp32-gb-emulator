#pragma once

#include <cstdint>
#include <map>
#include "joypad.h"
#include "bus.h"
#include "cpu.h"
#include "ppu.h"
#include "timer.h"
#include "memory_map.h"

class Motherboard {
    private:
        Joypad* joypad = nullptr;
        MemoryBus* memoryBus = nullptr;
        Screen* screen = nullptr;
        PPU* ppu = nullptr;
        CPU* cpu = nullptr;
        GameboyTimer* timer = nullptr;
        
        unsigned long cycles = 0;

    public:
        Motherboard(Joypad* joypad, Screen* screen, FileSystem* fileSystem, MemoryMap* memMap);
        void insertCartridge();
        void runCycle();

};
