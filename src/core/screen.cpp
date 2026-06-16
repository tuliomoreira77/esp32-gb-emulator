#include "Screen.h"

Screen::Screen() : tft() {
    colorArray[0] = tft.color565(232, 252, 204);
    colorArray[1] = tft.color565(172, 212, 144);
    colorArray[2] = tft.color565(84, 140, 112);
    colorArray[3] = tft.color565(20, 44, 56);
}

void Screen::init() {
    tft.init();
    tft.initDMA(true);
    tft.setSwapBytes(true);
    tft.setRotation(0); 
    tft.fillScreen(TFT_BLACK);

    int tw = tft.width();
    int th = tft.height();

    xOff = (tw - SCALED_WIDTH) / 2;
    yOff = (th - SCALED_HEIGHT) / 2;

    if (xOff < 0) xOff = 0;
    if (yOff < 0) yOff = 0;

    for (int dx = 0; dx < SCALED_WIDTH; dx++) {
        xMap[dx] = (dx * GB_WIDTH) / SCALED_WIDTH;
    }
    for (int y = 0; y <= GB_HEIGHT; y++) {
        yMap[y] = (y * SCALED_HEIGHT) / GB_HEIGHT;
    }

    dmaBuffer.counter=0;
    dmaBuffer.y0=0;
    dmaBuffer.bufferA = (uint16_t*) heap_caps_malloc(bufferSize, MALLOC_CAP_DMA);
    dmaBuffer.bufferB = (uint16_t*) heap_caps_malloc(bufferSize, MALLOC_CAP_DMA);
    dmaBuffer.cursor = dmaBuffer.bufferA;

    xTaskCreatePinnedToCore(displayJob, "display", 2048, this, 5, nullptr, 0);
}

void Screen::drawLine(uint8_t y, uint8_t* pixels) {
    LineJob job;
    job.y = y;
    memcpy(job.buffer, pixels, GB_WIDTH);
    xQueueSend(lineQueue, &job, portMAX_DELAY);
}

void Screen::drawLineSync(uint8_t y, uint8_t* pixels) {
    LineJob job;
    job.y = y;
    memcpy(job.buffer, pixels, GB_WIDTH);

    renderDMA(this, job);
}


void Screen::displayJob(void* args) {
    Screen* screen = static_cast<Screen*>(args);
    LineJob job;

    for (;;) {
        if (xQueueReceive(screen->lineQueue, &job, portMAX_DELAY)) {
            renderDMA(screen, job);
        }
    }
}

void Screen::renderDMA(Screen* screen, LineJob job) {
    for (int dx = 0; dx < SCALED_WIDTH; dx++) {
        screen->lineBuf[dx] = screen->colorArray[job.buffer[screen->xMap[dx]]];
    }

    int y0 = screen->yMap[job.y];
    int y1 = screen->yMap[job.y + 1];
    int linesToDraw = y1 - y0;

    if(screen->dmaBuffer.counter == 0) {
        screen->dmaBuffer.y0 = y0;
        uint16_t* writerBuffer = screen->dmaBuffer.pingPong == false ? screen->dmaBuffer.bufferA : screen->dmaBuffer.bufferB;
        screen->dmaBuffer.cursor = writerBuffer;
    }

    for (int i = 0; i < linesToDraw; i++) {
        memcpy(screen->dmaBuffer.cursor, screen->lineBuf, sizeof(uint16_t) * SCALED_WIDTH);

        screen->dmaBuffer.cursor = screen->dmaBuffer.cursor + SCALED_WIDTH;
        screen->dmaBuffer.counter = screen->dmaBuffer.counter + 1;
    }

    if(screen->dmaBuffer.counter >= BUFFER_SIZE_IN_LINES - 1 || y1 == SCALED_HEIGHT) {
        uint16_t* readerBuffer = screen->dmaBuffer.pingPong == false ? screen->dmaBuffer.bufferA : screen->dmaBuffer.bufferB;
        screen->dmaBuffer.pingPong = !screen->dmaBuffer.pingPong;

        screen->tft.setAddrWindow(
            screen->xOff,
            screen->yOff + screen->dmaBuffer.y0,
            SCALED_WIDTH,
            screen->dmaBuffer.counter
        );
        screen->tft.pushPixelsDMA(
            readerBuffer,
            SCALED_WIDTH * screen->dmaBuffer.counter
        );
        screen->dmaBuffer.counter = 0;
    }

}


void Screen::displayJobNoDMA(void* args) {
    Screen* screen = static_cast<Screen*>(args);
    LineJob job;

    for (;;) {
        if (xQueueReceive(screen->lineQueue, &job, portMAX_DELAY)) {
            for (int dx = 0; dx < SCALED_WIDTH; dx++) {
                screen->lineBuf[dx] = screen->colorArray[job.buffer[screen->xMap[dx]]];
            }

            int y0 = screen->yMap[job.y];
            int y1 = screen->yMap[job.y + 1];
            int linesToDraw = y1 - y0;

            screen->tft.startWrite();
            screen->tft.setAddrWindow(
                screen->xOff,
                screen->yOff + y0,
                SCALED_WIDTH,
                linesToDraw
            );
            for (int i = 0; i < linesToDraw; i++) {
                screen->tft.pushPixels(
                    screen->lineBuf,
                    SCALED_WIDTH
                );
            }
            screen->tft.endWrite();
        }
    }
}
