#pragma once

#include <cstdint>

struct MemoryMap {
    uint8_t* bank0;
    uint8_t* bank1;
    uint8_t* vram0;
    uint8_t* vram1;
    uint8_t* extMem;
    uint8_t* workRam0;
    uint8_t* workRam1;
    uint8_t* workRam2;
    uint8_t* workRam3;
    uint8_t* workRam4;
    uint8_t* workRam5;
    uint8_t* workRam6;
    uint8_t* workRam7;
    uint8_t* highMemory;
    uint8_t* bank2;
    uint8_t* bank3;
    uint8_t* bank4;
    uint8_t* bank5;
    uint8_t* bank6;
    uint8_t* bank7;
    uint8_t* bank8;
    uint8_t* colorRam;

    uint8_t* gameRom;
};