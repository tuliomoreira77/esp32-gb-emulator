#include "Screen.h"

Screen::Screen()
: display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1) {}

void Screen::init() {
    Wire.begin(21, 22);
    Wire.setClock(400000);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (true);
    }

    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const char* title = "Simple Gameboy";
    int16_t x1, y1;
    uint16_t w, h;

    display.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);

    int x = (OLED_WIDTH  - w) / 2;
    int y = (OLED_HEIGHT - h) / 2;

    display.setCursor(x, y);
    display.println(title);

    display.display();
}

bool Screen::shadeToMono(uint8_t shade) {
    return shade != 3;
}

void Screen::drawLine(uint8_t y, const uint8_t* pixels) {
    if (y < Y_OFFSET || y >= (Y_OFFSET + OLED_HEIGHT)) {
        return;
    }

    const int oledY = y - Y_OFFSET;

    // Cada "page" tem 8 linhas (bits)
    const uint8_t page = oledY >> 3;          // / 8
    const uint8_t bitMask = 1 << (oledY & 7); // % 8

    // Pega ponteiro do buffer interno do Adafruit_SSD1306
    uint8_t* buf = display.getBuffer();

    // Layout padrão: buf[x + page * OLED_WIDTH]
    const int rowBase = page * OLED_WIDTH;

    for (int x = 0; x < OLED_WIDTH; x++) {
        const uint8_t shade = pixels[x]; // seu gbX == x
        const bool on = shadeToMono(shade);

        uint8_t& b = buf[rowBase + x];

        if (on) {
            b |= bitMask;    // seta bit
        } else {
            b &= ~bitMask;   // limpa bit
        }
    }

    if (oledY == OLED_HEIGHT - 1) {
        canDraw = true;
        //drawFrame(); // normalmente: display.display()
    }
}

void Screen::drawFrame() {
    display.display();
}
