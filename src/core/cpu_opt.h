#ifndef CPUOPT_H
#define CPUOPT_H

#include <cstdint>
#include "registers.h"
#include "calculator.h"
#include "bus.h"

class CPUOpt {
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
        typedef void (CPUOpt::*InstructionHandler)(uint8_t*);

        void debugReg();

    public:
        CPUOpt(MemoryBus *bus);
        uint16_t executeStep();
        

    private:
        void pushToStack(uint16_t addr);
        uint16_t popFromStack();
        uint8_t getFirstInterrupt(uint8_t pendingInterrupts);
        void handleInterrupts(uint8_t pendingInterrupts);

        void handleInstruction();

        void noOp(uint8_t* instr);

        void load16Bc(uint8_t* instr);
        void load16De(uint8_t* instr);
        void load16Hl(uint8_t* instr);
        void load16Sp(uint8_t* instr);

        void baseAdd(uint8_t operand, bool addc);
        void addB(uint8_t* instr);
        void addC(uint8_t* instr);
        void addD(uint8_t* instr);
        void addE(uint8_t* instr);
        void addH(uint8_t* instr);
        void addL(uint8_t* instr);
        void addHL(uint8_t* instr);
        void addA(uint8_t* instr);
        void addD8(uint8_t* instr);

        void addCarryB(uint8_t* instr);
        void addCarryC(uint8_t* instr);
        void addCarryD(uint8_t* instr);
        void addCarryE(uint8_t* instr);
        void addCarryH(uint8_t* instr);
        void addCarryL(uint8_t* instr);
        void addCarryHL(uint8_t* instr);
        void addCarryA(uint8_t* instr);
        void addCarryD8(uint8_t* instr);

        void baseAnd(uint8_t operand); 
        void andB(uint8_t* instr);
        void andC(uint8_t* instr);
        void andD(uint8_t* instr);
        void andE(uint8_t* instr);
        void andH(uint8_t* instr);
        void andL(uint8_t* instr);
        void andHL(uint8_t* instr);
        void andA(uint8_t* instr);
        void andD8(uint8_t* instr);

        void baseOr(uint8_t operand);
        void orB(uint8_t* instr);
        void orC(uint8_t* instr);
        void orD(uint8_t* instr);
        void orE(uint8_t* instr);
        void orH(uint8_t* instr);
        void orL(uint8_t* instr);
        void orHL(uint8_t* instr);
        void orA(uint8_t* instr);
        void orD8(uint8_t* instr);

        void baseSub(uint8_t operand, bool subc);
        void subB(uint8_t* instr);
        void subC(uint8_t* instr);
        void subD(uint8_t* instr);
        void subE(uint8_t* instr);
        void subH(uint8_t* instr);
        void subL(uint8_t* instr);
        void subHL(uint8_t* instr);
        void subA(uint8_t* instr);
        void subD8(uint8_t* instr);

        void subCarryB(uint8_t* instr);
        void subCarryC(uint8_t* instr);
        void subCarryD(uint8_t* instr);
        void subCarryE(uint8_t* instr);
        void subCarryH(uint8_t* instr);
        void subCarryL(uint8_t* instr);
        void subCarryHL(uint8_t* instr);
        void subCarryA(uint8_t* instr);
        void subCarryD8(uint8_t* instr);

        void baseXor(uint8_t operand);
        void xorB(uint8_t* instr);
        void xorC(uint8_t* instr);
        void xorD(uint8_t* instr);
        void xorE(uint8_t* instr);
        void xorH(uint8_t* instr);
        void xorL(uint8_t* instr);
        void xorHL(uint8_t* instr);
        void xorA(uint8_t* instr);
        void xorD8(uint8_t* instr);

        void jumpHl(uint8_t* instr);
        void jumpD16(uint8_t* instr);
        void jumpNZ(uint8_t* instr);
        void jumpNC(uint8_t* instr);
        void jumpZ(uint8_t* instr);
        void jumpCR(uint8_t* instr);

        void jumpRelD16(uint8_t* instr);
        void jumpRelNZ(uint8_t* instr);
        void jumpRelNC(uint8_t* instr);
        void jumpRelZ(uint8_t* instr);
        void jumpRelCR(uint8_t* instr);

        void callD16(uint8_t* instr);
        void callNZ(uint8_t* instr);
        void callNC(uint8_t* instr);
        void callZ(uint8_t* instr);
        void callCR(uint8_t* instr);

        void retD16(uint8_t* instr);
        void retNZ(uint8_t* instr);
        void retNC(uint8_t* instr);
        void retZ(uint8_t* instr);
        void retCR(uint8_t* instr);

        void baseAddHL(uint16_t operand);
        void addHlBc(uint8_t* instr);
        void addHlDe(uint8_t* instr);
        void addHlHl(uint8_t* instr);
        void addHlSp(uint8_t* instr);

        void addSp(uint8_t* instr);

        void ccf(uint8_t* instr);
        void cpl(uint8_t* instr);
        void daa(uint8_t* instr);
        void di(uint8_t* instr);
        void ei(uint8_t* instr);
        void halt(uint8_t* instr);
        void reti(uint8_t* instr);
        void sfc(uint8_t* instr);

        void rlca(uint8_t* instr);
        void rla(uint8_t* instr);
        void rrca(uint8_t* instr);
        void rra(uint8_t* instr);

        void baseCompare(uint8_t operand);
        void compareB(uint8_t* instr);
        void compareC(uint8_t* instr);
        void compareD(uint8_t* instr);
        void compareE(uint8_t* instr);
        void compareH(uint8_t* instr);
        void compareL(uint8_t* instr);
        void compareHL(uint8_t* instr);
        void compareA(uint8_t* instr);
        void compareD8(uint8_t* instr);

        uint8_t baseDecrement(uint8_t operand);
        void decB(uint8_t* instr);
        void decC(uint8_t* instr);
        void decD(uint8_t* instr);
        void decE(uint8_t* instr);
        void decH(uint8_t* instr);
        void decL(uint8_t* instr);
        void decHL(uint8_t* instr);
        void decA(uint8_t* instr);

        void dec16HL(uint8_t* instr);
        void dec16BC(uint8_t* instr);
        void dec16DE(uint8_t* instr);
        void dec16SP(uint8_t* instr);

        uint8_t baseIncrement(uint8_t operand);
        void incB(uint8_t* instr);
        void incC(uint8_t* instr);
        void incD(uint8_t* instr);
        void incE(uint8_t* instr);
        void incH(uint8_t* instr);
        void incL(uint8_t* instr);
        void incHL(uint8_t* instr);
        void incA(uint8_t* instr);

        void inc16HL(uint8_t* instr);
        void inc16BC(uint8_t* instr);
        void inc16DE(uint8_t* instr);
        void inc16SP(uint8_t* instr);

        void load_A_A(uint8_t* instr);
        void load_A_B(uint8_t* instr);
        void load_A_C(uint8_t* instr);
        void load_A_D(uint8_t* instr);
        void load_A_E(uint8_t* instr);
        void load_A_H(uint8_t* instr);
        void load_A_L(uint8_t* instr);

        void load_B_A(uint8_t* instr);
        void load_B_B(uint8_t* instr);
        void load_B_C(uint8_t* instr);
        void load_B_D(uint8_t* instr);
        void load_B_E(uint8_t* instr);
        void load_B_H(uint8_t* instr);
        void load_B_L(uint8_t* instr);

        void load_C_A(uint8_t* instr);
        void load_C_B(uint8_t* instr);
        void load_C_C(uint8_t* instr);
        void load_C_D(uint8_t* instr);
        void load_C_E(uint8_t* instr);
        void load_C_H(uint8_t* instr);
        void load_C_L(uint8_t* instr);

        void load_D_A(uint8_t* instr);
        void load_D_B(uint8_t* instr);
        void load_D_C(uint8_t* instr);
        void load_D_D(uint8_t* instr);
        void load_D_E(uint8_t* instr);
        void load_D_H(uint8_t* instr);
        void load_D_L(uint8_t* instr);

        void load_E_A(uint8_t* instr);
        void load_E_B(uint8_t* instr);
        void load_E_C(uint8_t* instr);
        void load_E_D(uint8_t* instr);
        void load_E_E(uint8_t* instr);
        void load_E_H(uint8_t* instr);
        void load_E_L(uint8_t* instr);

        void load_H_A(uint8_t* instr);
        void load_H_B(uint8_t* instr);
        void load_H_C(uint8_t* instr);
        void load_H_D(uint8_t* instr);
        void load_H_E(uint8_t* instr);
        void load_H_H(uint8_t* instr);
        void load_H_L(uint8_t* instr);

        void load_L_A(uint8_t* instr);
        void load_L_B(uint8_t* instr);
        void load_L_C(uint8_t* instr);
        void load_L_D(uint8_t* instr);
        void load_L_E(uint8_t* instr);
        void load_L_H(uint8_t* instr);
        void load_L_L(uint8_t* instr);

        void load_HL_A(uint8_t* instr);
        void load_HL_B(uint8_t* instr);
        void load_HL_C(uint8_t* instr);
        void load_HL_D(uint8_t* instr);
        void load_HL_E(uint8_t* instr);
        void load_HL_H(uint8_t* instr);
        void load_HL_L(uint8_t* instr);

        void load_A_HL(uint8_t* instr);
        void load_B_HL(uint8_t* instr);
        void load_C_HL(uint8_t* instr);
        void load_D_HL(uint8_t* instr);
        void load_E_HL(uint8_t* instr);
        void load_H_HL(uint8_t* instr);
        void load_L_HL(uint8_t* instr);

        void load_A_D8(uint8_t* instr);
        void load_B_D8(uint8_t* instr);
        void load_C_D8(uint8_t* instr);
        void load_D_D8(uint8_t* instr);
        void load_E_D8(uint8_t* instr);
        void load_H_D8(uint8_t* instr);
        void load_L_D8(uint8_t* instr);
        void load_HL_D8(uint8_t* instr);

        void loadPointer_BC_A(uint8_t* instr);
        void loadPointer_DE_A(uint8_t* instr);
        void loadPointer_A_BC(uint8_t* instr);
        void loadPointer_A_DE(uint8_t* instr);
        void loadPointer_A_D16(uint8_t* instr);
        void loadPointer_D16_A(uint8_t* instr);
        void loadPointer_HLI_A(uint8_t* instr);
        void loadPointer_HLD_A(uint8_t* instr);
        void loadPointer_A_HLI(uint8_t* instr);
        void loadPointer_A_HLD(uint8_t* instr);

        void loadStack_D16_SP(uint8_t* instr);
        void loadStack_SP_HL(uint8_t* instr);
        void loadStack_HL_SPe(uint8_t* instr);

        void loadHigh_A_D8(uint8_t* instr);
        void loadHigh_A_C(uint8_t* instr);
        void loadHigh_C_A(uint8_t* instr);
        void loadHigh_D8_A(uint8_t* instr);

        void popAF(uint8_t* instr);
        void popBC(uint8_t* instr);
        void popDE(uint8_t* instr);
        void popHL(uint8_t* instr);

        void pushAF(uint8_t* instr);
        void pushBC(uint8_t* instr);
        void pushDE(uint8_t* instr);
        void pushHL(uint8_t* instr);

        void rstV0(uint8_t* instr);
        void rstV1(uint8_t* instr);
        void rstV2(uint8_t* instr);
        void rstV3(uint8_t* instr);
        void rstV4(uint8_t* instr);
        void rstV5(uint8_t* instr);
        void rstV6(uint8_t* instr);
        void rstV7(uint8_t* instr);


        uint8_t baseRL(uint8_t operand);
        void rlA(uint8_t* instr);
        void rlB(uint8_t* instr);
        void rlC(uint8_t* instr);
        void rlD(uint8_t* instr);
        void rlE(uint8_t* instr);
        void rlH(uint8_t* instr);
        void rlL(uint8_t* instr);
        void rlHL(uint8_t* instr);

        uint8_t baseRLC(uint8_t operand);
        void rlcA(uint8_t* instr);
        void rlcB(uint8_t* instr);
        void rlcC(uint8_t* instr);
        void rlcD(uint8_t* instr);
        void rlcE(uint8_t* instr);
        void rlcH(uint8_t* instr);
        void rlcL(uint8_t* instr);
        void rlcHL(uint8_t* instr);

        uint8_t baseRR(uint8_t operand);
        void rrA(uint8_t* instr);
        void rrB(uint8_t* instr);
        void rrC(uint8_t* instr);
        void rrD(uint8_t* instr);
        void rrE(uint8_t* instr);
        void rrH(uint8_t* instr);
        void rrL(uint8_t* instr);
        void rrHL(uint8_t* instr);

        uint8_t baseRRC(uint8_t operand);
        void rrcA(uint8_t* instr);
        void rrcB(uint8_t* instr);
        void rrcC(uint8_t* instr);
        void rrcD(uint8_t* instr);
        void rrcE(uint8_t* instr);
        void rrcH(uint8_t* instr);
        void rrcL(uint8_t* instr);
        void rrcHL(uint8_t* instr);

        void baseBit(uint8_t operand, uint8_t bit);
        void bit0A(uint8_t* instr);
        void bit0B(uint8_t* instr);
        void bit0C(uint8_t* instr);
        void bit0D(uint8_t* instr);
        void bit0E(uint8_t* instr);
        void bit0H(uint8_t* instr);
        void bit0L(uint8_t* instr);
        void bit0HL(uint8_t* instr);

        void bit1A(uint8_t* instr);
        void bit1B(uint8_t* instr);
        void bit1C(uint8_t* instr);
        void bit1D(uint8_t* instr);
        void bit1E(uint8_t* instr);
        void bit1H(uint8_t* instr);
        void bit1L(uint8_t* instr);
        void bit1HL(uint8_t* instr);

        void bit2A(uint8_t* instr);
        void bit2B(uint8_t* instr);
        void bit2C(uint8_t* instr);
        void bit2D(uint8_t* instr);
        void bit2E(uint8_t* instr);
        void bit2H(uint8_t* instr);
        void bit2L(uint8_t* instr);
        void bit2HL(uint8_t* instr);

        void bit3A(uint8_t* instr);
        void bit3B(uint8_t* instr);
        void bit3C(uint8_t* instr);
        void bit3D(uint8_t* instr);
        void bit3E(uint8_t* instr);
        void bit3H(uint8_t* instr);
        void bit3L(uint8_t* instr);
        void bit3HL(uint8_t* instr);

        void bit4A(uint8_t* instr);
        void bit4B(uint8_t* instr);
        void bit4C(uint8_t* instr);
        void bit4D(uint8_t* instr);
        void bit4E(uint8_t* instr);
        void bit4H(uint8_t* instr);
        void bit4L(uint8_t* instr);
        void bit4HL(uint8_t* instr);

        void bit5A(uint8_t* instr);
        void bit5B(uint8_t* instr);
        void bit5C(uint8_t* instr);
        void bit5D(uint8_t* instr);
        void bit5E(uint8_t* instr);
        void bit5H(uint8_t* instr);
        void bit5L(uint8_t* instr);
        void bit5HL(uint8_t* instr);

        void bit6A(uint8_t* instr);
        void bit6B(uint8_t* instr);
        void bit6C(uint8_t* instr);
        void bit6D(uint8_t* instr);
        void bit6E(uint8_t* instr);
        void bit6H(uint8_t* instr);
        void bit6L(uint8_t* instr);
        void bit6HL(uint8_t* instr);

        void bit7A(uint8_t* instr);
        void bit7B(uint8_t* instr);
        void bit7C(uint8_t* instr);
        void bit7D(uint8_t* instr);
        void bit7E(uint8_t* instr);
        void bit7H(uint8_t* instr);
        void bit7L(uint8_t* instr);
        void bit7HL(uint8_t* instr);

        uint8_t baseSRL(uint8_t operand);
        void srlA(uint8_t* instr);
        void srlB(uint8_t* instr);
        void srlC(uint8_t* instr);
        void srlD(uint8_t* instr);
        void srlE(uint8_t* instr);
        void srlH(uint8_t* instr);
        void srlL(uint8_t* instr);
        void srlHL(uint8_t* instr);

        uint8_t baseSwap(uint8_t operand);
        void swapA(uint8_t* instr);
        void swapB(uint8_t* instr);
        void swapC(uint8_t* instr);
        void swapD(uint8_t* instr);
        void swapE(uint8_t* instr);
        void swapH(uint8_t* instr);
        void swapL(uint8_t* instr);
        void swapHL(uint8_t* instr);

        void res0A(uint8_t* instr);
        void res0B(uint8_t* instr);
        void res0C(uint8_t* instr);
        void res0D(uint8_t* instr);
        void res0E(uint8_t* instr);
        void res0H(uint8_t* instr);
        void res0L(uint8_t* instr);
        void res0HL(uint8_t* instr);


        void res1A(uint8_t* instr);
        void res1B(uint8_t* instr);
        void res1C(uint8_t* instr);
        void res1D(uint8_t* instr);
        void res1E(uint8_t* instr);
        void res1H(uint8_t* instr);
        void res1L(uint8_t* instr);
        void res1HL(uint8_t* instr);


        void res2A(uint8_t* instr);
        void res2B(uint8_t* instr);
        void res2C(uint8_t* instr);
        void res2D(uint8_t* instr);
        void res2E(uint8_t* instr);
        void res2H(uint8_t* instr);
        void res2L(uint8_t* instr);
        void res2HL(uint8_t* instr);


        void res3A(uint8_t* instr);
        void res3B(uint8_t* instr);
        void res3C(uint8_t* instr);
        void res3D(uint8_t* instr);
        void res3E(uint8_t* instr);
        void res3H(uint8_t* instr);
        void res3L(uint8_t* instr);
        void res3HL(uint8_t* instr);


        void res4A(uint8_t* instr);
        void res4B(uint8_t* instr);
        void res4C(uint8_t* instr);
        void res4D(uint8_t* instr);
        void res4E(uint8_t* instr);
        void res4H(uint8_t* instr);
        void res4L(uint8_t* instr);
        void res4HL(uint8_t* instr);


        void res5A(uint8_t* instr);
        void res5B(uint8_t* instr);
        void res5C(uint8_t* instr);
        void res5D(uint8_t* instr);
        void res5E(uint8_t* instr);
        void res5H(uint8_t* instr);
        void res5L(uint8_t* instr);
        void res5HL(uint8_t* instr);


        void res6A(uint8_t* instr);
        void res6B(uint8_t* instr);
        void res6C(uint8_t* instr);
        void res6D(uint8_t* instr);
        void res6E(uint8_t* instr);
        void res6H(uint8_t* instr);
        void res6L(uint8_t* instr);
        void res6HL(uint8_t* instr);


        void res7A(uint8_t* instr);
        void res7B(uint8_t* instr);
        void res7C(uint8_t* instr);
        void res7D(uint8_t* instr);
        void res7E(uint8_t* instr);
        void res7H(uint8_t* instr);
        void res7L(uint8_t* instr);
        void res7HL(uint8_t* instr);

        void set0A(uint8_t* instr);
        void set0B(uint8_t* instr);
        void set0C(uint8_t* instr);
        void set0D(uint8_t* instr);
        void set0E(uint8_t* instr);
        void set0H(uint8_t* instr);
        void set0L(uint8_t* instr);
        void set0HL(uint8_t* instr);

        void set1A(uint8_t* instr);
        void set1B(uint8_t* instr);
        void set1C(uint8_t* instr);
        void set1D(uint8_t* instr);
        void set1E(uint8_t* instr);
        void set1H(uint8_t* instr);
        void set1L(uint8_t* instr);
        void set1HL(uint8_t* instr);

        void set2A(uint8_t* instr);
        void set2B(uint8_t* instr);
        void set2C(uint8_t* instr);
        void set2D(uint8_t* instr);
        void set2E(uint8_t* instr);
        void set2H(uint8_t* instr);
        void set2L(uint8_t* instr);
        void set2HL(uint8_t* instr);

        void set3A(uint8_t* instr);
        void set3B(uint8_t* instr);
        void set3C(uint8_t* instr);
        void set3D(uint8_t* instr);
        void set3E(uint8_t* instr);
        void set3H(uint8_t* instr);
        void set3L(uint8_t* instr);
        void set3HL(uint8_t* instr);

        void set4A(uint8_t* instr);
        void set4B(uint8_t* instr);
        void set4C(uint8_t* instr);
        void set4D(uint8_t* instr);
        void set4E(uint8_t* instr);
        void set4H(uint8_t* instr);
        void set4L(uint8_t* instr);
        void set4HL(uint8_t* instr);

        void set5A(uint8_t* instr);
        void set5B(uint8_t* instr);
        void set5C(uint8_t* instr);
        void set5D(uint8_t* instr);
        void set5E(uint8_t* instr);
        void set5H(uint8_t* instr);
        void set5L(uint8_t* instr);
        void set5HL(uint8_t* instr);

        void set6A(uint8_t* instr);
        void set6B(uint8_t* instr);
        void set6C(uint8_t* instr);
        void set6D(uint8_t* instr);
        void set6E(uint8_t* instr);
        void set6H(uint8_t* instr);
        void set6L(uint8_t* instr);
        void set6HL(uint8_t* instr);

        void set7A(uint8_t* instr);
        void set7B(uint8_t* instr);
        void set7C(uint8_t* instr);
        void set7D(uint8_t* instr);
        void set7E(uint8_t* instr);
        void set7H(uint8_t* instr);
        void set7L(uint8_t* instr);
        void set7HL(uint8_t* instr);

        uint8_t baseSLA(uint8_t operand);
        void slaA(uint8_t* instr);
        void slaB(uint8_t* instr);
        void slaC(uint8_t* instr);
        void slaD(uint8_t* instr);
        void slaE(uint8_t* instr);
        void slaH(uint8_t* instr);
        void slaL(uint8_t* instr);
        void slaHL(uint8_t* instr);

        uint8_t baseSRA(uint8_t operand);
        void sraA(uint8_t* instr);
        void sraB(uint8_t* instr);
        void sraC(uint8_t* instr);
        void sraD(uint8_t* instr);
        void sraE(uint8_t* instr);
        void sraH(uint8_t* instr);
        void sraL(uint8_t* instr);
        void sraHL(uint8_t* instr);

};


#endif