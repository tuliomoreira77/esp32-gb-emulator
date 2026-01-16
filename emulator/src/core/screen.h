#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Game Boy resolution
#define GB_WIDTH   160
#define GB_HEIGHT  144

// OLED resolution
#define OLED_WIDTH  128
#define OLED_HEIGHT 64

class Screen {
public:
    bool canDraw = false; //unsafe

    Screen();

    void init();
    void drawLine(uint8_t y, const uint8_t* pixels);
    void drawFrame();

private:
    Adafruit_SSD1306 display;

    static constexpr int X_OFFSET = 0;
    static constexpr int Y_OFFSET = 64;

    int testCount = 0;



    bool shadeToMono(uint8_t shade);
};
