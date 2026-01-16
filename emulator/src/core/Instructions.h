#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <map>
#include "reg_enums.h"

enum InstructionGroup : uint8_t {
    // ---------------- LOADS ----------------
    LD = 0,
    LDP,
    LDH,
    LD16,
    LDS,

    // ---------------- ALU 8-bit ----------------
    ADD,
    ADDC,
    SUB,
    SUBC,
    AND,
    OR,
    XOR,
    CP,
    DAA,

    // ---------------- INC / DEC ----------------
    INC,
    DEC8,
    INC16,
    DEC16,

    // ---------------- ALU 16-bit ----------------
    ADDHL,
    ADDSP,

    // ---------------- FLOW CONTROL ----------------
    JP,
    JPR,
    CALL,
    RET,
    RETI,
    RST,

    // ---------------- STACK ----------------
    PUSH,
    POP,

    // ---------------- ROTATE / SHIFT ----------------
    RL,
    RLC,
    RR,
    RRC,
    RLCA,
    RLA,
    RRCA,
    RRA,
    SLA,
    SRA,
    SRL,
    SWAP,

    // ---------------- BIT OPS ----------------
    BIT,
    SET,
    RES,

    // ---------------- FLAGS ----------------
    CCF,
    SCF,
    CPL,

    // ---------------- CPU STATE ----------------
    DI,
    EI,
    HALT,
    NOP,
    STOP,

    INSTRUCTION_GROUP_COUNT
};

class InstructionDefinition {

    public: 
        InstructionGroup group;
        uint8_t operandsNumber;
        uint8_t cycles;
        U8Reg def_1;
        U8Reg def_2;

    public:
        InstructionDefinition(InstructionGroup group, U8Reg def_1, U8Reg def_2, uint8_t operandsNumber, uint8_t cycles);

};

class Instruction {
    private:
        uint8_t operandsBuffer[2] = {0x00, 0x00};

    public:
        InstructionDefinition* definition = nullptr;
        uint8_t* operands = operandsBuffer;

    public:
        Instruction();
};

class InstructionDict {
    private:
        InstructionDefinition* dict[256];
        InstructionDefinition* dictPrefix[256];
    public:
        InstructionDict();
        InstructionDefinition* get(u_int8_t opcode, bool prefix);
};

#endif