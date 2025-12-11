#ifndef CPU_H
#define CPU_H


#include <cstdint>
#include <map>

#include "registers.h"
#include "calculator.h"
#include "bus.h"
#include "instructions.h"

class CPU {
    private:
        uint16_t programCounter = 0x0100;
        uint16_t stackPointer = 0xfffe;
        bool interruptEnable = false;
        bool isHalt = false;
        uint16_t clockCycle = 0;

        Registers *registers;
        Calculator *alu;
        MemoryBus *bus;
        InstructionDict *instrucionDict;
        uint8_t operandsBuffer[2] = {0x00, 0x00};

        std::map<int, void (CPU::*)(Instruction*)> instHandler;

    public:
        CPU(MemoryBus *bus);
        uint16_t executeStep();
        

    private:
        Instruction* getInstruction();
        void instructionRouter(Instruction* instruction);

        void noOp(Instruction *instruction);

};


#endif