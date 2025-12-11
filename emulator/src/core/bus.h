#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>

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

constexpr uint16_t LCD_CONTROL = 0xFF40;
constexpr uint16_t JOYPAD_REG  = 0xFF00;
constexpr uint16_t DMA         = 0xFF46;

constexpr uint16_t SB = 0xFF01;
constexpr uint16_t SC = 0xFF02;

constexpr uint16_t INTERRUPT_FLAG = 0xFF0F;
constexpr uint16_t INTERRUPT_ENABLE_REGISTER = 0xFFFF;

constexpr uint16_t LCD_STAT = 0xFF41;


class MemoryBus {
private:
    Calculator calculator;
    std::array<uint8_t, 0x10000> memory{};
    std::array<uint8_t, 0x2000> extMemory{};
    Joypad* joypad = nullptr;


private: 
    uint8_t wireJoypad();
    void dma(uint8_t addr);

public:
    MemoryBus(Joypad* jp);

    uint8_t readByte(uint16_t addr);

    void writeByte(uint16_t addr, uint8_t value);

    void incTimerDiv();

    void incTimerCounter();

    void requestTimerInterrupt();

    void requestStatInterrupt();

    void requestVblankInterrupt();

    void requestJoypadInterrupt();

    void requestSerialInterrupt();

    void clearInterruptionRequest(int bit);

    

/*
    void wire_outcome_serial(uint8_t value);
*/
    

};

#endif