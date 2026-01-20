#ifndef JOYPAD_H
#define JOYPAD_H

#include <cstdint>
#include <Arduino.h>

class Joypad {

public:
    bool keyPressed = false;

public:
    void init();
    uint8_t getDPad();
    uint8_t getButtons();

private:
    uint8_t rawDPad = 0b1111;
    uint8_t rawButtons = 0b1111;
    static void joypadJob(void* args);

};

#endif