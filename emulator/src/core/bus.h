#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "calculator.h"
#include "joypad.h"

constexpr uint16_t VBLANK_VECTOR = 0x40;
constexpr uint16_t TIMER_VECTOR  = 0x50;
constexpr uint16_t LCDSTAT_VECTOR = 0x48;
constexpr uint16_t SERIAL_VECTOR  = 0x58;
constexpr uint16_t JOYPAD_VECTOR  = 0x60;

constexpr uint16_t BIOS_END = 0xFF;
constexpr uint16_t ROM_BANK_N_END = 0x7FFF;

constexpr uint16_t EXTERNAL_RAM_BEGIN = 0xA000;
constexpr uint16_t EXTERNAL_RAM_END   = 0xBFFF;

constexpr uint16_t VRAM_BEGIN = 0x8000;
constexpr uint16_t VRAM_END   = 0x9FFF;

constexpr uint16_t OAM_BEGIN = 0xFE00;
constexpr uint16_t OAM_END   = 0xFE9F;

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


class MemoryBus {
public:
    uint16_t debugReg = 0x0000;

private:
    Calculator calculator;
    uint8_t* romMemory;
    uint8_t extMemory[0x2000];
    uint8_t intMemory[0x10000];
    Joypad* joypad = nullptr;


private: 
    uint8_t wireJoypad();
    void dma(uint8_t addr);

public:
    MemoryBus(Joypad* jp);

    void insertCartridge(uint8_t* rom);

    uint8_t readByte(uint16_t addr);
    uint8_t* fetchBlock(uint16_t addr);
    uint8_t readVRam(uint16_t addr);

    void writeByte(uint16_t addr, uint8_t value);

    void incTimerDiv();

    void incTimerCounter();

    void requestTimerInterrupt();

    void requestStatInterrupt();

    void requestVblankInterrupt();

    void requestJoypadInterrupt();

    void requestSerialInterrupt();

    void clearInterruptionRequest(int bit);

};

#endif