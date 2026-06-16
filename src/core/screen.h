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
    static constexpr int SCALED_WIDTH = 240;
    static constexpr int SCALED_HEIGHT = 216;
    static constexpr int BUFFER_SIZE_IN_LINES = 20;

    Screen();
    void init();
    void drawLine(uint8_t y, uint8_t* pixels);
    void drawLineSync(uint8_t y, uint8_t* pixels);

    struct LineJob {
        uint8_t y;
        uint8_t buffer[GB_WIDTH];
    };

    struct DMABuffer {
        uint8_t y0;
        uint8_t counter;
        uint8_t size = BUFFER_SIZE_IN_LINES;
        uint16_t* bufferA;
        uint16_t* bufferB;
        uint16_t* cursor;
        bool pingPong = false;
    };

private:
    TFT_eSPI tft;
    uint16_t colorArray[4];
    QueueHandle_t lineQueue = xQueueCreate(100, sizeof(LineJob));

    int xOff = 0;
    int yOff = 0;
    uint8_t xMap[SCALED_WIDTH];
    uint8_t yMap[GB_HEIGHT + 1];

    uint16_t lineBuf[SCALED_WIDTH];
    size_t bufferSize = sizeof(uint16_t) * SCALED_WIDTH * BUFFER_SIZE_IN_LINES;
    DMABuffer dmaBuffer;
    static void displayJob(void* args);
    static void displayJobNoDMA(void* args);
    static void renderDMA(Screen* screen, LineJob job);
};
