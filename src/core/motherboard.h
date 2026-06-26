#pragma once

#include <cstdint>
#include <map>
#include "joypad.h"
#include "bus.h"
#include "cpu.h"
#include "cpu_opt.h"
#include "ppu.h"
#include "timer.h"
#include "memory_map.h"

class Motherboard {
    private:
        Joypad* joypad = nullptr;
        MemoryBus* memoryBus = nullptr;
        Screen* screen = nullptr;
        PPU* ppu = nullptr;
        CPUOpt* cpu = nullptr;
        GameboyTimer* timer = nullptr;
        FileSystem* fileSystem = nullptr;
        
        unsigned long cycles = 0;

    public:
        Motherboard(Joypad* joypad, Screen* screen, FileSystem* fileSystem, MemoryMap* memMap);
        void insertCartridge();
        uint32_t runCycle();

};
