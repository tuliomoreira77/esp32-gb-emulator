#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <map>

enum InstructionGroup {
    NOP,
    STOP,
    ADD,
    ADDC,
    JP,
    JPR,
    AND,
    CALL,
    RET,
    ADDHL,
    ADDSP,
    CCF,
    CPL,
    CP,
    DEC,
    INC,
    DEC16,
    INC16,
    DI,
    EI,
    RETI,
    HALT,
    LD,
    LDP,
    LD16,
    LDS,
    LDH,
    OR,
    POP,
    PUSH,
    SUB,
    SUBC,
    XOR,
    RST,
    RL,
    RLC,
    RR,
    RRC,
    RLCA,
    RRCA,
    RLA,
    RRA,
    SRL,
    SRA,
    SLA,
    SWAP,
    DAA,
    BIT,
    RES,
    SET,
    SCF
};

class InstructionDefinition {

    public: 
        InstructionGroup group;
        uint8_t operandsNumber;
        uint8_t cycles;
        uint8_t def_1;
        uint8_t def_2;

    public:
        InstructionDefinition(InstructionGroup group, uint8_t def_1, uint8_t def_2, uint8_t operandsNumber, uint8_t cycles);

};

class Instruction {
    public:
        InstructionDefinition* definition = nullptr;
        uint8_t operands[2];

    public:
        Instruction(InstructionDefinition* definition, uint8_t operands[2]);
};

class InstructionDict {
    private:
        std::map<int, InstructionDefinition*> dict;
        std::map<int, InstructionDefinition*> dictPrefix;
    public:
        InstructionDict();
        InstructionDefinition* get(u_int8_t opcode, bool prefix);
};

#endif