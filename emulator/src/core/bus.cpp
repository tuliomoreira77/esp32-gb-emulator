#include "bus.h"

MemoryBus::MemoryBus(Joypad* jp) : joypad(jp)
{
    memory.fill(0x00);
    extMemory.fill(0x00);
}

uint8_t MemoryBus::readByte(uint16_t addr) {
    if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END)
        return extMemory[addr];

    if (addr == JOYPAD_REG)
        return wireJoypad();

    return memory[addr];
}

void MemoryBus::writeByte(uint16_t addr, uint8_t value) {
    if (addr >= 0x8000) {
        if (addr == TIMER_DIV) {
            memory[addr] = 0x00;
            return;
        }

        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END) {
            extMemory[addr] = value;
            return;
        }

        if (addr == DMA)
            dma(value);

//        if (addr == SC && (value == 0x81 || value == 0x80))
//            wire_outcome_serial(value);

        memory[addr] = value;
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
    uint8_t v = memory[TIMER_DIV];
    memory[TIMER_DIV] = (v + 1) & 0xFF;
}

void MemoryBus::incTimerCounter() {
    uint8_t v = memory[TIMER_COUNTER] + 1;

    if (v > 0xFF) {
        requestTimerInterrupt();
        v = memory[TIMER_MODULO];
    }

    memory[TIMER_COUNTER] = v;
}

void MemoryBus::requestTimerInterrupt() {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], 2);
}

void MemoryBus::requestStatInterrupt() {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], 1);
}

void MemoryBus::requestVblankInterrupt() {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], 0);
}

void MemoryBus::requestJoypadInterrupt() {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], 4);
}

void MemoryBus::requestSerialInterrupt() {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], 3);
}

void MemoryBus::clearInterruptionRequest(int bit) {
    memory[INTERRUPT_FLAG] =
        calculator.setBit(memory[INTERRUPT_FLAG], bit);
}

uint8_t MemoryBus::wireJoypad() {
    uint8_t j = memory[JOYPAD_REG];
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

/*
void wire_outcome_serial(uint8_t value) {
    memory[SC] = value;
    serial_port->send_byte(memory[SB]);
}

void wire_incoming_serial(uint8_t value) {
    uint8_t sc = memory[SC];

    if (calculator.verify_bit(sc, 7)) {
        memory[SB] = value;
        memory[SC] = calculator.reset_bit(sc, 7);
        request_serial_interrupt();
    }
}
*/

void MemoryBus::dma(uint8_t addr) {
    uint16_t src = addr << 8;
    uint16_t dst = OAM_BEGIN;

    for (int i = 0; i < 160; i++)
        memory[dst++] = readByte(src++);
}

