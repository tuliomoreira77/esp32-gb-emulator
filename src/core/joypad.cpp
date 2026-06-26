#include "joypad.h"

namespace {
    static constexpr int PIN_BUTTON_A      = 27;
    static constexpr int PIN_BUTTON_B      = 32;
    static constexpr int PIN_BUTTON_START  = 33;
    static constexpr int PIN_DPAD_UP       = 35;
    static constexpr int PIN_DPAD_DOWN     = 26;
    static constexpr int PIN_DPAD_LEFT     = 34;
    static constexpr int PIN_DPAD_RIGHT    = 25;
}

uint8_t IRAM_ATTR Joypad::getDPad() {
    return rawDPad;
}

uint8_t IRAM_ATTR Joypad::getButtons() {
    return rawButtons;
}

void Joypad::init() {
    pinMode(PIN_BUTTON_A, INPUT);
    pinMode(PIN_BUTTON_B, INPUT);
    pinMode(PIN_BUTTON_START, INPUT);
    pinMode(PIN_DPAD_UP, INPUT);
    pinMode(PIN_DPAD_DOWN, INPUT);
    pinMode(PIN_DPAD_LEFT, INPUT);
    pinMode(PIN_DPAD_RIGHT, INPUT);

    xTaskCreatePinnedToCore(joypadJob, "buttons", 1024, this, 3, nullptr, 0);
}


void Joypad::joypadJob(void* args) {
    Joypad* joypad = static_cast<Joypad*>(args);
    const TickType_t xDelay = pdMS_TO_TICKS(50);

    for(;;) {
        int buttonStart = digitalRead(PIN_BUTTON_START);
        int buttonA = digitalRead(PIN_BUTTON_A);
        int buttonB = digitalRead(PIN_BUTTON_B);
        int buttonUp = digitalRead(PIN_DPAD_UP);
        int buttonDown = digitalRead(PIN_DPAD_DOWN);
        int buttonLeft = digitalRead(PIN_DPAD_LEFT);
        int buttonRight = digitalRead(PIN_DPAD_RIGHT);

        joypad->rawDPad = 0b1111;
        joypad->rawButtons = 0b1111;

        if (buttonStart == HIGH) {
            joypad->rawButtons &= 0b0111;
            joypad->keyPressed = true;
        }
        if (buttonA == HIGH) {
            joypad->rawButtons &= 0b1110;
            joypad->keyPressed = true;
        }
        if (buttonB == HIGH) {
            joypad->rawButtons &= 0b1101;
            joypad->keyPressed = true;

            joypad->saveGame = buttonA != 0;
        }

        if (buttonRight == HIGH) {
            joypad->rawDPad &= 0b1110;
            joypad->keyPressed = true;
        }
        if (buttonLeft == HIGH) {
            joypad->rawDPad &= 0b1101;
            joypad->keyPressed = true;
        }
        if (buttonUp == HIGH) {
            joypad->rawDPad &= 0b1011;
            joypad->keyPressed = true;
        }
        if (buttonDown == HIGH) {
            joypad->rawDPad &= 0b0111;
            joypad->keyPressed = true;
        }

        vTaskDelay(xDelay);
    }
}