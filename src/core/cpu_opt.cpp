#include "cpu_opt.h"


static const uint16_t rstVectors[8] = {
    0x00, 0x08, 0x10, 0x18,
    0x20, 0x28, 0x30, 0x38
};

static const uint16_t interruptVector[5] = {0x40,0x48,0x50,0x58,0x60};

CPUOpt::CPUOpt(MemoryBus *bus) {
    registers = new Registers();
    alu = new Calculator();

    this->bus = bus;

    registers->setAF(0x01B0);
    registers->setBC(0x0013);
    registers->setDE(0x00D8);
    registers->setHL(0x014D);

    bus->writeByte(INTERRUPT_ENABLE_REGISTER, 0x00);
    bus->writeByte(TIMER_CONTROL, 0xF8);
    bus->writeByte(INTERRUPT_FLAG, 0xe1);
    bus->writeByte(LCD_CONTROL, 0x91);
    bus->writeByte(LCD_STAT, 0x81);
}

uint16_t IRAM_ATTR CPUOpt::executeStep() {
    clockCycle = 0;
    uint8_t pendingInterrupts = bus->verifyPendingInterrupts();

    if (isHalt == true) {
        if (pendingInterrupts != 0 && interruptEnable) {
            isHalt = false;
            handleInterrupts(pendingInterrupts);
        }

        if (pendingInterrupts !=0 && !interruptEnable) {
            isHalt = false;
        }

        clockCycle += 1;
    } else {
        if (pendingInterrupts != 0 && interruptEnable) {
            handleInterrupts(pendingInterrupts);
        }

        handleInstruction();
        handleInstruction();
    }
    return clockCycle;
}

inline IRAM_ATTR void CPUOpt::handleInstruction() {
    static const InstructionHandler handlers[256] = {
        &CPUOpt::noOp,                      // 0x00 - NOP
        &CPUOpt::load16Bc,                  // 0x01 - LD BC, d16
        &CPUOpt::loadPointer_BC_A,          // 0x02 - LD (BC), A
        &CPUOpt::inc16BC,                   // 0x03 - INC BC
        &CPUOpt::incB,                      // 0x04 - INC B
        &CPUOpt::decB,                      // 0x05 - DEC B
        &CPUOpt::load_B_D8,                 // 0x06 - LD B, d8
        &CPUOpt::rlca,                      // 0x07 - RLCA
        &CPUOpt::loadStack_D16_SP,          // 0x08 - LD (a16), SP
        &CPUOpt::addHlBc,                   // 0x09 - ADD HL, BC
        &CPUOpt::loadPointer_A_BC,          // 0x0A - LD A, (BC)
        &CPUOpt::dec16BC,                   // 0x0B - DEC BC
        &CPUOpt::incC,                      // 0x0C - INC C
        &CPUOpt::decC,                      // 0x0D - DEC C
        &CPUOpt::load_C_D8,                 // 0x0E - LD C, d8
        &CPUOpt::rrca,                      // 0x0F - RRCA
        &CPUOpt::noOp,                      // 0x10 - STOP (not implemented)
        &CPUOpt::load16De,                  // 0x11 - LD DE, d16
        &CPUOpt::loadPointer_DE_A,          // 0x12 - LD (DE), A
        &CPUOpt::inc16DE,                   // 0x13 - INC DE
        &CPUOpt::incD,                      // 0x14 - INC D
        &CPUOpt::decD,                      // 0x15 - DEC D
        &CPUOpt::load_D_D8,                 // 0x16 - LD D, d8
        &CPUOpt::rla,                       // 0x17 - RLA
        &CPUOpt::jumpRelD16,                // 0x18 - JR r8
        &CPUOpt::addHlDe,                   // 0x19 - ADD HL, DE
        &CPUOpt::loadPointer_A_DE,          // 0x1A - LD A, (DE)
        &CPUOpt::dec16DE,                   // 0x1B - DEC DE
        &CPUOpt::incE,                      // 0x1C - INC E
        &CPUOpt::decE,                      // 0x1D - DEC E
        &CPUOpt::load_E_D8,                 // 0x1E - LD E, d8
        &CPUOpt::rra,                       // 0x1F - RRA
        &CPUOpt::jumpRelNZ,                 // 0x20 - JR NZ, r8
        &CPUOpt::load16Hl,                  // 0x21 - LD HL, d16
        &CPUOpt::loadPointer_HLI_A,         // 0x22 - LD (HL+), A
        &CPUOpt::inc16HL,                   // 0x23 - INC HL
        &CPUOpt::incH,                      // 0x24 - INC H
        &CPUOpt::decH,                      // 0x25 - DEC H
        &CPUOpt::load_H_D8,                 // 0x26 - LD H, d8
        &CPUOpt::daa,                       // 0x27 - DAA
        &CPUOpt::jumpRelZ,                  // 0x28 - JR Z, r8
        &CPUOpt::addHlHl,                   // 0x29 - ADD HL, HL
        &CPUOpt::loadPointer_A_HLI,         // 0x2A - LD A, (HL+)
        &CPUOpt::dec16HL,                   // 0x2B - DEC HL
        &CPUOpt::incL,                      // 0x2C - INC L
        &CPUOpt::decL,                      // 0x2D - DEC L
        &CPUOpt::load_L_D8,                 // 0x2E - LD L, d8
        &CPUOpt::cpl,                       // 0x2F - CPL
        &CPUOpt::jumpRelNC,                 // 0x30 - JR NC, r8
        &CPUOpt::load16Sp,                  // 0x31 - LD SP, d16
        &CPUOpt::loadPointer_HLD_A,         // 0x32 - LD (HL-), A
        &CPUOpt::inc16SP,                   // 0x33 - INC SP
        &CPUOpt::incHL,                     // 0x34 - INC (HL)
        &CPUOpt::decHL,                     // 0x35 - DEC (HL)
        &CPUOpt::load_HL_D8,                // 0x36 - LD (HL), d8
        &CPUOpt::sfc,                       // 0x37 - SCF
        &CPUOpt::jumpRelCR,                 // 0x38 - JR C, r8
        &CPUOpt::addHlSp,                   // 0x39 - ADD HL, SP
        &CPUOpt::loadPointer_A_HLD,         // 0x3A - LD A, (HL-)
        &CPUOpt::dec16SP,                   // 0x3B - DEC SP
        &CPUOpt::incA,                      // 0x3C - INC A
        &CPUOpt::decA,                      // 0x3D - DEC A
        &CPUOpt::load_A_D8,                 // 0x3E - LD A, d8
        &CPUOpt::ccf,                       // 0x3F - CCF
        &CPUOpt::load_B_B,                  // 0x40 - LD B, B
        &CPUOpt::load_B_C,                  // 0x41 - LD B, C
        &CPUOpt::load_B_D,                  // 0x42 - LD B, D
        &CPUOpt::load_B_E,                  // 0x43 - LD B, E
        &CPUOpt::load_B_H,                  // 0x44 - LD B, H
        &CPUOpt::load_B_L,                  // 0x45 - LD B, L
        &CPUOpt::load_B_HL,                 // 0x46 - LD B, (HL)
        &CPUOpt::load_B_A,                  // 0x47 - LD B, A
        &CPUOpt::load_C_B,                  // 0x48 - LD C, B
        &CPUOpt::load_C_C,                  // 0x49 - LD C, C
        &CPUOpt::load_C_D,                  // 0x4A - LD C, D
        &CPUOpt::load_C_E,                  // 0x4B - LD C, E
        &CPUOpt::load_C_H,                  // 0x4C - LD C, H
        &CPUOpt::load_C_L,                  // 0x4D - LD C, L
        &CPUOpt::load_C_HL,                 // 0x4E - LD C, (HL)
        &CPUOpt::load_C_A,                  // 0x4F - LD C, A
        &CPUOpt::load_D_B,                  // 0x50 - LD D, B
        &CPUOpt::load_D_C,                  // 0x51 - LD D, C
        &CPUOpt::load_D_D,                  // 0x52 - LD D, D
        &CPUOpt::load_D_E,                  // 0x53 - LD D, E
        &CPUOpt::load_D_H,                  // 0x54 - LD D, H
        &CPUOpt::load_D_L,                  // 0x55 - LD D, L
        &CPUOpt::load_D_HL,                 // 0x56 - LD D, (HL)
        &CPUOpt::load_D_A,                  // 0x57 - LD D, A
        &CPUOpt::load_E_B,                  // 0x58 - LD E, B
        &CPUOpt::load_E_C,                  // 0x59 - LD E, C
        &CPUOpt::load_E_D,                  // 0x5A - LD E, D
        &CPUOpt::load_E_E,                  // 0x5B - LD E, E
        &CPUOpt::load_E_H,                  // 0x5C - LD E, H
        &CPUOpt::load_E_L,                  // 0x5D - LD E, L
        &CPUOpt::load_E_HL,                 // 0x5E - LD E, (HL)
        &CPUOpt::load_E_A,                  // 0x5F - LD E, A
        &CPUOpt::load_H_B,                  // 0x60 - LD H, B
        &CPUOpt::load_H_C,                  // 0x61 - LD H, C
        &CPUOpt::load_H_D,                  // 0x62 - LD H, D
        &CPUOpt::load_H_E,                  // 0x63 - LD H, E
        &CPUOpt::load_H_H,                  // 0x64 - LD H, H
        &CPUOpt::load_H_L,                  // 0x65 - LD H, L
        &CPUOpt::load_H_HL,                 // 0x66 - LD H, (HL)
        &CPUOpt::load_H_A,                  // 0x67 - LD H, A
        &CPUOpt::load_L_B,                  // 0x68 - LD L, B
        &CPUOpt::load_L_C,                  // 0x69 - LD L, C
        &CPUOpt::load_L_D,                  // 0x6A - LD L, D
        &CPUOpt::load_L_E,                  // 0x6B - LD L, E
        &CPUOpt::load_L_H,                  // 0x6C - LD L, H
        &CPUOpt::load_L_L,                  // 0x6D - LD L, L
        &CPUOpt::load_L_HL,                 // 0x6E - LD L, (HL)
        &CPUOpt::load_L_A,                  // 0x6F - LD L, A
        &CPUOpt::load_HL_B,                 // 0x70 - LD (HL), B
        &CPUOpt::load_HL_C,                 // 0x71 - LD (HL), C
        &CPUOpt::load_HL_D,                 // 0x72 - LD (HL), D
        &CPUOpt::load_HL_E,                 // 0x73 - LD (HL), E
        &CPUOpt::load_HL_H,                 // 0x74 - LD (HL), H
        &CPUOpt::load_HL_L,                 // 0x75 - LD (HL), L
        &CPUOpt::halt,                      // 0x76 - HALT
        &CPUOpt::load_HL_A,                 // 0x77 - LD (HL), A
        &CPUOpt::load_A_B,                  // 0x78 - LD A, B
        &CPUOpt::load_A_C,                  // 0x79 - LD A, C
        &CPUOpt::load_A_D,                  // 0x7A - LD A, D
        &CPUOpt::load_A_E,                  // 0x7B - LD A, E
        &CPUOpt::load_A_H,                  // 0x7C - LD A, H
        &CPUOpt::load_A_L,                  // 0x7D - LD A, L
        &CPUOpt::load_A_HL,                 // 0x7E - LD A, (HL)
        &CPUOpt::load_A_A,                  // 0x7F - LD A, A
        &CPUOpt::addB,                      // 0x80 - ADD A, B
        &CPUOpt::addC,                      // 0x81 - ADD A, C
        &CPUOpt::addD,                      // 0x82 - ADD A, D
        &CPUOpt::addE,                      // 0x83 - ADD A, E
        &CPUOpt::addH,                      // 0x84 - ADD A, H
        &CPUOpt::addL,                      // 0x85 - ADD A, L
        &CPUOpt::addHL,                     // 0x86 - ADD A, (HL)
        &CPUOpt::addA,                      // 0x87 - ADD A, A
        &CPUOpt::addCarryB,                 // 0x88 - ADC A, B
        &CPUOpt::addCarryC,                 // 0x89 - ADC A, C
        &CPUOpt::addCarryD,                 // 0x8A - ADC A, D
        &CPUOpt::addCarryE,                 // 0x8B - ADC A, E
        &CPUOpt::addCarryH,                 // 0x8C - ADC A, H
        &CPUOpt::addCarryL,                 // 0x8D - ADC A, L
        &CPUOpt::addCarryHL,                // 0x8E - ADC A, (HL)
        &CPUOpt::addCarryA,                 // 0x8F - ADC A, A
        &CPUOpt::subB,                      // 0x90 - SUB B
        &CPUOpt::subC,                      // 0x91 - SUB C
        &CPUOpt::subD,                      // 0x92 - SUB D
        &CPUOpt::subE,                      // 0x93 - SUB E
        &CPUOpt::subH,                      // 0x94 - SUB H
        &CPUOpt::subL,                      // 0x95 - SUB L
        &CPUOpt::subHL,                     // 0x96 - SUB (HL)
        &CPUOpt::subA,                      // 0x97 - SUB A
        &CPUOpt::subCarryB,                 // 0x98 - SBC A, B
        &CPUOpt::subCarryC,                 // 0x99 - SBC A, C
        &CPUOpt::subCarryD,                 // 0x9A - SBC A, D
        &CPUOpt::subCarryE,                 // 0x9B - SBC A, E
        &CPUOpt::subCarryH,                 // 0x9C - SBC A, H
        &CPUOpt::subCarryL,                 // 0x9D - SBC A, L
        &CPUOpt::subCarryHL,                // 0x9E - SBC A, (HL)
        &CPUOpt::subCarryA,                 // 0x9F - SBC A, A
        &CPUOpt::andB,                      // 0xA0 - AND B
        &CPUOpt::andC,                      // 0xA1 - AND C
        &CPUOpt::andD,                      // 0xA2 - AND D
        &CPUOpt::andE,                      // 0xA3 - AND E
        &CPUOpt::andH,                      // 0xA4 - AND H
        &CPUOpt::andL,                      // 0xA5 - AND L
        &CPUOpt::andHL,                     // 0xA6 - AND (HL)
        &CPUOpt::andA,                      // 0xA7 - AND A
        &CPUOpt::xorB,                      // 0xA8 - XOR B
        &CPUOpt::xorC,                      // 0xA9 - XOR C
        &CPUOpt::xorD,                      // 0xAA - XOR D
        &CPUOpt::xorE,                      // 0xAB - XOR E
        &CPUOpt::xorH,                      // 0xAC - XOR H
        &CPUOpt::xorL,                      // 0xAD - XOR L
        &CPUOpt::xorHL,                     // 0xAE - XOR (HL)
        &CPUOpt::xorA,                      // 0xAF - XOR A
        &CPUOpt::orB,                       // 0xB0 - OR B
        &CPUOpt::orC,                       // 0xB1 - OR C
        &CPUOpt::orD,                       // 0xB2 - OR D
        &CPUOpt::orE,                       // 0xB3 - OR E
        &CPUOpt::orH,                       // 0xB4 - OR H
        &CPUOpt::orL,                       // 0xB5 - OR L
        &CPUOpt::orHL,                      // 0xB6 - OR (HL)
        &CPUOpt::orA,                       // 0xB7 - OR A
        &CPUOpt::compareB,                  // 0xB8 - CP B
        &CPUOpt::compareC,                  // 0xB9 - CP C
        &CPUOpt::compareD,                  // 0xBA - CP D
        &CPUOpt::compareE,                  // 0xBB - CP E
        &CPUOpt::compareH,                  // 0xBC - CP H
        &CPUOpt::compareL,                  // 0xBD - CP L
        &CPUOpt::compareHL,                 // 0xBE - CP (HL)
        &CPUOpt::compareA,                  // 0xBF - CP A
        &CPUOpt::retNZ,                     // 0xC0 - RET NZ
        &CPUOpt::popBC,                     // 0xC1 - POP BC
        &CPUOpt::jumpNZ,                    // 0xC2 - JP NZ, a16
        &CPUOpt::jumpD16,                   // 0xC3 - JP a16
        &CPUOpt::callNZ,                    // 0xC4 - CALL NZ, a16
        &CPUOpt::pushBC,                    // 0xC5 - PUSH BC
        &CPUOpt::addD8,                     // 0xC6 - ADD A, d8
        &CPUOpt::rstV0,                     // 0xC7 - RST 0
        &CPUOpt::retZ,                      // 0xC8 - RET Z
        &CPUOpt::retD16,                    // 0xC9 - RET
        &CPUOpt::jumpZ,                     // 0xCA - JP Z, a16
        &CPUOpt::noOp,                      // 0xCB - PREFIX (handled separately)
        &CPUOpt::callZ,                     // 0xCC - CALL Z, a16
        &CPUOpt::callD16,                   // 0xCD - CALL a16
        &CPUOpt::addCarryD8,                // 0xCE - ADC A, d8
        &CPUOpt::rstV1,                     // 0xCF - RST 1
        &CPUOpt::retNC,                     // 0xD0 - RET NC
        &CPUOpt::popDE,                     // 0xD1 - POP DE
        &CPUOpt::jumpNC,                    // 0xD2 - JP NC, a16
        &CPUOpt::noOp,                      // 0xD3 - Undefined
        &CPUOpt::callNC,                    // 0xD4 - CALL NC, a16
        &CPUOpt::pushDE,                    // 0xD5 - PUSH DE
        &CPUOpt::subD8,                     // 0xD6 - SUB d8
        &CPUOpt::rstV2,                     // 0xD7 - RST 2
        &CPUOpt::retCR,                     // 0xD8 - RET C
        &CPUOpt::reti,                      // 0xD9 - RETI
        &CPUOpt::jumpCR,                    // 0xDA - JP C, a16
        &CPUOpt::noOp,                      // 0xDB - Undefined
        &CPUOpt::callCR,                    // 0xDC - CALL C, a16
        &CPUOpt::noOp,                      // 0xDD - Undefined
        &CPUOpt::subCarryD8,                // 0xDE - SBC A, d8
        &CPUOpt::rstV3,                     // 0xDF - RST 3
        &CPUOpt::loadHigh_A_D8,             // 0xE0 - LDH (a8), A
        &CPUOpt::popHL,                     // 0xE1 - POP HL
        &CPUOpt::loadHigh_A_C,              // 0xE2 - LDH (C), A
        &CPUOpt::noOp,                      // 0xE3 - Undefined
        &CPUOpt::noOp,                      // 0xE4 - Undefined
        &CPUOpt::pushHL,                    // 0xE5 - PUSH HL
        &CPUOpt::andD8,                     // 0xE6 - AND d8
        &CPUOpt::rstV4,                     // 0xE7 - RST 4
        &CPUOpt::addSp,                     // 0xE8 - ADD SP, r8
        &CPUOpt::jumpHl,                    // 0xE9 - JP HL
        &CPUOpt::loadPointer_D16_A,         // 0xEA - LD (a16), A
        &CPUOpt::noOp,                      // 0xEB - Undefined
        &CPUOpt::noOp,                      // 0xEC - Undefined
        &CPUOpt::noOp,                      // 0xED - Undefined
        &CPUOpt::xorD8,                     // 0xEE - XOR d8
        &CPUOpt::rstV5,                     // 0xEF - RST 5
        &CPUOpt::loadHigh_D8_A,             // 0xF0 - LDH A, (a8)
        &CPUOpt::popAF,                     // 0xF1 - POP AF
        &CPUOpt::loadHigh_C_A,              // 0xF2 - LDH A, (C)
        &CPUOpt::di,                        // 0xF3 - DI
        &CPUOpt::noOp,                      // 0xF4 - Undefined
        &CPUOpt::pushAF,                    // 0xF5 - PUSH AF
        &CPUOpt::orD8,                      // 0xF6 - OR d8
        &CPUOpt::rstV6,                     // 0xF7 - RST 6
        &CPUOpt::loadStack_HL_SPe,          // 0xF8 - LD HL, SP+r8
        &CPUOpt::loadStack_SP_HL,           // 0xF9 - LD SP, HL
        &CPUOpt::loadPointer_A_D16,         // 0xFA - LD A, (a16)
        &CPUOpt::ei,                        // 0xFB - EI
        &CPUOpt::noOp,                      // 0xFC - Undefined
        &CPUOpt::noOp,                      // 0xFD - Undefined
        &CPUOpt::compareD8,                 // 0xFE - CP d8
        &CPUOpt::rstV7                      // 0xFF - RST 7
    };

    static const InstructionHandler prefixHandlers[256] = {
        &CPUOpt::rlcB,    // 0x00 - RLC B
        &CPUOpt::rlcC,    // 0x01 - RLC C
        &CPUOpt::rlcD,    // 0x02 - RLC D
        &CPUOpt::rlcE,    // 0x03 - RLC E
        &CPUOpt::rlcH,    // 0x04 - RLC H
        &CPUOpt::rlcL,    // 0x05 - RLC L
        &CPUOpt::rlcHL,   // 0x06 - RLC (HL)
        &CPUOpt::rlcA,    // 0x07 - RLC A
        &CPUOpt::rrcB,    // 0x08 - RRC B
        &CPUOpt::rrcC,    // 0x09 - RRC C
        &CPUOpt::rrcD,    // 0x0A - RRC D
        &CPUOpt::rrcE,    // 0x0B - RRC E
        &CPUOpt::rrcH,    // 0x0C - RRC H
        &CPUOpt::rrcL,    // 0x0D - RRC L
        &CPUOpt::rrcHL,   // 0x0E - RRC (HL)
        &CPUOpt::rrcA,    // 0x0F - RRC A
        &CPUOpt::rlB,     // 0x10 - RL B
        &CPUOpt::rlC,     // 0x11 - RL C
        &CPUOpt::rlD,     // 0x12 - RL D
        &CPUOpt::rlE,     // 0x13 - RL E
        &CPUOpt::rlH,     // 0x14 - RL H
        &CPUOpt::rlL,     // 0x15 - RL L
        &CPUOpt::rlHL,    // 0x16 - RL (HL)
        &CPUOpt::rlA,     // 0x17 - RL A
        &CPUOpt::rrB,     // 0x18 - RR B
        &CPUOpt::rrC,     // 0x19 - RR C
        &CPUOpt::rrD,     // 0x1A - RR D
        &CPUOpt::rrE,     // 0x1B - RR E
        &CPUOpt::rrH,     // 0x1C - RR H
        &CPUOpt::rrL,     // 0x1D - RR L
        &CPUOpt::rrHL,    // 0x1E - RR (HL)
        &CPUOpt::rrA,     // 0x1F - RR A
        &CPUOpt::slaB,    // 0x20 - SLA B
        &CPUOpt::slaC,    // 0x21 - SLA C
        &CPUOpt::slaD,    // 0x22 - SLA D
        &CPUOpt::slaE,    // 0x23 - SLA E
        &CPUOpt::slaH,    // 0x24 - SLA H
        &CPUOpt::slaL,    // 0x25 - SLA L
        &CPUOpt::slaHL,   // 0x26 - SLA (HL)
        &CPUOpt::slaA,    // 0x27 - SLA A
        &CPUOpt::sraB,    // 0x28 - SRA B
        &CPUOpt::sraC,    // 0x29 - SRA C
        &CPUOpt::sraD,    // 0x2A - SRA D
        &CPUOpt::sraE,    // 0x2B - SRA E
        &CPUOpt::sraH,    // 0x2C - SRA H
        &CPUOpt::sraL,    // 0x2D - SRA L
        &CPUOpt::sraHL,   // 0x2E - SRA (HL)
        &CPUOpt::sraA,    // 0x2F - SRA A
        &CPUOpt::swapB,   // 0x30 - SWAP B
        &CPUOpt::swapC,   // 0x31 - SWAP C
        &CPUOpt::swapD,   // 0x32 - SWAP D
        &CPUOpt::swapE,   // 0x33 - SWAP E
        &CPUOpt::swapH,   // 0x34 - SWAP H
        &CPUOpt::swapL,   // 0x35 - SWAP L
        &CPUOpt::swapHL,  // 0x36 - SWAP (HL)
        &CPUOpt::swapA,   // 0x37 - SWAP A
        &CPUOpt::srlB,    // 0x38 - SRL B
        &CPUOpt::srlC,    // 0x39 - SRL C
        &CPUOpt::srlD,    // 0x3A - SRL D
        &CPUOpt::srlE,    // 0x3B - SRL E
        &CPUOpt::srlH,    // 0x3C - SRL H
        &CPUOpt::srlL,    // 0x3D - SRL L
        &CPUOpt::srlHL,   // 0x3E - SRL (HL)
        &CPUOpt::srlA,    // 0x3F - SRL A
        &CPUOpt::bit0B,   // 0x40 - BIT 0, B
        &CPUOpt::bit0C,   // 0x41 - BIT 0, C
        &CPUOpt::bit0D,   // 0x42 - BIT 0, D
        &CPUOpt::bit0E,   // 0x43 - BIT 0, E
        &CPUOpt::bit0H,   // 0x44 - BIT 0, H
        &CPUOpt::bit0L,   // 0x45 - BIT 0, L
        &CPUOpt::bit0HL,  // 0x46 - BIT 0, (HL)
        &CPUOpt::bit0A,   // 0x47 - BIT 0, A
        &CPUOpt::bit1B,   // 0x48 - BIT 1, B
        &CPUOpt::bit1C,   // 0x49 - BIT 1, C
        &CPUOpt::bit1D,   // 0x4A - BIT 1, D
        &CPUOpt::bit1E,   // 0x4B - BIT 1, E
        &CPUOpt::bit1H,   // 0x4C - BIT 1, H
        &CPUOpt::bit1L,   // 0x4D - BIT 1, L
        &CPUOpt::bit1HL,  // 0x4E - BIT 1, (HL)
        &CPUOpt::bit1A,   // 0x4F - BIT 1, A
        &CPUOpt::bit2B,   // 0x50 - BIT 2, B
        &CPUOpt::bit2C,   // 0x51 - BIT 2, C
        &CPUOpt::bit2D,   // 0x52 - BIT 2, D
        &CPUOpt::bit2E,   // 0x53 - BIT 2, E
        &CPUOpt::bit2H,   // 0x54 - BIT 2, H
        &CPUOpt::bit2L,   // 0x55 - BIT 2, L
        &CPUOpt::bit2HL,  // 0x56 - BIT 2, (HL)
        &CPUOpt::bit2A,   // 0x57 - BIT 2, A
        &CPUOpt::bit3B,   // 0x58 - BIT 3, B
        &CPUOpt::bit3C,   // 0x59 - BIT 3, C
        &CPUOpt::bit3D,   // 0x5A - BIT 3, D
        &CPUOpt::bit3E,   // 0x5B - BIT 3, E
        &CPUOpt::bit3H,   // 0x5C - BIT 3, H
        &CPUOpt::bit3L,   // 0x5D - BIT 3, L
        &CPUOpt::bit3HL,  // 0x5E - BIT 3, (HL)
        &CPUOpt::bit3A,   // 0x5F - BIT 3, A
        &CPUOpt::bit4B,   // 0x60 - BIT 4, B
        &CPUOpt::bit4C,   // 0x61 - BIT 4, C
        &CPUOpt::bit4D,   // 0x62 - BIT 4, D
        &CPUOpt::bit4E,   // 0x63 - BIT 4, E
        &CPUOpt::bit4H,   // 0x64 - BIT 4, H
        &CPUOpt::bit4L,   // 0x65 - BIT 4, L
        &CPUOpt::bit4HL,  // 0x66 - BIT 4, (HL)
        &CPUOpt::bit4A,   // 0x67 - BIT 4, A
        &CPUOpt::bit5B,   // 0x68 - BIT 5, B
        &CPUOpt::bit5C,   // 0x69 - BIT 5, C
        &CPUOpt::bit5D,   // 0x6A - BIT 5, D
        &CPUOpt::bit5E,   // 0x6B - BIT 5, E
        &CPUOpt::bit5H,   // 0x6C - BIT 5, H
        &CPUOpt::bit5L,   // 0x6D - BIT 5, L
        &CPUOpt::bit5HL,  // 0x6E - BIT 5, (HL)
        &CPUOpt::bit5A,   // 0x6F - BIT 5, A
        &CPUOpt::bit6B,   // 0x70 - BIT 6, B
        &CPUOpt::bit6C,   // 0x71 - BIT 6, C
        &CPUOpt::bit6D,   // 0x72 - BIT 6, D
        &CPUOpt::bit6E,   // 0x73 - BIT 6, E
        &CPUOpt::bit6H,   // 0x74 - BIT 6, H
        &CPUOpt::bit6L,   // 0x75 - BIT 6, L
        &CPUOpt::bit6HL,  // 0x76 - BIT 6, (HL)
        &CPUOpt::bit6A,   // 0x77 - BIT 6, A
        &CPUOpt::bit7B,   // 0x78 - BIT 7, B
        &CPUOpt::bit7C,   // 0x79 - BIT 7, C
        &CPUOpt::bit7D,   // 0x7A - BIT 7, D
        &CPUOpt::bit7E,   // 0x7B - BIT 7, E
        &CPUOpt::bit7H,   // 0x7C - BIT 7, H
        &CPUOpt::bit7L,   // 0x7D - BIT 7, L
        &CPUOpt::bit7HL,  // 0x7E - BIT 7, (HL)
        &CPUOpt::bit7A,   // 0x7F - BIT 7, A
        &CPUOpt::res0B,   // 0x80 - RES 0, B
        &CPUOpt::res0C,   // 0x81 - RES 0, C
        &CPUOpt::res0D,   // 0x82 - RES 0, D
        &CPUOpt::res0E,   // 0x83 - RES 0, E
        &CPUOpt::res0H,   // 0x84 - RES 0, H
        &CPUOpt::res0L,   // 0x85 - RES 0, L
        &CPUOpt::res0HL,  // 0x86 - RES 0, (HL)
        &CPUOpt::res0A,   // 0x87 - RES 0, A
        &CPUOpt::res1B,   // 0x88 - RES 1, B
        &CPUOpt::res1C,   // 0x89 - RES 1, C
        &CPUOpt::res1D,   // 0x8A - RES 1, D
        &CPUOpt::res1E,   // 0x8B - RES 1, E
        &CPUOpt::res1H,   // 0x8C - RES 1, H
        &CPUOpt::res1L,   // 0x8D - RES 1, L
        &CPUOpt::res1HL,  // 0x8E - RES 1, (HL)
        &CPUOpt::res1A,   // 0x8F - RES 1, A
        &CPUOpt::res2B,   // 0x90 - RES 2, B
        &CPUOpt::res2C,   // 0x91 - RES 2, C
        &CPUOpt::res2D,   // 0x92 - RES 2, D
        &CPUOpt::res2E,   // 0x93 - RES 2, E
        &CPUOpt::res2H,   // 0x94 - RES 2, H
        &CPUOpt::res2L,   // 0x95 - RES 2, L
        &CPUOpt::res2HL,  // 0x96 - RES 2, (HL)
        &CPUOpt::res2A,   // 0x97 - RES 2, A
        &CPUOpt::res3B,   // 0x98 - RES 3, B
        &CPUOpt::res3C,   // 0x99 - RES 3, C
        &CPUOpt::res3D,   // 0x9A - RES 3, D
        &CPUOpt::res3E,   // 0x9B - RES 3, E
        &CPUOpt::res3H,   // 0x9C - RES 3, H
        &CPUOpt::res3L,   // 0x9D - RES 3, L
        &CPUOpt::res3HL,  // 0x9E - RES 3, (HL)
        &CPUOpt::res3A,   // 0x9F - RES 3, A
        &CPUOpt::res4B,   // 0xA0 - RES 4, B
        &CPUOpt::res4C,   // 0xA1 - RES 4, C
        &CPUOpt::res4D,   // 0xA2 - RES 4, D
        &CPUOpt::res4E,   // 0xA3 - RES 4, E
        &CPUOpt::res4H,   // 0xA4 - RES 4, H
        &CPUOpt::res4L,   // 0xA5 - RES 4, L
        &CPUOpt::res4HL,  // 0xA6 - RES 4, (HL)
        &CPUOpt::res4A,   // 0xA7 - RES 4, A
        &CPUOpt::res5B,   // 0xA8 - RES 5, B
        &CPUOpt::res5C,   // 0xA9 - RES 5, C
        &CPUOpt::res5D,   // 0xAA - RES 5, D
        &CPUOpt::res5E,   // 0xAB - RES 5, E
        &CPUOpt::res5H,   // 0xAC - RES 5, H
        &CPUOpt::res5L,   // 0xAD - RES 5, L
        &CPUOpt::res5HL,  // 0xAE - RES 5, (HL)
        &CPUOpt::res5A,   // 0xAF - RES 5, A
        &CPUOpt::res6B,   // 0xB0 - RES 6, B
        &CPUOpt::res6C,   // 0xB1 - RES 6, C
        &CPUOpt::res6D,   // 0xB2 - RES 6, D
        &CPUOpt::res6E,   // 0xB3 - RES 6, E
        &CPUOpt::res6H,   // 0xB4 - RES 6, H
        &CPUOpt::res6L,   // 0xB5 - RES 6, L
        &CPUOpt::res6HL,  // 0xB6 - RES 6, (HL)
        &CPUOpt::res6A,   // 0xB7 - RES 6, A
        &CPUOpt::res7B,   // 0xB8 - RES 7, B
        &CPUOpt::res7C,   // 0xB9 - RES 7, C
        &CPUOpt::res7D,   // 0xBA - RES 7, D
        &CPUOpt::res7E,   // 0xBB - RES 7, E
        &CPUOpt::res7H,   // 0xBC - RES 7, H
        &CPUOpt::res7L,   // 0xBD - RES 7, L
        &CPUOpt::res7HL,  // 0xBE - RES 7, (HL)
        &CPUOpt::res7A,   // 0xBF - RES 7, A
        &CPUOpt::set0B,   // 0xC0 - SET 0, B
        &CPUOpt::set0C,   // 0xC1 - SET 0, C
        &CPUOpt::set0D,   // 0xC2 - SET 0, D
        &CPUOpt::set0E,   // 0xC3 - SET 0, E
        &CPUOpt::set0H,   // 0xC4 - SET 0, H
        &CPUOpt::set0L,   // 0xC5 - SET 0, L
        &CPUOpt::set0HL,  // 0xC6 - SET 0, (HL)
        &CPUOpt::set0A,   // 0xC7 - SET 0, A
        &CPUOpt::set1B,   // 0xC8 - SET 1, B
        &CPUOpt::set1C,   // 0xC9 - SET 1, C
        &CPUOpt::set1D,   // 0xCA - SET 1, D
        &CPUOpt::set1E,   // 0xCB - SET 1, E
        &CPUOpt::set1H,   // 0xCC - SET 1, H
        &CPUOpt::set1L,   // 0xCD - SET 1, L
        &CPUOpt::set1HL,  // 0xCE - SET 1, (HL)
        &CPUOpt::set1A,   // 0xCF - SET 1, A
        &CPUOpt::set2B,   // 0xD0 - SET 2, B
        &CPUOpt::set2C,   // 0xD1 - SET 2, C
        &CPUOpt::set2D,   // 0xD2 - SET 2, D
        &CPUOpt::set2E,   // 0xD3 - SET 2, E
        &CPUOpt::set2H,   // 0xD4 - SET 2, H
        &CPUOpt::set2L,   // 0xD5 - SET 2, L
        &CPUOpt::set2HL,  // 0xD6 - SET 2, (HL)
        &CPUOpt::set2A,   // 0xD7 - SET 2, A
        &CPUOpt::set3B,   // 0xD8 - SET 3, B
        &CPUOpt::set3C,   // 0xD9 - SET 3, C
        &CPUOpt::set3D,   // 0xDA - SET 3, D
        &CPUOpt::set3E,   // 0xDB - SET 3, E
        &CPUOpt::set3H,   // 0xDC - SET 3, H
        &CPUOpt::set3L,   // 0xDD - SET 3, L
        &CPUOpt::set3HL,  // 0xDE - SET 3, (HL)
        &CPUOpt::set3A,   // 0xDF - SET 3, A
        &CPUOpt::set4B,   // 0xE0 - SET 4, B
        &CPUOpt::set4C,   // 0xE1 - SET 4, C
        &CPUOpt::set4D,   // 0xE2 - SET 4, D
        &CPUOpt::set4E,   // 0xE3 - SET 4, E
        &CPUOpt::set4H,   // 0xE4 - SET 4, H
        &CPUOpt::set4L,   // 0xE5 - SET 4, L
        &CPUOpt::set4HL,  // 0xE6 - SET 4, (HL)
        &CPUOpt::set4A,   // 0xE7 - SET 4, A
        &CPUOpt::set5B,   // 0xE8 - SET 5, B
        &CPUOpt::set5C,   // 0xE9 - SET 5, C
        &CPUOpt::set5D,   // 0xEA - SET 5, D
        &CPUOpt::set5E,   // 0xEB - SET 5, E
        &CPUOpt::set5H,   // 0xEC - SET 5, H
        &CPUOpt::set5L,   // 0xED - SET 5, L
        &CPUOpt::set5HL,  // 0xEE - SET 5, (HL)
        &CPUOpt::set5A,   // 0xEF - SET 5, A
        &CPUOpt::set6B,   // 0xF0 - SET 6, B
        &CPUOpt::set6C,   // 0xF1 - SET 6, C
        &CPUOpt::set6D,   // 0xF2 - SET 6, D
        &CPUOpt::set6E,   // 0xF3 - SET 6, E
        &CPUOpt::set6H,   // 0xF4 - SET 6, H
        &CPUOpt::set6L,   // 0xF5 - SET 6, L
        &CPUOpt::set6HL,  // 0xF6 - SET 6, (HL)
        &CPUOpt::set6A,   // 0xF7 - SET 6, A
        &CPUOpt::set7B,   // 0xF8 - SET 7, B
        &CPUOpt::set7C,   // 0xF9 - SET 7, C
        &CPUOpt::set7D,   // 0xFA - SET 7, D
        &CPUOpt::set7E,   // 0xFB - SET 7, E
        &CPUOpt::set7H,   // 0xFC - SET 7, H
        &CPUOpt::set7L,   // 0xFD - SET 7, L
        &CPUOpt::set7HL,  // 0xFE - SET 7, (HL)
        &CPUOpt::set7A    // 0xFF - SET 7, A
    };

    uint8_t* rawInstruction = bus->fetchBlock(programCounter);

    if (rawInstruction[0] == 0xCB) {
        programCounter = programCounter + 2;
        (this->*prefixHandlers[rawInstruction[1]])(&rawInstruction[2]);
    } else {
        programCounter = programCounter + 1;
        (this->*handlers[rawInstruction[0]])(&rawInstruction[1]);
    }
}

void CPUOpt::noOp(uint8_t* instr) {

}

/* ADD BLOCK */

void CPUOpt::baseAdd(uint8_t operand, bool addc) {
    uint8_t carry = (registers->carry && addc) ? 1 : 0;

    uint8_t acumulator = registers->getA();
    uint8_t firstResult = alu->addU8(acumulator, operand);
    bool firstCarry = alu->overflow;
    bool firstHalfCarry = alu->verifyOverflow(acumulator, operand, 3);

    uint8_t secondResult = alu->addU8(firstResult, carry);
    bool secondCarry = alu->overflow;
    bool secondHalfCarry = alu->verifyOverflow(firstResult, carry, 3);

    registers->zero = (secondResult == 0);
    registers->negative = false;
    registers->halfCarry = firstHalfCarry | secondHalfCarry;
    registers->carry = firstCarry | secondCarry;

    registers->setA(secondResult);
}

void CPUOpt::addB(uint8_t* instr) {
    baseAdd(registers->getB(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addC(uint8_t* instr) {
    baseAdd(registers->getC(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addD(uint8_t* instr) {
    baseAdd(registers->getD(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addE(uint8_t* instr) {
    baseAdd(registers->getE(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addH(uint8_t* instr) {
    baseAdd(registers->getH(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addL(uint8_t* instr) {
    baseAdd(registers->getL(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addHL(uint8_t* instr) {
    baseAdd(bus->readByte(registers->getHL()), false);
    clockCycle = clockCycle + 2;
}

void CPUOpt::addA(uint8_t* instr) {
    baseAdd(registers->getA(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addD8(uint8_t* instr) {
    baseAdd(instr[0], false);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::addCarryB(uint8_t* instr) {
    baseAdd(registers->getB(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryC(uint8_t* instr) {
    baseAdd(registers->getC(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryD(uint8_t* instr) {
    baseAdd(registers->getD(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryE(uint8_t* instr) {
    baseAdd(registers->getE(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryH(uint8_t* instr) {
    baseAdd(registers->getH(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryL(uint8_t* instr) {
    baseAdd(registers->getL(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryHL(uint8_t* instr) {
    baseAdd(bus->readByte(registers->getHL()), true);
    clockCycle = clockCycle + 2;
}

void CPUOpt::addCarryA(uint8_t* instr) {
    baseAdd(registers->getA(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::addCarryD8(uint8_t* instr) {
    baseAdd(instr[0], true);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}


/* AND BLOCK */
void CPUOpt::baseAnd(uint8_t operand) {
    uint8_t accumulator = registers->getA();
    uint8_t result = alu->andU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = true;
    registers->carry = false;

    registers->setA(result);
}

void CPUOpt::andB(uint8_t* instr) {
    baseAnd(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andC(uint8_t* instr) {
    baseAnd(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andD(uint8_t* instr) {
    baseAnd(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andE(uint8_t* instr) {
    baseAnd(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andH(uint8_t* instr) {
    baseAnd(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andL(uint8_t* instr) {
    baseAnd(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andHL(uint8_t* instr) {
    baseAnd(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::andA(uint8_t* instr) {
    baseAnd(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::andD8(uint8_t* instr) {
    baseAnd(instr[0]);
    clockCycle = clockCycle + 1;
    programCounter = programCounter + 1;
}


/* COMPARE BLOCK */

void CPUOpt::baseCompare(uint8_t operand) {
    uint8_t a = registers->getA();

    registers->zero = (operand == a);
    registers->negative = true;
    registers->halfCarry = alu->verifyBorrow(a, operand, 3);
    registers->carry = (operand > a);
}

void CPUOpt::compareB(uint8_t* instr) {
    baseCompare(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareC(uint8_t* instr) {
    baseCompare(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareD(uint8_t* instr) {
    baseCompare(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareE(uint8_t* instr) {
    baseCompare(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareH(uint8_t* instr) {
    baseCompare(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareL(uint8_t* instr) {
    baseCompare(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareHL(uint8_t* instr) {
    baseCompare(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::compareA(uint8_t* instr) {
    baseCompare(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::compareD8(uint8_t* instr) {
    baseCompare(instr[0]);
    programCounter = programCounter + 1;
    clockCycle = clockCycle + 1;
}


/* DECREMENT BLOCK */

uint8_t CPUOpt::baseDecrement(uint8_t operand) {
    uint8_t result = alu->subU8(operand, 1);

    registers->zero = (result == 0);
    registers->negative = true;
    registers->halfCarry = alu->verifyBorrow(operand, 1, 3);

    return result;
}

void CPUOpt::decB(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getB());
    registers->setB(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decC(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getC());
    registers->setC(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decD(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getD());
    registers->setD(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decE(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getE());
    registers->setE(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decH(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getH());
    registers->setH(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decL(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getL());
    registers->setL(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decA(uint8_t* instr) {
    uint8_t result = baseDecrement(registers->getA());
    registers->setA(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::decHL(uint8_t* instr) {
    uint8_t result = baseDecrement(bus->readByte(registers->getHL()));
    bus->writeByte(registers->getHL(), result);

    clockCycle = clockCycle + 3;
}



/* DECREMENT 16BTIS */

void CPUOpt::dec16HL(uint8_t* instr) {
    registers->setHL(alu->subU16(registers->getHL(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::dec16BC(uint8_t* instr) {
    registers->setBC(alu->subU16(registers->getBC(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::dec16DE(uint8_t* instr) {
    registers->setDE(alu->subU16(registers->getDE(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::dec16SP(uint8_t* instr) {
    stackPointer = alu->subU16(stackPointer, 1);
    clockCycle = clockCycle + 2;
}



/* INCREMENT BLOCK */

uint8_t CPUOpt::baseIncrement(uint8_t operand) {
    uint8_t result = alu->addU8(operand, 1);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(operand, 1, 3);

    return result;
}

void CPUOpt::incB(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getB());
    registers->setB(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incC(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getC());
    registers->setC(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incD(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getD());
    registers->setD(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incE(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getE());
    registers->setE(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incH(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getH());
    registers->setH(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incL(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getL());
    registers->setL(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incA(uint8_t* instr) {
    uint8_t result = baseIncrement(registers->getA());
    registers->setA(result);

    clockCycle = clockCycle + 1;
}

void CPUOpt::incHL(uint8_t* instr) {
    uint8_t result = baseIncrement(bus->readByte(registers->getHL()));
    bus->writeByte(registers->getHL(), result);

    clockCycle = clockCycle + 3;
}

/* INCREMENT 16 BITS */

void CPUOpt::inc16HL(uint8_t* instr) {
    registers->setHL(alu->addU16(registers->getHL(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::inc16BC(uint8_t* instr) {
    registers->setBC(alu->addU16(registers->getBC(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::inc16DE(uint8_t* instr) {
    registers->setDE(alu->addU16(registers->getDE(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::inc16SP(uint8_t* instr) {
    stackPointer = alu->addU16(stackPointer, 1);
    clockCycle = clockCycle + 2;
}


/* OR BLOCK */

void CPUOpt::baseOr(uint8_t operand) {
    uint8_t accumulator = registers->getA();
    uint8_t result = alu->orU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;

    registers->setA(result);
}

void CPUOpt::orB(uint8_t* instr) {
    baseOr(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orC(uint8_t* instr) {
    baseOr(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orD(uint8_t* instr) {
    baseOr(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orE(uint8_t* instr) {
    baseOr(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orH(uint8_t* instr) {
    baseOr(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orL(uint8_t* instr) {
    baseOr(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orHL(uint8_t* instr) {
    baseOr(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::orA(uint8_t* instr) {
    baseOr(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::orD8(uint8_t* instr) {
    baseOr(instr[0]);
    clockCycle = clockCycle + 1;
    programCounter = programCounter + 1;
}

/* SUB BLOCK */

void CPUOpt::baseSub(uint8_t operand, bool subc) {
    uint8_t carry = (registers->carry && subc) ? 1 : 0;

    uint8_t acumulator = registers->getA();
    uint8_t firstResult = alu->subU8(acumulator, operand);
    
    bool firstCarry = alu->overflow;
    bool firstHalf = alu->verifyBorrow(acumulator, operand, 3);

    uint8_t secondResult = alu->subU8(firstResult, carry);
    bool secondCarry = alu->overflow;
    bool secondHalf = alu->verifyBorrow(firstResult, carry, 3);

    registers->zero = (secondResult == 0);
    registers->negative = true;
    registers->halfCarry = (firstHalf | secondHalf);
    registers->carry = (firstCarry | secondCarry);

    registers->setA(secondResult);
}

void CPUOpt::subB(uint8_t* instr) {
    baseSub(registers->getB(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subC(uint8_t* instr) {
    baseSub(registers->getC(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subD(uint8_t* instr) {
    baseSub(registers->getD(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subE(uint8_t* instr) {
    baseSub(registers->getE(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subH(uint8_t* instr) {
    baseSub(registers->getH(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subL(uint8_t* instr) {
    baseSub(registers->getL(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subHL(uint8_t* instr) {
    baseSub(bus->readByte(registers->getHL()), false);
    clockCycle = clockCycle + 2;
}

void CPUOpt::subA(uint8_t* instr) {
    baseSub(registers->getA(), false);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subD8(uint8_t* instr) {
    baseSub(instr[0], false);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::subCarryB(uint8_t* instr) {
    baseSub(registers->getB(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryC(uint8_t* instr) {
    baseSub(registers->getC(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryD(uint8_t* instr) {
    baseSub(registers->getD(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryE(uint8_t* instr) {
    baseSub(registers->getE(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryH(uint8_t* instr) {
    baseSub(registers->getH(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryL(uint8_t* instr) {
    baseSub(registers->getL(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryHL(uint8_t* instr) {
    baseSub(bus->readByte(registers->getHL()), true);
    clockCycle = clockCycle + 2;
}

void CPUOpt::subCarryA(uint8_t* instr) {
    baseSub(registers->getA(), true);
    clockCycle = clockCycle + 1;
}

void CPUOpt::subCarryD8(uint8_t* instr) {
    baseSub(instr[0], true);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}


/* XOR BLOCK */
void CPUOpt::baseXor(uint8_t operand) {
    uint8_t accumulator = registers->getA();
    uint8_t result = alu->xorU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;

    registers->setA(result);
}

void CPUOpt::xorB(uint8_t* instr) {
    baseXor(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorC(uint8_t* instr) {
    baseXor(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorD(uint8_t* instr) {
    baseXor(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorE(uint8_t* instr) {
    baseXor(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorH(uint8_t* instr) {
    baseXor(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorL(uint8_t* instr) {
    baseXor(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorHL(uint8_t* instr) {
    baseXor(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::xorA(uint8_t* instr) {
    baseXor(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::xorD8(uint8_t* instr) {
    baseXor(instr[0]);
    clockCycle = clockCycle + 1;
    programCounter = programCounter + 1;
}


/* POP PUSH BLOCK */

void CPUOpt::popAF(uint8_t* instr) {
    uint16_t value = popFromStack();
    registers->setAF(value);
    clockCycle = clockCycle + 3;
}

void CPUOpt::popBC(uint8_t* instr) {
    uint16_t value = popFromStack();
    registers->setBC(value);
    clockCycle = clockCycle + 3;
}

void CPUOpt::popDE(uint8_t* instr) {
    uint16_t value = popFromStack();
    registers->setDE(value);
    clockCycle = clockCycle + 3;
}

void CPUOpt::popHL(uint8_t* instr) {
    uint16_t value = popFromStack();
    registers->setHL(value);
    clockCycle = clockCycle + 3;
}

void CPUOpt::pushAF(uint8_t* instr) {
    pushToStack(registers->getAF());
    clockCycle = clockCycle + 4;
}

void CPUOpt::pushBC(uint8_t* instr) {
    pushToStack(registers->getBC());
    clockCycle = clockCycle + 4;
}

void CPUOpt::pushDE(uint8_t* instr) {
    pushToStack(registers->getDE());
    clockCycle = clockCycle + 4;
}

void CPUOpt::pushHL(uint8_t* instr) {
    pushToStack(registers->getHL());
    clockCycle = clockCycle + 4;
}

/* ROTATE A BLOCK */
void CPUOpt::rlca(uint8_t* instr) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t a = registers->getA();
    uint8_t result = alu->rotateLeftCarry(a, carry);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    registers->setA(result);
    clockCycle = clockCycle + 1;
}

void CPUOpt::rla(uint8_t* instr) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t a = registers->getA();
    uint8_t result = alu->rotateLeft(a, carry);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    registers->setA(result);
    clockCycle = clockCycle + 1;
}

void CPUOpt::rrca(uint8_t* instr) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t a = registers->getA();
    uint8_t result = alu->rotateRightCarry(a, carry);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    registers->setA(result);
    clockCycle = clockCycle + 1;
}

void CPUOpt::rra(uint8_t* instr) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t a = registers->getA();
    uint8_t result = alu->rotateRight(a, carry);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    registers->setA(result);
    clockCycle = clockCycle + 1;
}

/* RST JUMP BLOCK */

void CPUOpt::rstV0(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[0];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV1(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[1];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV2(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[2];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV3(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[3];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV4(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[4];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV5(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[5];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV6(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[6];
    clockCycle = clockCycle + 4;
}

void CPUOpt::rstV7(uint8_t* instr) {
    pushToStack(programCounter);
    programCounter = rstVectors[7];
    clockCycle = clockCycle + 4;
}


/* JUMP BLOCK */

void CPUOpt::jumpHl(uint8_t* instr) {
    programCounter = registers->getHL();
    clockCycle = clockCycle + 2; 
}

void CPUOpt::jumpD16(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    programCounter = value;
    clockCycle = clockCycle + 4;
}

void CPUOpt::jumpNZ(uint8_t* instr) {
    if (!registers->zero) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        programCounter = value;
        clockCycle = clockCycle + 4;
    } else {
        programCounter = programCounter + 2;
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::jumpNC(uint8_t* instr) {
    if (!registers->carry) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        programCounter = value;
        clockCycle = clockCycle + 4;
    } else {
        programCounter = programCounter + 2;
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::jumpZ(uint8_t* instr) {
    if (registers->zero) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        programCounter = value;
        clockCycle = clockCycle + 4;
    } else {
        programCounter = programCounter + 2;
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::jumpCR(uint8_t* instr) {
    if (registers->carry) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        programCounter = value;
        clockCycle = clockCycle + 4;
    } else {
        programCounter = programCounter + 2;
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::jumpRelD16(uint8_t* instr) {
    int8_t offset = instr[0];
    programCounter = programCounter + 1;
    programCounter = alu->addAsSig(programCounter, offset);
    clockCycle = clockCycle + 3;
}

void CPUOpt::jumpRelNZ(uint8_t* instr) {
    if (!registers->zero) {
        int8_t offset = instr[0];
        programCounter = programCounter + 1;
        programCounter = alu->addAsSig(programCounter, offset);
        clockCycle = clockCycle + 3;
    } else {
        programCounter = programCounter + 1;
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::jumpRelNC(uint8_t* instr) {
    if (!registers->carry) {
        int8_t offset = instr[0];
        programCounter = programCounter + 1;
        programCounter = alu->addAsSig(programCounter, offset);
        clockCycle = clockCycle + 3;
    } else {
        programCounter = programCounter + 1;
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::jumpRelZ(uint8_t* instr) {
    if (registers->zero) {
        int8_t offset = instr[0];
        programCounter = programCounter + 1;
        programCounter = alu->addAsSig(programCounter, offset);
        clockCycle = clockCycle + 3;
    } else {
        programCounter = programCounter + 1;
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::jumpRelCR(uint8_t* instr) {
    if (registers->carry) {
        int8_t offset = instr[0];
        programCounter = programCounter + 1;
        programCounter = alu->addAsSig(programCounter, offset);
        clockCycle = clockCycle + 3;
    } else {
        programCounter = programCounter + 1;
        clockCycle = clockCycle + 2;
    }
}



/* CALL BLOCK */

void CPUOpt::callD16(uint8_t* instr) {
    programCounter = programCounter + 2;
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    pushToStack(programCounter);
    programCounter = value;
    clockCycle = clockCycle + 6;
}

void CPUOpt::callNZ(uint8_t* instr) {
    programCounter = programCounter + 2;
    if (!registers->zero) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        pushToStack(programCounter);
        programCounter = value;
        clockCycle = clockCycle + 6;
    } else {
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::callNC(uint8_t* instr) {
    programCounter = programCounter + 2;
    if (!registers->carry) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        pushToStack(programCounter);
        programCounter = value;
        clockCycle = clockCycle + 6;
    } else {
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::callZ(uint8_t* instr) {
    programCounter = programCounter + 2;
    if (registers->zero) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        pushToStack(programCounter);
        programCounter = value;
        clockCycle = clockCycle + 6;
    } else {
        clockCycle = clockCycle + 3;
    }
}

void CPUOpt::callCR(uint8_t* instr) {
    programCounter = programCounter + 2;
    if (registers->carry) {
        uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
        pushToStack(programCounter);
        programCounter = value;
        clockCycle = clockCycle + 6;
    } else {

        clockCycle = clockCycle + 3;
    }
}



/* RET BLOCK */

void CPUOpt::retD16(uint8_t* instr) {
    uint16_t returnAddress = popFromStack();
    programCounter = returnAddress;
    clockCycle = clockCycle + 4;
}

void CPUOpt::retNZ(uint8_t* instr) {
    if (!registers->zero) {
        uint16_t returnAddress = popFromStack();
        programCounter = returnAddress;
        clockCycle = clockCycle + 5;
    } else {
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::retNC(uint8_t* instr) {
    if (!registers->carry) {
        uint16_t returnAddress = popFromStack();
        programCounter = returnAddress;
        clockCycle = clockCycle + 5;
    } else {
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::retZ(uint8_t* instr) {
    if (registers->zero) {
        uint16_t returnAddress = popFromStack();
        programCounter = returnAddress;
        clockCycle = clockCycle + 5;
    } else {
        clockCycle = clockCycle + 2;
    }
}

void CPUOpt::retCR(uint8_t* instr) {
    if (registers->carry) {
        uint16_t returnAddress = popFromStack();
        programCounter = returnAddress;
        clockCycle = clockCycle + 5;
    } else {
        clockCycle = clockCycle + 2;
    }
}



/* 16 BIT ADD */

void CPUOpt::baseAddHL(uint16_t operand) {
    uint16_t accumulator = registers->getHL();
    uint16_t result = alu->addU16(accumulator, operand);

    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(accumulator, operand, 11);
    registers->carry = alu->overflow;

    registers->setHL(result);
}

void CPUOpt::addHlBc(uint8_t* instr) {
    baseAddHL(registers->getBC());
    clockCycle = clockCycle + 2;
}

void CPUOpt::addHlDe(uint8_t* instr) {
    baseAddHL(registers->getDE());
    clockCycle = clockCycle + 2;
}

void CPUOpt::addHlHl(uint8_t* instr) {
    baseAddHL(registers->getHL());
    clockCycle = clockCycle + 2;
}

void CPUOpt::addHlSp(uint8_t* instr) {
    baseAddHL(stackPointer);
    clockCycle = clockCycle + 2;
}

void CPUOpt::addSp(uint8_t* instr) {
    uint16_t accumulator = stackPointer;
    int8_t operand = instr[0];

    uint16_t result = alu->addAsSig(accumulator, operand);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(accumulator, operand, 3);
    registers->carry = alu->verifyOverflow(accumulator, operand, 7);

    stackPointer = result;

    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}


/* ESPECIAL INSTRUCTIONS */

void CPUOpt::ccf(uint8_t* instr) {
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = !registers->carry;

    clockCycle= clockCycle + 1;
}

void CPUOpt::cpl(uint8_t* instr) { 
    uint8_t result = alu->notU8(registers->getA());
    registers->setA(result);

    registers->negative = true;
    registers->halfCarry = true;

    clockCycle= clockCycle + 1;
}

void CPUOpt::sfc(uint8_t* instr) {
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = true;

    clockCycle= clockCycle + 1;
}

void CPUOpt::daa(uint8_t* instr) {
    bool newCarry = false;

    if (registers->negative) {
        uint8_t adjust = 0;

        if (registers->halfCarry)
            adjust = alu->addU8(adjust, 0x06);

        if (registers->carry) {
            adjust = alu->addU8(adjust, 0x60);
            newCarry = true;
        }

        uint8_t result = alu->subU8(registers->getA(), adjust);
        registers->setA(result);
        registers->zero = (result == 0);

    } else {
        uint8_t a = registers->getA();
        uint8_t adjust = 0;

        if (registers->halfCarry || (a & 0x0F) > 0x09)
            adjust = alu->addU8(adjust, 0x06);

        if (registers->carry || a > 0x99) {
            adjust = alu->addU8(adjust, 0x60);
            newCarry = true;
        }

        uint8_t result = alu->addU8(a, adjust);
        registers->setA(result);
        registers->zero = (result == 0);
    }

    registers->halfCarry = false;
    registers->carry = newCarry;

    clockCycle= clockCycle + 1;
}

void CPUOpt::di(uint8_t* instr) {
    interruptEnable = false;
    clockCycle= clockCycle + 1;
}

void CPUOpt::ei(uint8_t* instr) {
    interruptEnable = true;
    clockCycle= clockCycle + 1;
}

void CPUOpt::halt(uint8_t* instr) {
    if (interruptEnable == true) {
        isHalt = true;
    } else {
        if (bus->verifyPendingInterrupts() != 0) {
            isHalt = false;
        } else {
            isHalt = true;
        }
    }
    clockCycle= clockCycle + 1;
}

void CPUOpt::reti(uint8_t* instr) {
    interruptEnable = true;
    programCounter = popFromStack();
    clockCycle= clockCycle + 4;
}



/* BASIC LOAD BLOCK */

void CPUOpt::load_A_A(uint8_t* instr) {
    registers->setA(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_B(uint8_t* instr) {
    registers->setA(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_C(uint8_t* instr) {
    registers->setA(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_D(uint8_t* instr) {
    registers->setA(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_E(uint8_t* instr) {
    registers->setA(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_H(uint8_t* instr) {
    registers->setA(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_A_L(uint8_t* instr) {
    registers->setA(registers->getL());
    clockCycle = clockCycle + 1;
}


void CPUOpt::load_B_A(uint8_t* instr) {
    registers->setB(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_B(uint8_t* instr) {
    registers->setB(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_C(uint8_t* instr) {
    registers->setB(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_D(uint8_t* instr) {
    registers->setB(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_E(uint8_t* instr) {
    registers->setB(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_H(uint8_t* instr) {
    registers->setB(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_B_L(uint8_t* instr) {
    registers->setB(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_A(uint8_t* instr) {
    registers->setC(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_B(uint8_t* instr) {
    registers->setC(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_C(uint8_t* instr) {
    registers->setC(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_D(uint8_t* instr) {
    registers->setC(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_E(uint8_t* instr) {
    registers->setC(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_H(uint8_t* instr) {
    registers->setC(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_C_L(uint8_t* instr) {
    registers->setC(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_A(uint8_t* instr) {
    registers->setD(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_B(uint8_t* instr) {
    registers->setD(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_C(uint8_t* instr) {
    registers->setD(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_D(uint8_t* instr) {
    registers->setD(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_E(uint8_t* instr) {
    registers->setD(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_H(uint8_t* instr) {
    registers->setD(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_D_L(uint8_t* instr) {
    registers->setD(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_A(uint8_t* instr) {
    registers->setE(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_B(uint8_t* instr) {
    registers->setE(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_C(uint8_t* instr) {
    registers->setE(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_D(uint8_t* instr) {
    registers->setE(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_E(uint8_t* instr) {
    registers->setE(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_H(uint8_t* instr) {
    registers->setE(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_E_L(uint8_t* instr) {
    registers->setE(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_A(uint8_t* instr) {
    registers->setH(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_B(uint8_t* instr) {
    registers->setH(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_C(uint8_t* instr) {
    registers->setH(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_D(uint8_t* instr) {
    registers->setH(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_E(uint8_t* instr) {
    registers->setH(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_H(uint8_t* instr) {
    registers->setH(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_H_L(uint8_t* instr) {
    registers->setH(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_A(uint8_t* instr) {
    registers->setL(registers->getA());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_B(uint8_t* instr) {
    registers->setL(registers->getB());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_C(uint8_t* instr) {
    registers->setL(registers->getC());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_D(uint8_t* instr) {
    registers->setL(registers->getD());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_E(uint8_t* instr) {
    registers->setL(registers->getE());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_H(uint8_t* instr) {
    registers->setL(registers->getH());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_L_L(uint8_t* instr) {
    registers->setL(registers->getL());
    clockCycle = clockCycle + 1;
}

void CPUOpt::load_HL_A(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getA());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_B(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getB());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_C(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getC());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_D(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getD());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_E(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getE());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_H(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getH());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_HL_L(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getL());
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_A_HL(uint8_t* instr) {
    registers->setA(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_B_HL(uint8_t* instr) {
    registers->setB(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_C_HL(uint8_t* instr) {
    registers->setC(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_D_HL(uint8_t* instr) {
    registers->setD(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_E_HL(uint8_t* instr) {
    registers->setE(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_H_HL(uint8_t* instr) {
    registers->setH(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_L_HL(uint8_t* instr) {
    registers->setL(bus->readByte(registers->getHL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::load_A_D8(uint8_t* instr) {
    registers->setA(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_B_D8(uint8_t* instr) {
    registers->setB(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_C_D8(uint8_t* instr) {
    registers->setC(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_D_D8(uint8_t* instr) {
    registers->setD(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_E_D8(uint8_t* instr) {
    registers->setE(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_H_D8(uint8_t* instr) {
    registers->setH(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_L_D8(uint8_t* instr) {
    registers->setL(instr[0]);
    clockCycle = clockCycle + 2;
    programCounter = programCounter + 1;
}

void CPUOpt::load_HL_D8(uint8_t* instr) {
    bus->writeByte(registers->getHL(), instr[0]);
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 1;
}


/* LOAD POINTER BLOCK */

void CPUOpt::loadPointer_BC_A(uint8_t* instr) {
    bus->writeByte(registers->getBC(), registers->getA());
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_DE_A(uint8_t* instr) {
    bus->writeByte(registers->getDE(), registers->getA());
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_A_BC(uint8_t* instr) {
    registers->setA(bus->readByte(registers->getBC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_A_DE(uint8_t* instr) {
    registers->setA(bus->readByte(registers->getDE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_A_D16(uint8_t* instr) {
    uint16_t addr = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    registers->setA(bus->readByte(addr));
    clockCycle = clockCycle + 4;
    programCounter = programCounter + 2;
}

void CPUOpt::loadPointer_D16_A(uint8_t* instr) {
    uint16_t addr = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    bus->writeByte(addr, registers->getA());
    clockCycle = clockCycle + 4;
    programCounter = programCounter + 2;
}

void CPUOpt::loadPointer_HLI_A(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getA());
    registers->setHL(registers->getHL() + 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_HLD_A(uint8_t* instr) {
    bus->writeByte(registers->getHL(), registers->getA());
    registers->setHL(registers->getHL() - 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_A_HLI(uint8_t* instr) {
    registers->setA(bus->readByte(registers->getHL()));
    registers->setHL(registers->getHL() + 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadPointer_A_HLD(uint8_t* instr) {
    registers->setA(bus->readByte(registers->getHL()));
    registers->setHL(registers->getHL() - 1);
    clockCycle = clockCycle + 2;
}

/* LOAD STACK BLOCK */

void CPUOpt::loadStack_D16_SP(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    bus->writeByte(value, static_cast<uint8_t>(stackPointer));
    bus->writeByte(value + 1, static_cast<uint8_t>(stackPointer >> 8));
    clockCycle = clockCycle + 5;
    programCounter = programCounter + 2;
}

void CPUOpt::loadStack_SP_HL(uint8_t* instr) {
    stackPointer = registers->getHL();
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadStack_HL_SPe(uint8_t* instr) {
    uint16_t value = alu->addAsSig(stackPointer, instr[0]);
    registers->setHL(value);
    registers->zero = false;  
    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(stackPointer, instr[0], 3);
    registers->carry = alu->verifyOverflow(stackPointer, instr[0], 7);

    clockCycle = clockCycle + 3;
    programCounter = programCounter + 1;
}

/* LOAD HIGH MEMORY */

void CPUOpt::loadHigh_A_D8(uint8_t* instr) {
    uint16_t addr = 0xFF00 + instr[0];
    bus->writeByte(addr, registers->getA());
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 1;
}

void CPUOpt::loadHigh_A_C(uint8_t* instr) {
    uint16_t addr = 0xFF00 + registers->getC();
    bus->writeByte(addr, registers->getA());
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadHigh_C_A(uint8_t* instr) {
    uint16_t addr = 0xFF00 + registers->getC();
    registers->setA(bus->readByte(addr));
    clockCycle = clockCycle + 2;
}

void CPUOpt::loadHigh_D8_A(uint8_t* instr) {
    uint16_t addr = 0xFF00 + instr[0];
    registers->setA(bus->readByte(addr));
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 1;
}

/*  LOAD 16 BLOCK */

void CPUOpt::load16Bc(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    registers->setBC(value);
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 2;
}

void CPUOpt::load16De(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    registers->setDE(value);
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 2;
}

void CPUOpt::load16Hl(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    registers->setHL(value);
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 2;
}

void CPUOpt::load16Sp(uint8_t* instr) {
    uint16_t value = (static_cast<uint16_t>(instr[1])) << 8 | static_cast<uint16_t>(instr[0]);
    stackPointer = value;
    clockCycle = clockCycle + 3;
    programCounter = programCounter + 2;
}


/*  PREFIX ROTATE LEFT */

uint8_t CPUOpt::baseRL(uint8_t operand) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t result = alu->rotateLeft(operand, carry);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::rlA(uint8_t* instr) {
    registers->setA(baseRL(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlB(uint8_t* instr) {
    registers->setB(baseRL(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlC(uint8_t* instr) {
    registers->setC(baseRL(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlD(uint8_t* instr) {
    registers->setD(baseRL(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlE(uint8_t* instr) {
    registers->setE(baseRL(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlH(uint8_t* instr) {
    registers->setH(baseRL(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlL(uint8_t* instr) {
    registers->setL(baseRL(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    uint8_t result = baseRL(value);
    bus->writeByte(registers->getHL(), result);
    clockCycle = clockCycle + 4;
}


/* PREFIX ROTATE LEFT CARRY */
uint8_t CPUOpt::baseRLC(uint8_t operand) {
    uint8_t result = alu->rotateLeftCarry(operand, 0);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::rlcA(uint8_t* instr) {
    registers->setA(baseRLC(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcB(uint8_t* instr) {
    registers->setB(baseRLC(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcC(uint8_t* instr) {
    registers->setC(baseRLC(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcD(uint8_t* instr) {
    registers->setD(baseRLC(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcE(uint8_t* instr) {
    registers->setE(baseRLC(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcH(uint8_t* instr) {
    registers->setH(baseRLC(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcL(uint8_t* instr) {
    registers->setL(baseRLC(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rlcHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    uint8_t result = baseRLC(value);
    bus->writeByte(registers->getHL(), result);
    clockCycle = clockCycle + 4;
}

/* PREFIX ROTATE RIGHT */
uint8_t CPUOpt::baseRR(uint8_t operand) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t result = alu->rotateRight(operand, carry);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::rrA(uint8_t* instr) {
    registers->setA(baseRR(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrB(uint8_t* instr) {
    registers->setB(baseRR(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrC(uint8_t* instr) {
    registers->setC(baseRR(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrD(uint8_t* instr) {
    registers->setD(baseRR(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrE(uint8_t* instr) {
    registers->setE(baseRR(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrH(uint8_t* instr) {
    registers->setH(baseRR(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrL(uint8_t* instr) {
    registers->setL(baseRR(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    uint8_t result = baseRR(value);
    bus->writeByte(registers->getHL(), result);
    clockCycle = clockCycle + 4;
}

/* PREFIX ROTATE RIGHT CARRY */
uint8_t CPUOpt::baseRRC(uint8_t operand) {
    uint8_t result = alu->rotateRightCarry(operand, 0);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::rrcA(uint8_t* instr) {
    registers->setA(baseRRC(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcB(uint8_t* instr) {
    registers->setB(baseRRC(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcC(uint8_t* instr) {
    registers->setC(baseRRC(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcD(uint8_t* instr) {
    registers->setD(baseRRC(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcE(uint8_t* instr) {
    registers->setE(baseRRC(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcH(uint8_t* instr) {
    registers->setH(baseRRC(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcL(uint8_t* instr) {
    registers->setL(baseRRC(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::rrcHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    uint8_t result = baseRRC(value);
    bus->writeByte(registers->getHL(), result);
    clockCycle = clockCycle + 4;
}

/* BIT OPERATIONS */

void CPUOpt::baseBit(uint8_t operand, uint8_t bit) {
    bool bitSet = alu->verifyBit(operand, bit);

    registers->zero = !bitSet;
    registers->negative = false;
    registers->halfCarry = true;
}

void CPUOpt::bit0A(uint8_t* instr) {
    baseBit(registers->getA(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0B(uint8_t* instr) {
    baseBit(registers->getB(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0C(uint8_t* instr) {
    baseBit(registers->getC(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0D(uint8_t* instr) {
    baseBit(registers->getC(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0E(uint8_t* instr) {
    baseBit(registers->getE(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0H(uint8_t* instr) {
    baseBit(registers->getH(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0L(uint8_t* instr) {
    baseBit(registers->getL(), 0);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit0HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 0);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit1A(uint8_t* instr) {
    baseBit(registers->getA(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1B(uint8_t* instr) {
    baseBit(registers->getB(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1C(uint8_t* instr) {
    baseBit(registers->getC(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1D(uint8_t* instr) {
    baseBit(registers->getC(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1E(uint8_t* instr) {
    baseBit(registers->getE(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1H(uint8_t* instr) {
    baseBit(registers->getH(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1L(uint8_t* instr) {
    baseBit(registers->getL(), 1);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit1HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 1);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit2A(uint8_t* instr) {
    baseBit(registers->getA(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2B(uint8_t* instr) {
    baseBit(registers->getB(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2C(uint8_t* instr) {
    baseBit(registers->getC(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2D(uint8_t* instr) {
    baseBit(registers->getC(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2E(uint8_t* instr) {
    baseBit(registers->getE(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2H(uint8_t* instr) {
    baseBit(registers->getH(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2L(uint8_t* instr) {
    baseBit(registers->getL(), 2);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit2HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 2);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit3A(uint8_t* instr) {
    baseBit(registers->getA(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3B(uint8_t* instr) {
    baseBit(registers->getB(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3C(uint8_t* instr) {
    baseBit(registers->getC(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3D(uint8_t* instr) {
    baseBit(registers->getD(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3E(uint8_t* instr) {
    baseBit(registers->getE(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3H(uint8_t* instr) {
    baseBit(registers->getH(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3L(uint8_t* instr) {
    baseBit(registers->getL(), 3);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit3HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 3);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit4A(uint8_t* instr) {
    baseBit(registers->getA(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4B(uint8_t* instr) {
    baseBit(registers->getB(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4C(uint8_t* instr) {
    baseBit(registers->getC(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4D(uint8_t* instr) {
    baseBit(registers->getD(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4E(uint8_t* instr) {
    baseBit(registers->getE(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4H(uint8_t* instr) {
    baseBit(registers->getH(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4L(uint8_t* instr) {
    baseBit(registers->getL(), 4);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit4HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 4);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit5A(uint8_t* instr) {
    baseBit(registers->getA(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5B(uint8_t* instr) {
    baseBit(registers->getB(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5C(uint8_t* instr) {
    baseBit(registers->getC(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5D(uint8_t* instr) {
    baseBit(registers->getD(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5E(uint8_t* instr) {
    baseBit(registers->getE(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5H(uint8_t* instr) {
    baseBit(registers->getH(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5L(uint8_t* instr) {
    baseBit(registers->getL(), 5);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit5HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 5);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit6A(uint8_t* instr) {
    baseBit(registers->getA(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6B(uint8_t* instr) {
    baseBit(registers->getB(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6C(uint8_t* instr) {
    baseBit(registers->getC(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6D(uint8_t* instr) {
    baseBit(registers->getD(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6E(uint8_t* instr) {
    baseBit(registers->getE(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6H(uint8_t* instr) {
    baseBit(registers->getH(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6L(uint8_t* instr) {
    baseBit(registers->getL(), 6);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit6HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 6);
    clockCycle = clockCycle + 3;
}

void CPUOpt::bit7A(uint8_t* instr) {
    baseBit(registers->getA(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7B(uint8_t* instr) {
    baseBit(registers->getB(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7C(uint8_t* instr) {
    baseBit(registers->getC(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7D(uint8_t* instr) {
    baseBit(registers->getD(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7E(uint8_t* instr) {
    baseBit(registers->getE(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7H(uint8_t* instr) {
    baseBit(registers->getH(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7L(uint8_t* instr) {
    baseBit(registers->getL(), 7);
    clockCycle = clockCycle + 2;
}

void CPUOpt::bit7HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    baseBit(value, 7);
    clockCycle = clockCycle + 3;
}

/* SRL OPERATIONS */
uint8_t CPUOpt::baseSRL(uint8_t operand) {
    uint8_t result = alu->shiftRightLogical(operand, 0);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::srlA(uint8_t* instr) {
    registers->setA(baseSRL(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlB(uint8_t* instr) {
    registers->setB(baseSRL(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlC(uint8_t* instr) {
    registers->setC(baseSRL(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlD(uint8_t* instr) {
    registers->setD(baseSRL(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlE(uint8_t* instr) {
    registers->setE(baseSRL(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlH(uint8_t* instr) {
    registers->setH(baseSRL(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlL(uint8_t* instr) {
    registers->setL(baseSRL(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::srlHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), baseSRL(value));
    clockCycle = clockCycle + 3;
}

/* SWAP BLOCK */

uint8_t CPUOpt::baseSwap(uint8_t operand) {
    uint8_t result = alu->swapU8(operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;

    return result;
}

void CPUOpt::swapA(uint8_t* instr) {
    registers->setA(baseSwap(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapB(uint8_t* instr) {
    registers->setB(baseSwap(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapC(uint8_t* instr) {
    registers->setC(baseSwap(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapD(uint8_t* instr) {
    registers->setD(baseSwap(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapE(uint8_t* instr) {
    registers->setE(baseSwap(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapH(uint8_t* instr) {
    registers->setH(baseSwap(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapL(uint8_t* instr) {
    registers->setL(baseSwap(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::swapHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), baseSwap(value));
    clockCycle = clockCycle + 3;
}


/* SLA BLOCK */
uint8_t CPUOpt::baseSLA(uint8_t operand) {
    uint8_t result = alu->shiftLeftA(operand, 0);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::slaA(uint8_t* instr) {
    registers->setA(baseSLA(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaB(uint8_t* instr) {
    registers->setB(baseSLA(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaC(uint8_t* instr) {
    registers->setC(baseSLA(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaD(uint8_t* instr) {
    registers->setD(baseSLA(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaE(uint8_t* instr) {
    registers->setE(baseSLA(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaH(uint8_t* instr) {
    registers->setH(baseSLA(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaL(uint8_t* instr) {
    registers->setL(baseSLA(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::slaHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), baseSLA(value));
    clockCycle = clockCycle + 4;
}

/* SRA BLOCK */
uint8_t CPUOpt::baseSRA(uint8_t operand) {
    uint8_t result = alu->shiftRightA(operand, 0);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;

    return result;
}

void CPUOpt::sraA(uint8_t* instr) {
    registers->setA(baseSRA(registers->getA()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraB(uint8_t* instr) {
    registers->setB(baseSRA(registers->getB()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraC(uint8_t* instr) {
    registers->setC(baseSRA(registers->getC()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraD(uint8_t* instr) {
    registers->setD(baseSRA(registers->getD()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraE(uint8_t* instr) {
    registers->setE(baseSRA(registers->getE()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraH(uint8_t* instr) {
    registers->setH(baseSRA(registers->getH()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraL(uint8_t* instr) {
    registers->setL(baseSRA(registers->getL()));
    clockCycle = clockCycle + 2;
}

void CPUOpt::sraHL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), baseSRA(value));
    clockCycle = clockCycle + 4;
}


/* RESET BIT BLOCK */

void CPUOpt::res0A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res0HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 0));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res1A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res1HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 1));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res2A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res2HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 2));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res3A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res3HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 3));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res4A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res4HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 4));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res5A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res5HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 5));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res6A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res6HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 6));
    clockCycle = clockCycle + 4;
}

void CPUOpt::res7A(uint8_t* instr) {
    registers->setA(alu->resetBit(registers->getA(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7B(uint8_t* instr) {
    registers->setB(alu->resetBit(registers->getB(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7C(uint8_t* instr) {
    registers->setC(alu->resetBit(registers->getC(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7D(uint8_t* instr) {
    registers->setD(alu->resetBit(registers->getD(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7E(uint8_t* instr) {
    registers->setE(alu->resetBit(registers->getE(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7H(uint8_t* instr) {
    registers->setH(alu->resetBit(registers->getH(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7L(uint8_t* instr) {
    registers->setL(alu->resetBit(registers->getL(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::res7HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->resetBit(value, 7));
    clockCycle = clockCycle + 4;
}

// ==========================================
// SET (Set Bit) Operations - Direct Registers (2 cycles)
// ==========================================

void CPUOpt::set0A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 0));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set0HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 0));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set1A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 1));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set1HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 1));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set2A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 2));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set2HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 2));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set3A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 3));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set3HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 3));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set4A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 4));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set4HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 4));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set5A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 5));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set5HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 5));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set6A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 6));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set6HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 6));
    clockCycle = clockCycle + 4;
}

void CPUOpt::set7A(uint8_t* instr) {
    registers->setA(alu->setBit(registers->getA(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7B(uint8_t* instr) {
    registers->setB(alu->setBit(registers->getB(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7C(uint8_t* instr) {
    registers->setC(alu->setBit(registers->getC(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7D(uint8_t* instr) {
    registers->setD(alu->setBit(registers->getD(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7E(uint8_t* instr) {
    registers->setE(alu->setBit(registers->getE(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7H(uint8_t* instr) {
    registers->setH(alu->setBit(registers->getH(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7L(uint8_t* instr) {
    registers->setL(alu->setBit(registers->getL(), 7));
    clockCycle = clockCycle + 2;
}

void CPUOpt::set7HL(uint8_t* instr) {
    uint8_t value = bus->readByte(registers->getHL());
    bus->writeByte(registers->getHL(), alu->setBit(value, 7));
    clockCycle = clockCycle + 4;
}



/* COMMON UTILITIES */

void CPUOpt::pushToStack(uint16_t addr) {
    uint8_t lowByte = static_cast<uint8_t>(addr);
    uint8_t highByte = static_cast<uint8_t>((addr) >> 8);

    stackPointer -= 1;
    bus->writeByte(stackPointer, highByte);
    stackPointer -= 1;
    bus->writeByte(stackPointer, lowByte);
}

uint16_t CPUOpt::popFromStack() {
    uint8_t* bytes = bus->fetchBlock(stackPointer);
    stackPointer += 2;

    return (static_cast<uint16_t>(bytes[1])) << 8 | static_cast<uint16_t>(bytes[0]);
}

void CPUOpt::handleInterrupts(uint8_t pendingInterrupts) {
    uint8_t interruptIndex = getFirstInterrupt(pendingInterrupts);
    if (interruptIndex == 7) {
        return;
    }
    isHalt = false;
    interruptEnable = false;
    clockCycle += 5;
    bus->clearInterruptionRequest(interruptIndex);
    pushToStack(programCounter);
    programCounter = interruptVector[interruptIndex];
}

uint8_t CPUOpt::getFirstInterrupt(uint8_t pendingInterrupts) {
    for(uint8_t i=0; i<5; i++) {
        if ((pendingInterrupts & 0x01) !=0) {
            return i;
        }
        pendingInterrupts = pendingInterrupts >> 1;
    }

    return 7;
}