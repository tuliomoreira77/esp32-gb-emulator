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
        bool finish = false;
        uint16_t stackPointer = 0xfffe;
        bool interruptEnable = false;
        bool isHalt = false;
        uint16_t clockCycle = 0;

        Registers *registers;
        Calculator *alu;
        MemoryBus *bus;
        InstructionDict *instrucionDict;
        Instruction *currentInstruction;

        uint16_t interruptVector[5] = {0x40,0x50,0x48,0x58,0x60};

        std::map<U8Reg, uint16_t> vectorLookup = {
            {U8Reg::V0, 0x0},
            {U8Reg::V1, 0x10},
            {U8Reg::V2, 0x20},
            {U8Reg::V3, 0x30},
            {U8Reg::V4, 0x08},
            {U8Reg::V5, 0x18},
            {U8Reg::V6, 0x28},
            {U8Reg::V7, 0x38},
        };

    public:
        CPU(MemoryBus *bus);
        uint16_t executeStep();
        

    private:
        void fetchInstruction();
        void instructionRouter(Instruction* instruction);
        uint16_t build16FromOperands(Instruction* instruction);
        void pushToStack(uint16_t addr);
        u_int16_t popFromStack();
        void shiftRotationOpr(Instruction* instruction, uint8_t (Calculator::*alu_opr)(uint8_t, uint8_t));
        void setReset(Instruction* instruction, uint8_t (Calculator::*alu_opr)(uint8_t, uint8_t));
        uint8_t verifyPendingInterrupts();
        uint8_t getFirstInterrupt(uint8_t pendingInterrupts);
        void handleInterrupts(uint8_t pendingInterrupts);
        void debugReg();
        void debugInstr(Instruction* instruction);
        void breakPoint(Instruction* Instruction);

        void noOp(Instruction *instruction);
        void addU8(Instruction *instruction);
        void addU16(Instruction* instruction);
        void addSignedSP(Instruction* instruction);
        void andU8(Instruction* instruction);
        void orU8(Instruction* instruction);
        void xorU8(Instruction* instruction);
        void subU8(Instruction* instruction);
        void decimalAdjust(Instruction* instruction);
        void compare(Instruction* instruction);
        void decrementU8(Instruction* instruction);
        void incrementU8(Instruction* instruction);
        void decrementU16(Instruction* instruction);
        void incrementU16(Instruction* instruction);
        void directJump(Instruction* instruction);
        void relativeJump(Instruction* instruction);
        void call(Instruction* instruction);
        void ret(Instruction* instruction);
        void rst(Instruction* instruction);
        void complementCarryFlag(Instruction* instruction);
        void complementAcumulator(Instruction* instruction);

        void rotateLeftA(Instruction* instruction);
        void rotateLeftACarry(Instruction* instruction);
        void rotateRightA(Instruction* instruction);
        void rotateRightACarry(Instruction* instruction);
        void rotateLeft(Instruction* instruction);
        void rotateLeftCarry(Instruction* instruction);
        void rotateRight(Instruction* instruction);
        void rotateRightCarry(Instruction* instruction);
        void shiftRightL(Instruction* instruction);
        void shiftRightA(Instruction* instruction);
        void shiftLeftA(Instruction* instruction);

        void swap(Instruction* instruction);
        void bitOp(Instruction* instruction);
        void set(Instruction* instruction);
        void reset(Instruction* instruction);
        void setCarryFlag(Instruction* instruction);

        void disableInterrupts(Instruction* instruction);
        void enableInterrupts(Instruction* instruction);
        void halt(Instruction* instruction);
        void reti(Instruction* instruction);


        void basicLoad(Instruction* instruction);
        void pointerLoad(Instruction* instruction);
        void load16(Instruction* instruction);
        void loadStack(Instruction* instruction);
        void loadHigh(Instruction* instruction);

        void pop(Instruction* instruction);
        void push(Instruction* instruction);
};


#endif