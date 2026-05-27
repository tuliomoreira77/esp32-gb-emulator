#include "Screen.h"

Screen::Screen() : tft() {
    colorArray[0] = tft.color565(232, 252, 204);
    colorArray[1] = tft.color565(172, 212, 144);
    colorArray[2] = tft.color565(84, 140, 112);
    colorArray[3] = tft.color565(20, 44, 56);
}

void Screen::init() {
    tft.init();
    tft.setSwapBytes(true);
    tft.setRotation(0); 
    tft.fillScreen(TFT_BLACK);

    int tw = tft.width();
    int th = tft.height();

    xOff = (tw - GB_WIDTH)  / 2;
    yOff = (th - GB_HEIGHT) / 2;

    if (xOff < 0) xOff = 0;
    if (yOff < 0) yOff = 0;

    xTaskCreatePinnedToCore(displayJob, "display", 8192, this, 5, nullptr, 0);
}

void Screen::drawLine(uint8_t y, uint8_t* pixels) {
    LineJob job = {y, pixels};
    xQueueSend(lineQueue, &job, portMAX_DELAY);
}


void Screen::displayJob(void* args) {
    Screen* screen = static_cast<Screen*>(args);
    LineJob job;
    for(;;) {
        if(xQueueReceive(screen->lineQueue, &job, portMAX_DELAY)) {
            for (int x = 0; x < GB_WIDTH; x++) {
                screen->lineBuf[x] = screen->colorArray[job.buffer[x]];
            }

            screen->tft.startWrite();
            screen->tft.setAddrWindow(screen->xOff, screen->yOff + job.y, GB_WIDTH, 1);
            screen->tft.pushPixels(screen->lineBuf, GB_WIDTH);
            screen->tft.endWrite();
        }
    }
}
