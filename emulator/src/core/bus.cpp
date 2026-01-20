#include "bus.h"

MemoryBus::MemoryBus(Joypad* jp) : joypad(jp)
{

}

void MemoryBus::insertCartridge(uint8_t* rom) {
    this->romMemory = rom;
}

uint8_t MemoryBus::readByte(uint16_t addr) {
    if(addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END)
            return intMemory[addr];

        if (addr == JOYPAD_REG)
            return wireJoypad();

        return intMemory[addr];
    }

    return romMemory[addr];
}

uint8_t* MemoryBus::fetchBlock(uint16_t addr) {
    if(addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END)
            return &intMemory[addr];
        
        return &intMemory[addr];
    }

    return &romMemory[addr];
}

uint8_t MemoryBus::readVRam(uint16_t addr) {
    return intMemory[addr];
}

void MemoryBus::writeByte(uint16_t addr, uint8_t value) {
    if (addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END) {
            intMemory[addr] = value;
            return;
        }

        if (addr == DMA) {
            dma(value);
            return;
        }

        if (addr == TIMER_DIV) {
            intMemory[addr] = 0x00;
            return;
        }
        
        intMemory[addr] = value;
        return;
    }

    if (addr < 0x2000)
        return;

    if (addr < 0x4000) {
        //cartridge->select_rom(value);
        return;
    }

    if (addr < 0x6000) {
        //cartridge->select_ram(value);
        return;
    }

    if (addr < 0x8000) {
        //cartridge->select_extra(value);
        return;
    }
}

void MemoryBus::incTimerDiv() {
    uint8_t v = intMemory[TIMER_DIV];
    intMemory[TIMER_DIV] = (v + 1) & 0xFF;
}

void MemoryBus::incTimerCounter() {
    uint8_t v = intMemory[TIMER_COUNTER] + 1;

    if (v > 0xFF) {
        requestTimerInterrupt();
        v = intMemory[TIMER_MODULO];
    }

    intMemory[TIMER_COUNTER] = v;
}

void MemoryBus::requestTimerInterrupt() {
    intMemory[INTERRUPT_FLAG] =
        calculator.setBit(intMemory[INTERRUPT_FLAG], 2);
}

void MemoryBus::requestStatInterrupt() {
    intMemory[INTERRUPT_FLAG] =
        calculator.setBit(intMemory[INTERRUPT_FLAG], 1);
}

void MemoryBus::requestVblankInterrupt() {
    intMemory[INTERRUPT_FLAG] =
        calculator.setBit(intMemory[INTERRUPT_FLAG], 0);
}

void MemoryBus::requestJoypadInterrupt() {
    intMemory[INTERRUPT_FLAG] =
        calculator.setBit(intMemory[INTERRUPT_FLAG], 4);
}

void MemoryBus::requestSerialInterrupt() {
    intMemory[INTERRUPT_FLAG] =
        calculator.setBit(intMemory[INTERRUPT_FLAG], 3);
}

void MemoryBus::clearInterruptionRequest(int bit) {
    intMemory[INTERRUPT_FLAG] =
        calculator.resetBit(intMemory[INTERRUPT_FLAG], bit);
}

uint8_t MemoryBus::wireJoypad() {
    uint8_t j = intMemory[JOYPAD_REG];
    uint8_t selector = j & 0b00110000;

    uint8_t dpad = joypad->getDPad();
    uint8_t buttons = joypad->getButtons();

    if (selector == 48)
        return 0x3F;

    if (selector == 32)
        return j | dpad;

    if (selector == 16)
        return j | buttons;

    return j;
}

void MemoryBus::dma(uint8_t addr) {
    uint16_t src = addr << 8;
    uint16_t dst = OAM_BEGIN;

    for (int i = 0; i < 160; i++)
        intMemory[dst++] = readByte(src++);
}

