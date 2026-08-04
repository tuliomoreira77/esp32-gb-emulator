#include "joypad.h"

namespace {
    static constexpr int PIN_BUTTON_A      = 7;
    static constexpr int PIN_BUTTON_B      = 4;
    static constexpr int PIN_BUTTON_START  = 5;
    static constexpr int PIN_BUTTON_SELECT = 6;
    static constexpr int PIN_DPAD_UP       = 16;
    static constexpr int PIN_DPAD_DOWN     = 17;
    static constexpr int PIN_DPAD_LEFT     = 18;
    static constexpr int PIN_DPAD_RIGHT    = 15;
}

uint8_t IRAM_ATTR Joypad::getDPad() {
    return rawDPad;
}

uint8_t IRAM_ATTR Joypad::getButtons() {
    return rawButtons;
}

void Joypad::init() {
    pinMode(PIN_BUTTON_A, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_B, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_START, INPUT_PULLDOWN);
    pinMode(PIN_BUTTON_SELECT, INPUT_PULLDOWN);
    pinMode(PIN_DPAD_UP, INPUT_PULLDOWN);
    pinMode(PIN_DPAD_DOWN, INPUT_PULLDOWN);
    pinMode(PIN_DPAD_LEFT, INPUT_PULLDOWN);
    pinMode(PIN_DPAD_RIGHT, INPUT_PULLDOWN);

    xTaskCreatePinnedToCore(joypadJob, "buttons", 1024, this, 3, nullptr, 0);
}


void Joypad::joypadJob(void* args) {
    Joypad* joypad = static_cast<Joypad*>(args);
    const TickType_t xDelay = pdMS_TO_TICKS(50);

    for(;;) {
        int buttonStart = digitalRead(PIN_BUTTON_START);
        int buttonSelect = digitalRead(PIN_BUTTON_SELECT);
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
        if (buttonSelect == HIGH) {
            joypad->rawButtons &= 0b1011;
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