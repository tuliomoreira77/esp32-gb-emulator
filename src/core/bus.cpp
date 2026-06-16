#include "bus.h"
#include <Arduino.h>

MemoryBus::MemoryBus(Joypad* jp, FileSystem* fileSystem, MemoryMap* memoryMap) {
    this->joypad = jp;
    this->fileSystem = fileSystem;
    this->memoryMap = memoryMap;

    this->highMemory = memoryMap->highMemory - 0x8000; //offset addr to not need to calculate this every time
    this->bank0 = memoryMap->bank0;
    this->bank1 = memoryMap->bank1 - 0x4000;  //offset addr to not need to calculate this every time
    this->extMemory = memoryMap->extMem;

    this->bankCache[0].bankPointer = memoryMap->bank1;
    this->bankCache[1].bankPointer = memoryMap->bank2;
    this->bankCache[2].bankPointer = memoryMap->bank3;
    this->bankCache[3].bankPointer = memoryMap->bank4;
    this->bankCache[4].bankPointer = memoryMap->bank5;
    this->bankCache[5].bankPointer = memoryMap->bank6;
    this->bankCache[6].bankPointer = memoryMap->bank7;
    this->bankCache[7].bankPointer = memoryMap->bank8;
}

void MemoryBus::insertCartridge() {
    this->fileSystem->readRom(0, 0x4000, bank0);

    this->fileSystem->readRom(0x4000, 0x4000, bankCache[0].bankPointer);
    bankCache[0].bankNumber = 1;

    this->fileSystem->readRom(0x8000, 0x4000, bankCache[1].bankPointer);
    bankCache[1].bankNumber = 2;

    this->fileSystem->readRom(0xC000, 0x4000, bankCache[2].bankPointer);
    bankCache[2].bankNumber = 3;

    this->fileSystem->readRom(0x10000, 0x4000, bankCache[3].bankPointer);
    bankCache[3].bankNumber = 4;

    this->fileSystem->readRom(0x14000, 0x4000, bankCache[4].bankPointer);
    bankCache[4].bankNumber = 5;

    this->fileSystem->readRom(0x18000, 0x4000, bankCache[5].bankPointer);
    bankCache[5].bankNumber = 6;

    this->fileSystem->readRom(0x1C000, 0x4000, bankCache[6].bankPointer);
    bankCache[6].bankNumber = 7;

    this->fileSystem->readRom(0x20000, 0x4000, bankCache[7].bankPointer);
    bankCache[7].bankNumber = 8;

    this->bank1 = memoryMap->bank1 - 0x4000;

    uint8_t gameType = bank0[0x147];
    if (gameType == 0x00) {
        this->mbc = new MBC0();
    }
    if (gameType == 0x13) {
        this->mbc = new MBC3();
    }
}

uint8_t IRAM_ATTR MemoryBus::readByte(uint16_t addr) {
    if(addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END)
            return extMemory[mbc->ramAddr + (addr - EXTERNAL_RAM_BEGIN)];

        if (addr == JOYPAD_REG)
            return wireJoypad();

        return highMemory[addr];
    }
    if(addr >= 0x4000) 
        return bank1[addr];

    return bank0[addr];
}

uint8_t* IRAM_ATTR MemoryBus::fetchBlock(uint16_t addr) {
    if(addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END)
            return &extMemory[mbc->ramAddr + (addr - EXTERNAL_RAM_BEGIN)];
        
        return &highMemory[addr];
    }

    if(addr >= 0x4000) 
        return &bank1[addr];

    return &bank0[addr];
}

uint8_t IRAM_ATTR MemoryBus::readVRam(uint16_t addr) {
    return highMemory[addr];
}

void IRAM_ATTR MemoryBus::writeByte(uint16_t addr, uint8_t value) {
    if (addr >= 0x8000) {
        if (addr >= EXTERNAL_RAM_BEGIN && addr <= EXTERNAL_RAM_END) {
            extMemory[mbc->ramAddr + (addr - EXTERNAL_RAM_BEGIN)] = value;
            return;
        }

        if (addr == DMA) {
            dma(value);
            return;
        }

        if (addr == TIMER_DIV) {
            highMemory[addr] = 0x00;
            return;
        }
        
        highMemory[addr] = value;
        return;
    }

    if (addr < 0x2000)
        return;

    if (addr < 0x4000) {
        changeRomBank(value);
        return;
    }

    if (addr < 0x6000) {
        mbc->selectRam(value);
        return;
    }

    if (addr < 0x8000) {
        //cartridge->select_extra(value);
        return;
    }
}

void MemoryBus::changeRomBank(uint8_t bank) {
    bank = bank == 0 ? 1 : bank;

    uint32_t actualAddr = mbc->romAddr;
    mbc->selectRom(bank);
    if(actualAddr == mbc->romAddr) {
        return;
    }

    BankCacheControl* leastUsedBank = &bankCache[0];
    for(int i = 0; i < 8; i++) {
        if(bankCache[i].bankNumber == bank) {
            bankCache[i].lastUsed = ++accessCounter;
            this->bank1 = bankCache[i].bankPointer - 0x4000;
            return;
        }

        if(bankCache[i].lastUsed < leastUsedBank->lastUsed) {
            leastUsedBank = &bankCache[i];
        }
    }

    this->fileSystem->readRom(mbc->romAddr, 0x4000, leastUsedBank->bankPointer);
    leastUsedBank->bankNumber = bank;
    leastUsedBank->lastUsed = ++accessCounter;
    this->bank1 = leastUsedBank->bankPointer - 0x4000;
}

void IRAM_ATTR MemoryBus::incTimerDiv() {
    uint8_t v = highMemory[TIMER_DIV];
    highMemory[TIMER_DIV] = (v + 1) & 0xFF;
}

void IRAM_ATTR MemoryBus::incTimerCounter() {
    uint8_t v = highMemory[TIMER_COUNTER] + 1;

    if (v == 0x00) {
        requestTimerInterrupt();
        v = highMemory[TIMER_MODULO];
    }

    highMemory[TIMER_COUNTER] = v;
}

void IRAM_ATTR MemoryBus::requestTimerInterrupt() {
    highMemory[INTERRUPT_FLAG] =
    calculator.setBit(highMemory[INTERRUPT_FLAG], 2);
}

void IRAM_ATTR MemoryBus::requestStatInterrupt() {
    highMemory[INTERRUPT_FLAG] =
    calculator.setBit(highMemory[INTERRUPT_FLAG], 1);
}

void IRAM_ATTR MemoryBus::requestVblankInterrupt() {
    highMemory[INTERRUPT_FLAG] =
    calculator.setBit(highMemory[INTERRUPT_FLAG], 0);
}

void IRAM_ATTR MemoryBus::requestJoypadInterrupt() {
    highMemory[INTERRUPT_FLAG] =
    calculator.setBit(highMemory[INTERRUPT_FLAG], 4);
}

void IRAM_ATTR MemoryBus::requestSerialInterrupt() {
    highMemory[INTERRUPT_FLAG] =
    calculator.setBit(highMemory[INTERRUPT_FLAG], 3);
}

void IRAM_ATTR MemoryBus::clearInterruptionRequest(int bit) {
    highMemory[INTERRUPT_FLAG] =
    calculator.resetBit(highMemory[INTERRUPT_FLAG], bit);
}

uint8_t IRAM_ATTR MemoryBus::verifyPendingInterrupts() {
    uint8_t interruptEnableReg = highMemory[INTERRUPT_ENABLE_REGISTER];
    uint8_t interruptFlag = highMemory[INTERRUPT_FLAG];
    return interruptEnableReg & interruptFlag;
}


inline uint8_t IRAM_ATTR MemoryBus::wireJoypad() {
    uint8_t j = highMemory[JOYPAD_REG];
    uint8_t selector = j & 0b00110000;

    if (selector == 48)
        return 0x3F;

    if (selector == 32)
        return j | joypad->getDPad();

    if (selector == 16)
        return j | joypad->getButtons();

    return j;
}

void IRAM_ATTR MemoryBus::dma(uint8_t addr) {
    uint16_t src = addr << 8;
    uint16_t dst = OAM_BEGIN;

    for (int i = 0; i < 160; i++)
        highMemory[dst++] = readByte(src++);
}
