#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

enum UIAction {
    NO_ACTION,
    LOAD_GAME,
    DUMP_SAVE
};

struct UIElement {
    const char* text;
    bool selected;
    UIAction action;
};

class Screen {
public:
    static constexpr int GB_WIDTH  = 160;
    static constexpr int GB_HEIGHT = 144;
    static constexpr int Y_OFFSET = 0;

    static constexpr int PALLETE_MAP_SIZE = 128;
    static constexpr int SCALED_WIDTH = 240;
    static constexpr int SCALED_HEIGHT = 216;
    static constexpr int BUFFER_SIZE_IN_LINES = 18;

    // UI Constants
    static constexpr int POS_X = 10;   
    static constexpr int POS_Y = 30; 
    static constexpr int LINE_SIZE = 35; 
    static constexpr int UI_WIDTH = 220;

    Screen();
    void init();
    void drawLine(uint8_t y, uint8_t* pixels);
    void drawLineColor(uint8_t y, uint8_t* pixels, uint8_t* colorPallete);
    void drawLineSync(uint8_t y, uint8_t* pixels, uint8_t* colorPallete);
    void requestDrawUI();
    void endDrawUI();
    void drawSaveUI();
    void drawGenericUI(UIElement elements[], int size);

    struct LineJob {
        uint8_t y;
        uint8_t buffer[GB_WIDTH];
        uint8_t palleteMap[PALLETE_MAP_SIZE];
    };

    struct DMABuffer {
        uint8_t cY;
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
    QueueHandle_t lineQueue = xQueueCreate(18, sizeof(LineJob));
    bool isDrawingUI = false;

    int xOff = 0;
    int yOff = 0;

    size_t bufferSize = sizeof(uint16_t) * SCALED_WIDTH * BUFFER_SIZE_IN_LINES;
    DMABuffer dmaBuffer;

    static uint16_t getPixel(uint8_t encodedPixel, uint8_t* palleteMap);
    static uint16_t blendColors(uint16_t color1, uint16_t color2);

    static void displayJob(void* args);
    static void renderDMA(Screen* screen, LineJob job);
};
