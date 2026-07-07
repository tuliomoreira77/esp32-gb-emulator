#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <sys/time.h>
#include "calculator.h"
#include "joypad.h"
#include "filesystem.h"
#include "memory_map.h"

constexpr uint16_t VBLANK_VECTOR = 0x40;
constexpr uint16_t TIMER_VECTOR  = 0x50;
constexpr uint16_t LCDSTAT_VECTOR = 0x48;
constexpr uint16_t SERIAL_VECTOR  = 0x58;
constexpr uint16_t JOYPAD_VECTOR  = 0x60;

constexpr uint16_t BIOS_END = 0xFF;

// Memory Map
constexpr uint16_t ROM_BAN_0_END = 0x3FFF;
constexpr uint16_t ROM_BANK_N_START = 0x4000;
constexpr uint16_t ROM_BANK_N_END = 0x7FFF;
constexpr uint16_t VRAM_BEGIN = 0x8000;
constexpr uint16_t VRAM_END   = 0x9FFF;
constexpr uint16_t EXTERNAL_RAM_BEGIN = 0xA000;
constexpr uint16_t EXTERNAL_RAM_END   = 0xBFFF;
constexpr uint16_t WORK_RAM_0_BEGIN = 0xC000;
constexpr uint16_t WORK_RAM_0_END = 0xCFFF;
constexpr uint16_t WORK_RAM_N_BEGIN = 0xD000;
constexpr uint16_t WORK_RAM_N_END = 0xDFFF;
constexpr uint16_t OAM_BEGIN = 0xFE00;
constexpr uint16_t OAM_END   = 0xFE9F;
constexpr uint16_t HIGH_RAM_BEGIN = 0xFF00;


constexpr uint16_t TIMER_DIV     = 0xFF04;
constexpr uint16_t TIMER_COUNTER = 0xFF05;
constexpr uint16_t TIMER_MODULO  = 0xFF06;
constexpr uint16_t TIMER_CONTROL = 0xFF07;

constexpr uint16_t JOYPAD_REG  = 0xFF00;
constexpr uint16_t DMA         = 0xFF46;

constexpr uint16_t SB = 0xFF01;
constexpr uint16_t SC = 0xFF02;

constexpr uint16_t INTERRUPT_FLAG = 0xFF0F;
constexpr uint16_t INTERRUPT_ENABLE_REGISTER = 0xFFFF;

constexpr uint16_t TILE_MAP_1_START = 0x9800;
constexpr uint16_t TILE_MAP_1_END   = 0x9BFF;
constexpr uint16_t TILE_MAP_2_START = 0x9C00;
constexpr uint16_t TILE_MAP_2_END   = 0x9FFF;

constexpr uint16_t TILE_DATA_BLOCK_0 = 0x8000;
constexpr uint16_t TILE_DATA_BLOCK_1 = 0x8800;
constexpr uint16_t TILE_DATA_BLOCK_2 = 0x9000;

constexpr uint16_t OAM_START = 0xFE00;

constexpr uint16_t LCD_Y        = 0xFF44; // LY
constexpr uint16_t LCD_YC       = 0xFF45; // LYC
constexpr uint16_t LCD_STAT     = 0xFF41;
constexpr uint16_t LCD_CONTROL  = 0xFF40;

constexpr uint16_t BG_SCROLL_Y  = 0xFF42; // SCY
constexpr uint16_t BG_SCROLL_X  = 0xFF43; // SCX
constexpr uint16_t W_SCROLL_Y   = 0xFF4A; // WY
constexpr uint16_t W_SCROLL_X   = 0xFF4B; // WX

constexpr uint16_t BG_W_PALETTE = 0xFF47; // BGP
constexpr uint16_t OBP0         = 0xFF48;
constexpr uint16_t OBP1         = 0xFF49;

constexpr uint16_t HDMA1 = 0xFF51;
constexpr uint16_t HDMA2 = 0xFF52;
constexpr uint16_t HDMA3 = 0xFF53;
constexpr uint16_t HDMA4 = 0xFF54;
constexpr uint16_t HDMA5 = 0xFF55;

constexpr uint16_t VBK = 0xFF4F;
constexpr uint16_t WBK = 0xFF70;

constexpr uint16_t BGPI = 0xFF68;
constexpr uint16_t BGPD = 0xFF69;
constexpr uint16_t OBPI = 0xFF6A;
constexpr uint16_t OBPD = 0xFF6B;



class MBC {
    public:
        uint32_t romAddr = 0x4000;
        uint32_t ramAddr = 0x0000;
        uint8_t rtcRegister = 0x00;

        virtual void selectRom(uint8_t bank) = 0;
        virtual void selectRam(uint8_t bank) = 0;
        virtual void selectExtra(uint8_t bank) = 0;
};

class MBC0 : public MBC {
    void selectRom(uint8_t bank) override {

    }
    void selectRam(uint8_t bank) override {

    }
    void selectExtra(uint8_t bank) override {

    }
};

class MBC3 : public MBC {
    void selectRom(uint8_t bank) override {
        if (bank == 0) {
            romAddr = 0x4000;
            return;
        }

        romAddr = (bank) * 0x4000;
    }

    void selectRam(uint8_t bank) {
        if(bank > 0x07) {
            rtcRegister = bank;
            return;
        }
        
        rtcRegister = 0x00;
        ramAddr = bank * 0x2000;
    }

    void selectExtra(uint8_t bank) {
        
    }
};

struct BankCacheControl {
    uint8_t bankNumber;
    uint32_t lastUsed = 0;
    uint8_t* bankPointer = nullptr;
};

struct TradeControl {
    int dataSize = 444;
    int syncState = 1;
    int tradeState = 1;
    int byteCounter = 0;
};

class MemoryBus {
public:
    uint16_t debugReg = 0x0000;
    uint8_t* colorRam;
    uint32_t bankChangeCounter = 0;

private:
    Calculator calculator;

    MemoryMap* memoryMap;
    BankCacheControl bankCache[8];
    uint32_t accessCounter = 0;
    uint8_t* bank0;
    uint8_t* bank1;
    uint8_t* vramPointer;
    uint8_t* workingRam0;
    uint8_t* workingRamPointer;
    uint8_t* extMemory;
    uint8_t* highMemory;
    uint8_t* vramBanks[2];
    uint8_t* workingRamBanks[8];

    Joypad* joypad = nullptr;
    FileSystem* fileSystem = nullptr;
    MBC* mbc = nullptr;

    TradeControl tradeControl;

    uint16_t hdmaDestAddr = 0;
    uint16_t hdmaSourceAddr = 0;

private: 
    uint8_t wireJoypad();
    void dma(uint8_t addr);
    void hdma(uint8_t value);
    void changeRomBank(uint8_t bank);
    uint8_t decodeRTC();

public:
    MemoryBus(Joypad* jp, FileSystem* fileSystem, MemoryMap* memoryMap);

    void insertCartridge();

    uint8_t readByte(uint16_t addr);
    uint8_t* fetchBlock(uint16_t addr);
    uint8_t readVRam(uint16_t addr);
    uint8_t readHighRam(uint16_t addr);
    uint8_t readVRamBank(uint16_t addr, uint8_t bank);

    void writeByte(uint16_t addr, uint8_t value);

    void writeHighMemory(uint16_t addr, uint8_t value);

    void wireSerial(uint8_t value);

    uint8_t executeTrade(uint8_t value);

    void stepHdma();

    void incTimerDiv();

    void incTimerCounter();

    void requestTimerInterrupt();

    void requestStatInterrupt();

    void requestVblankInterrupt();

    void requestJoypadInterrupt();

    void requestSerialInterrupt();

    void clearInterruptionRequest(int bit);

    uint8_t verifyPendingInterrupts();

};

#endif