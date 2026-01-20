#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static constexpr int GB_WIDTH  = 160;
static constexpr int GB_HEIGHT = 144;

static constexpr int Y_OFFSET = 0;

class Screen {
public:
    Screen();
    void init();
    void drawLine(uint8_t y, uint8_t* pixels);

    struct LineJob {
        uint8_t y;
        uint8_t* buffer;
    };

private:
    TFT_eSPI tft;
    uint16_t colorArray[3];
    QueueHandle_t lineQueue = xQueueCreate(1, sizeof(LineJob));

    int xOff = 0;
    int yOff = 0;

    uint16_t lineBuf[GB_WIDTH];
    static void displayJob(void* args);
};
