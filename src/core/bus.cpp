#include "bus.h"
#include <Arduino.h>

#define ROM_BANK_CACHE_SIZE 7

MemoryBus::MemoryBus(Joypad* jp, FileSystem* fileSystem, MemoryMap* memoryMap) {
    this->joypad = jp;
    this->fileSystem = fileSystem;
    this->memoryMap = memoryMap;

    this->vramBanks[0] = memoryMap->vram0;
    this->vramBanks[1] = memoryMap->vram1;
    this->workingRamBanks[0] = memoryMap->workRam0;
    this->workingRamBanks[1] = memoryMap->workRam1;
    this->workingRamBanks[2] = memoryMap->workRam2;
    this->workingRamBanks[3] = memoryMap->workRam3;
    this->workingRamBanks[4] = memoryMap->workRam4;
    this->workingRamBanks[5] = memoryMap->workRam5;
    this->workingRamBanks[6] = memoryMap->workRam6;
    this->workingRamBanks[7] = memoryMap->workRam7;

    this->bank0 = memoryMap->bank0;
    this->bank1 = memoryMap->bank1 - ROM_BANK_N_START;  //offset addr to not need to calculate this every time
    this->vramPointer = memoryMap->vram0 - VRAM_BEGIN;
    this->extMemory = memoryMap->extMem - EXTERNAL_RAM_BEGIN;
    this->workingRam0 = memoryMap->workRam0 - WORK_RAM_0_BEGIN;
    this->workingRamPointer = memoryMap->workRam1 - WORK_RAM_N_BEGIN;
    this->highMemory = memoryMap->highMemory - OAM_BEGIN; //offset addr to not need to calculate this every time

    this->colorRam = memoryMap->colorRam;

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
    this->bank1 = memoryMap->bank1 - 0x4000;

    uint32_t romAddr = 0x4000;
    for(int i=0; i < ROM_BANK_CACHE_SIZE; i++) {
        this->fileSystem->readRom(romAddr, 0x4000, bankCache[i].bankPointer);
        bankCache[i].bankNumber = i + 1;
        romAddr += 0x4000;
    }

    uint8_t gameType = bank0[0x147];
    if (gameType == 0x00) {
        this->mbc = new MBC0();
    }
    if (gameType == 0x13 || gameType == 0x10) {
        this->mbc = new MBC3();
    }

    highMemory[HDMA5] = 0xFF;
}

uint8_t IRAM_ATTR MemoryBus::readByte(uint16_t addr) {
    if(addr <= ROM_BAN_0_END) {
        return bank0[addr];
    }
    if(addr <= ROM_BANK_N_END) {
        return bank1[addr];
    }
    if(addr <= WORK_RAM_N_END) {
        if (addr <= VRAM_END) {
            return vramPointer[addr];
        }
        if (addr <= EXTERNAL_RAM_END) {
            if (mbc->rtcRegister != 0x00) {
                uint8_t decodedRtc = decodeRTC();
                return decodedRtc;
            }
            return extMemory[mbc->ramAddr + addr];
        }
        if (addr <= WORK_RAM_0_END) {
            return workingRam0[addr];
        }
        if (addr <= WORK_RAM_N_END) {
            return workingRamPointer[addr];
        }
    }
    switch(addr) {
        case JOYPAD_REG:
            return wireJoypad();
        case BGPD:
            return colorRam[highMemory[BGPI] & 0x3F];
        case OBPD:
            return colorRam[highMemory[OBPI] & 0x3F + 64];
        default:
            return highMemory[addr];
    }
}

uint8_t* IRAM_ATTR MemoryBus::fetchBlock(uint16_t addr) {
    if(addr <= ROM_BAN_0_END) {
        return &bank0[addr];
    }
    if(addr <= ROM_BANK_N_END) {
        return &bank1[addr];
    }
    if(addr <= WORK_RAM_N_END) {
        if (addr <= VRAM_END) {
            return &vramPointer[addr];
        }
        if (addr <= EXTERNAL_RAM_END) { //May Can RTC BUG this part??
            return &extMemory[mbc->ramAddr + addr];
        }
        if (addr <= WORK_RAM_0_END) {
            return &workingRam0[addr];
        }
        if (addr <= WORK_RAM_N_END) {
            return &workingRamPointer[addr];
        }
    }
    return &highMemory[addr];
}

uint8_t IRAM_ATTR MemoryBus::readVRam(uint16_t addr) {
    return vramPointer[addr];
}

uint8_t IRAM_ATTR MemoryBus::readVRamBank(uint16_t addr, uint8_t bank) {
    return vramBanks[bank][addr];
}

uint8_t IRAM_ATTR MemoryBus::readHighRam(uint16_t addr) {
    return highMemory[addr];
}

void IRAM_ATTR MemoryBus::writeByte(uint16_t addr, uint8_t value) {
    if (addr >= 0x8000) {
        if (addr <= VRAM_END) {
            vramPointer[addr] = value;
            return;
        }

        if (addr <= EXTERNAL_RAM_END) {
            extMemory[mbc->ramAddr + addr] = value;
            return;
        }

        if (addr <= WORK_RAM_0_END) {
            workingRam0[addr] = value;
            return;
        }
        
        if (addr <= WORK_RAM_N_END) {
            workingRamPointer[addr] = value;
            return;
        }

        if (addr == SC && (value == 0x80 || value == 0x81)) {
            highMemory[SC] = value;
            wireSerial(value);
            return;
        }

        switch(addr) {
            case DMA:
                highMemory[DMA] = value;
                dma(value);
                return;

            case TIMER_DIV:
                highMemory[TIMER_DIV] = 0x00;
                return;

            case HDMA5:
                hdma(value);
                return;
            
            case VBK:
                vramPointer = vramBanks[value & 0x01] - VRAM_BEGIN;
                highMemory[VBK] = value;
                return;
            
            case WBK: {
                uint8_t bank = value & 0b111;
                bank = bank != 0 ? bank : 0x1;
                workingRamPointer = workingRamBanks[bank] - WORK_RAM_N_BEGIN;
                highMemory[WBK] = value;
                return;
            }

            case BGPD: {
                uint8_t index = highMemory[BGPI];
                colorRam[index & 0x3F] = value;
                highMemory[BGPI] += index >> 7;
                return;
            }

            case OBPD: {
                uint8_t index = highMemory[OBPI];
                colorRam[(index & 0x3F) + 64] = value;
                highMemory[OBPI] += index >> 7;
                return;
            }

            default:
                highMemory[addr] = value;
                return;
        }

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
    for(int i = 0; i < ROM_BANK_CACHE_SIZE; i++) {
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

uint8_t MemoryBus::decodeRTC() {
    time_t timestamp = time(NULL);
    switch (mbc->rtcRegister) {
        case 0x08: return timestamp % 60;
        case 0x09: return (timestamp/60) % 60;
        case 0x0A: return (timestamp/3600) % 24;
        case 0x0B: return (timestamp/86400) % 256;
        case 0x0C: return 0x00;
        default: return 0x00;
    }
}

void IRAM_ATTR MemoryBus::writeHighMemory(uint16_t addr, uint8_t value) {
    highMemory[addr] = value;
}

void IRAM_ATTR MemoryBus::incTimerDiv() {
    uint8_t v = highMemory[TIMER_DIV];
    highMemory[TIMER_DIV] = v + 1;
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

void MemoryBus::hdma(uint8_t value) {
    uint8_t mode = calculator.verifyBit(value, 7);
    uint16_t source = (highMemory[HDMA1] << 8 | highMemory[HDMA2]) & 0xFFF0;
    uint16_t dest = ((highMemory[HDMA3] << 8 | highMemory[HDMA4]) & 0x1FF0) + VRAM_BEGIN;
    uint16_t size = ((value & 0x7F) + 1) * 0x10;

    if (mode == 0 && highMemory[HDMA5] != 0xFF) {
        highMemory[HDMA5] = 0xFF;
        return;
    }
    if (mode == 0) {
        for(int i=0; i<size; i++) {
            vramPointer[dest] = readByte(source);
            source += 1;
            dest += 1;
        }
        highMemory[HDMA5] = 0xFF;
    } else {
        highMemory[HDMA5] = size;
        hdmaDestAddr = dest;
        hdmaSourceAddr = source;
    }
}

void MemoryBus::stepHdma() {
    if (highMemory[HDMA5] != 0xFF) {
        return;
    }

    for(int i=0; i<0x10; i++) {
        vramPointer[hdmaDestAddr] = readByte(hdmaSourceAddr);
        hdmaDestAddr += 1;
        hdmaSourceAddr += 1;
    }
    highMemory[HDMA5] -= 0x10;

    if (highMemory[HDMA5] == 0) {
        highMemory[HDMA5] = 0xFF;
    }
}

void MemoryBus::wireSerial(uint8_t value) {
    uint8_t receivedByte = executeTrade(highMemory[SB]);
    if (calculator.verifyBit(highMemory[SC], 7)) {
        highMemory[SB] = receivedByte;
        highMemory[SC] = calculator.resetBit(highMemory[SC], 7);
        requestSerialInterrupt();
    }
}

uint8_t MemoryBus::executeTrade(uint8_t value) {
    switch(tradeControl.syncState) {
        case 1:
            if (value == 0x01) {
                return 0x02;
            }
            if (value == 0x61) {
                return 0x61;
            }
            if (value == 0xD1) {
                tradeControl.syncState = 2;
                tradeControl.byteCounter = 0;
                return 0xD1;
            }
            return 0x00;

        case 2:
            if (tradeControl.tradeState == 1 && value == 0x00) {
                tradeControl.tradeState = 2;
                return 0xFD;
            }
            if (tradeControl.tradeState == 2 && value == 0xFD) {
                tradeControl.tradeState = 3;
                return 0xFD;
            }
            if (tradeControl.tradeState == 3 && value != 0xFD) {
                tradeControl.tradeState = 4;
                return value;
            }
            if (tradeControl.tradeState == 4 && value == 0xFD) {
                tradeControl.tradeState = 5;
                return 0xFD;
            }
            if (tradeControl.tradeState == 5 && value != 0xFD) {
                tradeControl.tradeState = 6;
                tradeControl.byteCounter += 1;
                return value;
            }
            if (tradeControl.tradeState == 6) {
                tradeControl.byteCounter += 1;
                if (tradeControl.byteCounter >= tradeControl.dataSize) {
                    tradeControl.syncState = 3;
                    tradeControl.byteCounter = 0;
                }
                return value;
            }
            return value;
        
        case 3:
            return value;
    }
    return 0x00;
}
