#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>

class Joypad {


public:
    uint8_t getDPad();
    uint8_t getButtons();

};

#endif