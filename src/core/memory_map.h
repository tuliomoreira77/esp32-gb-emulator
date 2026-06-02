#pragma once

#include <cstdint>

struct MemoryMap {
    uint8_t* bank0;
    uint8_t* bank1;
    uint8_t* highMemory;
    uint8_t* bank2;
    uint8_t* bank3;
    uint8_t* bank4;
    uint8_t* bank5;
    uint8_t* bank6;
    uint8_t* bank7;
    uint8_t* bank8;

    uint8_t* extMem;
};