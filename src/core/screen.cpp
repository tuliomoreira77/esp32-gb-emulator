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
    tft.setRotation(1); 
    tft.fillScreen(TFT_BLACK);

    int tw = tft.width();
    int th = tft.height();

    xOff = (tw - SCALED_WIDTH) / 2;
    yOff = (th - SCALED_HEIGHT) / 2;

    if (xOff < 0) xOff = 0;
    if (yOff < 0) yOff = 0;

    dmaBuffer.counter=0;
    dmaBuffer.cY=0;
    dmaBuffer.y0=0;
    dmaBuffer.bufferA = (uint16_t*) heap_caps_malloc(bufferSize, MALLOC_CAP_DMA);
    dmaBuffer.bufferB = (uint16_t*) heap_caps_malloc(bufferSize, MALLOC_CAP_DMA);
    dmaBuffer.cursor = dmaBuffer.bufferA;

    xTaskCreatePinnedToCore(displayJob, "display", 2048, this, 24, nullptr, 0);
}

void Screen::requestDrawUI() {
    isDrawingUI = true;
}

void Screen::endDrawUI() {
    isDrawingUI = false;
}

void Screen::drawGenericUI(UIElement elements[], int size) {
    tft.setTextSize(1);
    tft.setTextDatum(TL_DATUM); 

    for(int i=0; i < size; i++) {
        int pos_y = POS_Y + (i * LINE_SIZE);

        if (elements[i].selected) {
            tft.fillRect(POS_X, pos_y, UI_WIDTH, LINE_SIZE - 4, TFT_WHITE);
            tft.setTextColor(TFT_BLACK, TFT_WHITE);
        } else {
            tft.fillRect(POS_X, pos_y, UI_WIDTH, LINE_SIZE - 4, TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
        }

        tft.drawString(elements[i].text, POS_X + 10, pos_y + 6);
    }
}

void Screen::drawSaveUI() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(30, 60, 2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    tft.setTextSize(3);
    tft.println("Saving...");
}

void Screen::drawLine(uint8_t y, uint8_t* pixels) {
    LineJob job;
    job.y = y;
    memcpy(job.buffer, pixels, GB_WIDTH);
    xQueueSend(lineQueue, &job, portMAX_DELAY);
}

void Screen::drawLineColor(uint8_t y, uint8_t* pixels, uint8_t* colorPallete) {
    LineJob job;
    job.y = y;
    memcpy(job.buffer, pixels, GB_WIDTH);
    memcpy(job.palleteMap, colorPallete, PALLETE_MAP_SIZE);
    xQueueSend(lineQueue, &job, portMAX_DELAY);
}

void Screen::drawLineSync(uint8_t y, uint8_t* pixels, uint8_t* colorPallete) {
    LineJob job;
    job.y = y;
    memcpy(job.buffer, pixels, GB_WIDTH);
    memcpy(job.palleteMap, colorPallete, PALLETE_MAP_SIZE);

    renderDMA(this, job);
}


void Screen::displayJob(void* args) {
    Screen* screen = static_cast<Screen*>(args);
    LineJob job;

    for (;;) {
        if(!screen->isDrawingUI) {
            if (xQueueReceive(screen->lineQueue, &job, portMAX_DELAY)) {
                renderDMA(screen, job);
            }
        } else {
            vTaskDelay(1);
        }
    }
}

void Screen::renderDMA(Screen* screen, LineJob job) {
    if(job.y == 0) {
        screen->dmaBuffer.cY = 0;
        screen->dmaBuffer.counter = 0;
    }

    if(screen->dmaBuffer.counter == 0) {
        screen->dmaBuffer.y0 = screen->dmaBuffer.cY;
        uint16_t* writerBuffer = screen->dmaBuffer.pingPong == false ? screen->dmaBuffer.bufferA : screen->dmaBuffer.bufferB;
        screen->dmaBuffer.cursor = writerBuffer;
    }

    bool scale = job.y & 0x1;
    for (int x = 0, dx = 0; x < GB_WIDTH; x += 2, dx += 3) {
        uint16_t color1 = Screen::getPixel(job.buffer[x], job.palleteMap);
        uint16_t color2 = Screen::getPixel(job.buffer[x+1], job.palleteMap);

        if(scale) {
            uint16_t* lastLine = screen->dmaBuffer.cursor - SCALED_WIDTH;
            uint16_t* scaleLine = screen->dmaBuffer.cursor;
            uint16_t* line = screen->dmaBuffer.cursor + SCALED_WIDTH;

            line[dx] = color1;
            line[dx + 1] = Screen::blendColors(color1, color2);
            line[dx + 2] = color2;

            scaleLine[dx] = Screen::blendColors(lastLine[dx], line[dx]);
            scaleLine[dx + 1] = Screen::blendColors(lastLine[dx + 1], line[dx + 1]);
            scaleLine[dx + 2] = Screen::blendColors(lastLine[dx + 2], line[dx + 2]);
        } else {
            uint16_t* line = screen->dmaBuffer.cursor;
            line[dx] = color1;
            line[dx + 1] = Screen::blendColors(color1, color2);
            line[dx + 2] = color2;
        }
    }

    int multiplier = scale ? 2 : 1;
    screen->dmaBuffer.cursor += SCALED_WIDTH * multiplier;
    screen->dmaBuffer.counter += multiplier;
    screen->dmaBuffer.cY += multiplier;

    if(screen->dmaBuffer.counter == BUFFER_SIZE_IN_LINES) {
        uint16_t* readerBuffer = screen->dmaBuffer.pingPong == false ? screen->dmaBuffer.bufferA : screen->dmaBuffer.bufferB;
        screen->dmaBuffer.pingPong = !screen->dmaBuffer.pingPong;

        screen->tft.dmaWait();
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
        if(screen->dmaBuffer.cY == SCALED_HEIGHT) {
            screen->dmaBuffer.cY = 0;
        }
    }

    //if(y1 == SCALED_HEIGHT) {
        //vTaskDelay(1);
    //}

}

uint16_t Screen::getPixel(uint8_t encodedPixel, uint8_t* palleteMap) {
    uint8_t formatedPixel = (encodedPixel & 0x3F) << 1;
    uint16_t w = (palleteMap[formatedPixel | 0x1] << 8) | palleteMap[formatedPixel];
    return ((w & 0x001F) << 11) | ((w & 0x03E0) << 1)  | ((w >> 10) & 0x001F);
}

uint16_t Screen::blendColors(uint16_t color1, uint16_t color2) {
    return (((color1 ^ color2) & 0xF7DE) >> 1) + (color1 & color2);
}
