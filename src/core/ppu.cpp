#include "ppu.h"
#include "bus.h"
#include "screen.h"

PPU::PPU(MemoryBus* bus, Screen* screen) {
    this->bus = bus;
    this->screen = screen;
    getLcdControl();
}

void IRAM_ATTR PPU::step(uint16_t newCycle) {
    cycles += newCycle;

    if (!decodedLcdControlRegister[7]) {
        getLcdControl();
        return;
    }

    uint8_t newMode = getScanlineMode();
    bool changed = actualMode != newMode;

    if (changed) {
        actualMode = newMode;
        getLcdControl();
        updateStat();

        if (actualMode == 0) {
            bus->stepHdma();
        }

        if (actualMode == 1) {
            renderFrame = frameRendered % FRAME_SKIP_DIVIDER == 0;
            bus->requestVblankInterrupt();
            windowLineRendered = 0;
        }

        if (renderFrame && actualMode == 2) {
            oamScan();
            oamFetch();
        }

        if (renderFrame && actualMode == 3) {
            uint8_t* scanLine = renderScanLine();
            screen->drawLineColor(lineRendered, scanLine, bus->colorRam);
        }
    }

    if (cycles >= 456) {
        getLcdControl();
        bus->writeByte(LCD_Y, lineRendered);
        statLineTrigger = false;
        lineRendered += 1;
        if (lineRendered > 153) {
            lineRendered = 0;
            frameRendered++;
        }
        cycles = cycles - 456;
    }
}

uint8_t IRAM_ATTR PPU::getScanlineMode() {
    if (lineRendered > 143) {
        return 1;
    }
    if (cycles < 80) {
        return 2;
    }
    if (cycles < 256) {
        return 3;
    }
    if (cycles < 456) {
        return 0;
    }
    
    return 0;
}

uint8_t* PPU::renderScanLine() {
    uint8_t bgOffset = renderBgLine();
    uint8_t* offsetBgBuffer = &bgBuffer[bgOffset % 8];
    if (decodedLcdControlRegister[1])
        renderObjLine(offsetBgBuffer);
    return offsetBgBuffer;
}

uint8_t PPU::renderBgLine() {
    if(!decodedLcdControlRegister[0]) {
        for(int i=0; i<168; i++) {
            bgBuffer[i] = 0x00;
        }
        return 0;
    }

    uint8_t windowStart = 160; //means no window
    if (decodedLcdControlRegister[5]) {
        windowStart = renderWindowLine();
    }

    if (windowStart == 0) {
        return 0;
    }

    uint8_t yOffset = bus->readHighRam(BG_SCROLL_Y);
    uint8_t xOffset = bus->readHighRam(BG_SCROLL_X);

    uint8_t viewportLine = (lineRendered + yOffset) % 256;
    uint8_t tileLineOffset = viewportLine % 8;
    uint8_t tileMapOffset = viewportLine / 8;
    
    boolean tileAddressing = !decodedLcdControlRegister[4];
    uint16_t tileMapStart = decodedLcdControlRegister[3] ? TILE_MAP_2_START : TILE_MAP_1_START;

    uint8_t windowStartTile = windowStart / 8;
    uint8_t windowStartPixel = windowStart % 8;

    //Create better bg window overlay, now if window is in mid tile bg will overlay it
    for(int i=0; i < windowStartTile + 1; i++) {
        uint8_t tileMapX = (i + xOffset/8) % 32;
        uint16_t tileMapAddr =  (tileMapStart + tileMapX) + (tileMapOffset * 32);
        uint8_t tileIndex = bus->readVRamBank(tileMapAddr, 0);
        uint8_t attributes = bus->readVRamBank(tileMapAddr, 1);
        uint16_t tileAddr = tileAddrResolver(tileIndex, tileAddressing);
        readTileLine(tileAddr, tileLineOffset, 8, &bgBuffer[i*8], 0x00, attributes);
    }

    return xOffset;
}

uint8_t PPU::renderWindowLine() {
    uint8_t yOffset = bus->readHighRam(W_SCROLL_Y);
    int xOffset = bus->readHighRam(W_SCROLL_X) - 7;

    if (yOffset > lineRendered || xOffset > 166) {
        return 160;
    }

    
    if (xOffset < 0) {
        xOffset = 0;
    }

    uint8_t viewportLine = windowLineRendered;
    uint8_t tileLineOffset = viewportLine % 8;
    uint8_t tileMapOffset = viewportLine / 8;
    windowLineRendered += 1;

    boolean tileAddressing = !decodedLcdControlRegister[4];
    uint16_t tileMapStart = decodedLcdControlRegister[6] ? TILE_MAP_2_START : TILE_MAP_1_START;

    uint8_t initialTile = xOffset / 8;

    for(int i=0; i < 20 - initialTile; i++) {
        uint16_t tileMapAddr = (tileMapStart + i) + (tileMapOffset * 32);
        uint8_t tileIndex = bus->readVRamBank(tileMapAddr, 0);
        uint8_t attributes = bus->readVRamBank(tileMapAddr, 1);
        uint16_t tileAddr = tileAddrResolver(tileIndex, tileAddressing);
        readTileLine(tileAddr, tileLineOffset, 8, &bgBuffer[(i + initialTile)*8], 0x00, attributes);
    }

    return xOffset;
}

void PPU::buildPalleteMap(uint16_t addr, uint8_t* palleteMap) {
    uint8_t pallete = bus->readHighRam(addr);
    palleteMap[0] = pallete & 0b11;
    palleteMap[1] = pallete >> 2 & 0b11;
    palleteMap[2] = pallete >> 4 & 0b11;
    palleteMap[3] = pallete >> 6 & 0b11;
}

void PPU::renderObjLine(uint8_t* bgLine) {
    for(int i=0; i<objectsBufferSize; i++) {
        OAMObject obj = objectsBuffer[i];
        for(uint8_t j=0; j<8; j++) {
            if(obj.xPosition >= 8 && obj.xPosition < 168) {
                if((obj.pixels[j] & 0x3) !=0) {
                    if(
                        ((bgLine[obj.xPosition - 8 + j] & 0x03) == 0) || 
                        ((bgLine[obj.xPosition - 8 + j] >> 7) == 0 && (obj.pixels[j] >> 7) == 0)) {
                            
                        bgLine[obj.xPosition - 8 + j] = obj.pixels[j];
                    }
                }
            }
        }
    }
}

void PPU::oamScan() {
    bool objExtended = decodedLcdControlRegister[2];
    uint16_t addr = OAM_START;
    uint8_t objSize = 8;
    if (objExtended) {
        objSize = 16;
    }

    objectsBufferSize = 0;
    for(int i=0; i < 40; i++) {
        if(objectsBufferSize >= 10) {
            break;
        }

        uint8_t y = bus->readHighRam(addr);
        if(lineRendered >= (y - 16) && lineRendered < (y - 16 + objSize)) {
            objectsBuffer[objectsBufferSize].yPosition = y;
            objectsBuffer[objectsBufferSize].xPosition = bus->readHighRam(addr + 1);
            objectsBuffer[objectsBufferSize].tileIndex = bus->readHighRam(addr + 2);
            objectsBuffer[objectsBufferSize].attributes = bus->readHighRam(addr + 3);
            objectsBuffer[objectsBufferSize].extendedSize = objExtended;
            objectsBuffer[objectsBufferSize].tineLine = lineRendered - (y - 16);
            objectsBufferSize += 1;
        }
        addr += 4;
    }
}

void PPU::oamFetch() {
    for(int i=0; i<objectsBufferSize; i++) {
        uint16_t tileAddr;
        uint8_t tileLine = objectsBuffer[i].tineLine;
        
        if(objectsBuffer[i].extendedSize) {
            if(objectsBuffer[i].tineLine >= 8) {
                tileAddr = tileAddrResolver(objectsBuffer[i].tileIndex | 0x01, false);
                tileLine -= 8;
            } else {
                tileAddr = tileAddrResolver(objectsBuffer[i].tileIndex & 0xFE, false);
            }
        } else {
            tileAddr = tileAddrResolver(objectsBuffer[i].tileIndex, false);
        }

        readTileLine(tileAddr, tileLine, 8, objectsBuffer[i].pixels, 0x20, objectsBuffer[i].attributes);
    }
}

uint16_t PPU::tileAddrResolver(uint8_t tileIndex, bool isSigned) {
    if (isSigned) {
        return tileIndex < 128 ? (tileIndex * 16) + TILE_DATA_BLOCK_2 : (tileIndex * 16) + TILE_DATA_BLOCK_0;
    } else {
        return (tileIndex * 16) + TILE_DATA_BLOCK_0;
    }
}

void PPU::readTileLine(uint16_t tileAddr, uint8_t lineIndex, uint8_t size, uint8_t* buffer, uint8_t palleteMask, uint8_t attributes) {
    uint8_t yInverted = calculator->verifyBit(attributes, 6);
    uint8_t xInverted = calculator->verifyBit(attributes, 5);
    uint8_t vRamBank = calculator->verifyBit(attributes, 3);

    lineIndex = !yInverted ? lineIndex : 7 - lineIndex;

    uint16_t lineAddr = tileAddr + (lineIndex * 2);
    uint8_t low  = bus->readVRamBank(lineAddr, vRamBank);
    uint8_t high = bus->readVRamBank(lineAddr + 1, vRamBank);

    uint8_t mask = 0x80;
    uint8_t shift = 7;
    for (int p = 0; p < size; p++) {
        uint8_t pixelIndex = !xInverted ? p : 7-p;
        uint8_t hiBit = (high & mask) >> shift; 
        uint8_t loBit = (low & mask) >> shift;
        buffer[pixelIndex] = (attributes & 0x80) | palleteMask | ((attributes & 0x07) << 2) | (hiBit << 1) | loBit;
        mask >>= 1;
        shift -= 1;
    }
}


void PPU::updateStat() {
    uint8_t lyc  = bus->readHighRam(LCD_YC);
    uint8_t stat = bus->readHighRam(LCD_STAT);

    uint8_t modeInd = 0x00;
    bool triggerInt = false;

    if (lyc == lineRendered && calculator->verifyBit(stat, 6) && !statLineTrigger) {
        triggerInt = true;
        statLineTrigger = true;
    }

    if (actualMode == 0 && calculator->verifyBit(stat, 3)) {
        triggerInt = true;
        modeInd = 0;
    }

    if (actualMode == 1 && calculator->verifyBit(stat, 4)) {
        triggerInt = true;
        modeInd = 1;
    }

    if (actualMode == 2 && calculator->verifyBit(stat, 5)) {
        triggerInt = true;
        modeInd = 2;
    }

    if (actualMode == 3) {
        modeInd = 3;
    }

    uint8_t lyLyc = (lyc == lineRendered) ? 0b100 : 0x0;

    stat = (stat & 0xF8) | lyLyc | modeInd;
    bus->writeByte(LCD_STAT, stat);

    if (triggerInt) {
        bus->requestStatInterrupt();
    }
}

void PPU::getLcdControl() {
    uint8_t controlRegister = bus->readHighRam(LCD_CONTROL);
    if (controlRegister != rawLcdControlRegister) {
        rawLcdControlRegister = controlRegister;
        decodedLcdControlRegister[7] = calculator->verifyBit(controlRegister, 7); //LCD_ENABLE
        decodedLcdControlRegister[6] = calculator->verifyBit(controlRegister, 6); //W_TILE_MAP
        decodedLcdControlRegister[5] = calculator->verifyBit(controlRegister, 5); //WINDOW_ENABLE
        decodedLcdControlRegister[4] = calculator->verifyBit(controlRegister, 4); //BG_W_TILES
        decodedLcdControlRegister[3] = calculator->verifyBit(controlRegister, 3); //BG_TILE_MAP
        decodedLcdControlRegister[2] = calculator->verifyBit(controlRegister, 2); //OJB_SIZE
        decodedLcdControlRegister[1] = calculator->verifyBit(controlRegister, 1); //OBJ_ENABLE
        decodedLcdControlRegister[0] = calculator->verifyBit(controlRegister, 0); //BG_W_ENABLE
    }
}
