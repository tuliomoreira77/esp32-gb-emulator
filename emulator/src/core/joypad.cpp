#include "joypad.h"

uint8_t Joypad::getDPad() {
    return rawDPad;
}

uint8_t Joypad::getButtons() {
    return rawButtons;
}

void Joypad::init() {
    pinMode(25, INPUT);
    pinMode(32, INPUT);
    pinMode(33, INPUT);
    pinMode(34, INPUT);
    pinMode(35, INPUT);

    xTaskCreatePinnedToCore(joypadJob, "buttons", 1024, this, 3, nullptr, 0);
}


void Joypad::joypadJob(void* args) {
    Joypad* joypad = static_cast<Joypad*>(args);
    const TickType_t xDelay = pdMS_TO_TICKS(50);

    for(;;) {
        int rawValueX = analogRead(34);
        int rawValueY = analogRead(35);
        int buttonStart = digitalRead(32);
        int buttonA = digitalRead(33);
        int buttonB = digitalRead(25);

        joypad->rawDPad = 0b1111;
        joypad->rawButtons = 0b1111;

        if(rawValueX < 1024) {
            joypad->rawDPad = joypad->rawDPad & 0b1011;
            joypad->keyPressed = true;
        }
        if(rawValueX > 3072) {
            joypad->rawDPad = joypad->rawDPad & 0b0111;
            joypad->keyPressed = true;
        }
        if(rawValueY < 1024) {
            joypad->rawDPad = joypad->rawDPad & 0b1110;
            joypad->keyPressed = true;
        }
        if(rawValueY > 3072) {
            joypad->rawDPad = joypad->rawDPad & 0b1101;
            joypad->keyPressed = true;
        }

        if(buttonStart == HIGH) {
            joypad->rawButtons = joypad->rawButtons & 0b0111;
            joypad->keyPressed = true;
        }
        if(buttonA == HIGH) {
            joypad->rawButtons = joypad->rawButtons & 0b1101;
            joypad->keyPressed = true;
        }
        if(buttonB == HIGH) {
            joypad->rawButtons = joypad->rawButtons & 0b1110;
            joypad->keyPressed = true;
        }

        vTaskDelay(xDelay);
    }
}