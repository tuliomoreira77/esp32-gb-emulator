
#include <cstdint>

#include "screen.h"
#include "bus.h"

class OAMObject {
    public:
        uint8_t yPosition = 0;
        uint8_t xPosition = 0;
        uint8_t tileIndex = 0;
        uint8_t tineLine = 0;
        bool extendedSize = 0;
        uint8_t attributes = 0x00;
        uint8_t pixels[8];
};

class PPU {
    private:
        MemoryBus* bus;
        Screen* screen;
        Calculator* calculator = new Calculator();

        uint16_t cycles = 0;
        uint8_t lineRendered = 0;
        uint8_t rawLcdControlRegister = 0xFF;
        uint8_t actualMode = 2;
        bool statLineTrigger = false;
        bool decodedLcdControlRegister[8];
        bool renderFrame = true;

        OAMObject objectsBuffer[10];
        uint8_t objectsBufferSize = 0;

        uint8_t bgBuffer[200];

    public:
        PPU(MemoryBus* bus, Screen* screen);
        void step(uint16_t cycles);

    private:
        void getLcdControl();
        uint16_t tileAddrResolver(uint8_t tileIndex, bool isSigned);
        void readTileLine(uint16_t tileAddr, uint8_t lineIndex, uint8_t size, uint8_t* buffer);
        uint8_t getScanlineMode();
        void updateStat();
        uint8_t renderBgLine();
        uint8_t renderWindowLine();
        void oamScan();
        void oamFetch();
        void renderObjLine(uint8_t* bgLine);

        uint8_t* renderScanLine();
};