#include "instructions.h"

InstructionDefinition::InstructionDefinition(InstructionGroup group, U8Reg def_1, U8Reg def_2, uint8_t operandsNumber, uint8_t cycles) {
    this->group = group;
    this->def_1 = def_1;
    this->def_2 = def_2;
    this->operandsNumber = operandsNumber;
    this->cycles = cycles;
}

Instruction::Instruction() {}

InstructionDict::InstructionDict() {
    dict[0x00] = new InstructionDefinition(InstructionGroup::NOP, U8Reg::X, U8Reg::X, 0, 1);

    dict[0x80] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::B,  U8Reg::A, 0, 1);
    dict[0x81] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::C,  U8Reg::A, 0, 1);
    dict[0x82] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::D,  U8Reg::A, 0, 1);
    dict[0x83] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::E,  U8Reg::A, 0, 1);
    dict[0x84] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::H,  U8Reg::A, 0, 1);
    dict[0x85] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::L,  U8Reg::A, 0, 1);
    dict[0x86] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::HL, U8Reg::A, 0, 2);
    dict[0x87] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::A,  U8Reg::A, 0, 1);
    dict[0xC6] = new InstructionDefinition(InstructionGroup::ADD,  U8Reg::D8, U8Reg::A, 1, 2);

    dict[0x88] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::B,  U8Reg::A, 0, 1);
    dict[0x89] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::C,  U8Reg::A, 0, 1);
    dict[0x8A] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::D,  U8Reg::A, 0, 1);
    dict[0x8B] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::E,  U8Reg::A, 0, 1);
    dict[0x8C] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::H,  U8Reg::A, 0, 1);
    dict[0x8D] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::L,  U8Reg::A, 0, 1);
    dict[0x8E] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::HL, U8Reg::A, 0, 2);
    dict[0x8F] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::A,  U8Reg::A, 0, 1);
    dict[0xCE] = new InstructionDefinition(InstructionGroup::ADDC, U8Reg::D8, U8Reg::A, 1, 2);

    dict[0xA0] = new InstructionDefinition(InstructionGroup::AND, U8Reg::B,  U8Reg::A, 0, 1);
    dict[0xA1] = new InstructionDefinition(InstructionGroup::AND, U8Reg::C,  U8Reg::A, 0, 1);
    dict[0xA2] = new InstructionDefinition(InstructionGroup::AND, U8Reg::D,  U8Reg::A, 0, 1);
    dict[0xA3] = new InstructionDefinition(InstructionGroup::AND, U8Reg::E,  U8Reg::A, 0, 1);
    dict[0xA4] = new InstructionDefinition(InstructionGroup::AND, U8Reg::H,  U8Reg::A, 0, 1);
    dict[0xA5] = new InstructionDefinition(InstructionGroup::AND, U8Reg::L,  U8Reg::A, 0, 1);
    dict[0xA6] = new InstructionDefinition(InstructionGroup::AND, U8Reg::HL, U8Reg::A, 0, 1);
    dict[0xA7] = new InstructionDefinition(InstructionGroup::AND, U8Reg::A,  U8Reg::A, 0, 1);
    dict[0xE6] = new InstructionDefinition(InstructionGroup::AND, U8Reg::D8, U8Reg::A, 1, 2);

    dict[0xE9] = new InstructionDefinition(InstructionGroup::JP, U8Reg::HL, U8Reg::X, 0, 1);
    dict[0xC3] = new InstructionDefinition(InstructionGroup::JP, U8Reg::D16, U8Reg::X, 2, 4);
    dict[0xC2] = new InstructionDefinition(InstructionGroup::JP, U8Reg::NZ, U8Reg::X, 2, 3);
    dict[0xD2] = new InstructionDefinition(InstructionGroup::JP, U8Reg::NC, U8Reg::X, 2, 3);
    dict[0xCA] = new InstructionDefinition(InstructionGroup::JP, U8Reg::Z, U8Reg::X, 2, 3);
    dict[0xDA] = new InstructionDefinition(InstructionGroup::JP, U8Reg::CR, U8Reg::X, 2, 3);

    dict[0x18] = new InstructionDefinition(InstructionGroup::JPR, U8Reg::D16, U8Reg::X, 1, 2);
    dict[0x20] = new InstructionDefinition(InstructionGroup::JPR, U8Reg::NZ, U8Reg::X, 1, 2);
    dict[0x30] = new InstructionDefinition(InstructionGroup::JPR, U8Reg::NC, U8Reg::X, 1, 2);
    dict[0x28] = new InstructionDefinition(InstructionGroup::JPR, U8Reg::Z, U8Reg::X, 1, 2);
    dict[0x38] = new InstructionDefinition(InstructionGroup::JPR, U8Reg::CR, U8Reg::X, 1, 2);

    dict[0xCD] = new InstructionDefinition(InstructionGroup::CALL, U8Reg::D16, U8Reg::X, 2, 6);
    dict[0xC4] = new InstructionDefinition(InstructionGroup::CALL, U8Reg::NZ, U8Reg::X, 2, 3);
    dict[0xD4] = new InstructionDefinition(InstructionGroup::CALL, U8Reg::NC, U8Reg::X, 2, 3);
    dict[0xCC] = new InstructionDefinition(InstructionGroup::CALL, U8Reg::Z, U8Reg::X, 2, 3);
    dict[0xDC] = new InstructionDefinition(InstructionGroup::CALL, U8Reg::CR, U8Reg::X, 2, 3);

    dict[0xC9] = new InstructionDefinition(InstructionGroup::RET, U8Reg::D16, U8Reg::X, 0, 4);
    dict[0xC0] = new InstructionDefinition(InstructionGroup::RET, U8Reg::NZ, U8Reg::X, 0, 2);
    dict[0xD0] = new InstructionDefinition(InstructionGroup::RET, U8Reg::NC, U8Reg::X, 0, 2);
    dict[0xC8] = new InstructionDefinition(InstructionGroup::RET, U8Reg::Z, U8Reg::X, 0, 2);
    dict[0xD8] = new InstructionDefinition(InstructionGroup::RET, U8Reg::CR, U8Reg::X, 0, 2);

    dict[0x09] = new InstructionDefinition(InstructionGroup::ADDHL, U8Reg::BC, U8Reg::X, 0, 2);
    dict[0x19] = new InstructionDefinition(InstructionGroup::ADDHL, U8Reg::DE, U8Reg::X, 0, 2);
    dict[0x29] = new InstructionDefinition(InstructionGroup::ADDHL, U8Reg::HL, U8Reg::X, 0, 2);
    dict[0x39] = new InstructionDefinition(InstructionGroup::ADDHL, U8Reg::SP, U8Reg::X, 0, 2);

    dict[0xE8] = new InstructionDefinition(InstructionGroup::ADDSP, U8Reg::D8, U8Reg::X, 1, 2);

    dict[0x3F] = new InstructionDefinition(InstructionGroup::CCF, U8Reg::X, U8Reg::X, 0, 1);
    dict[0x2F] = new InstructionDefinition(InstructionGroup::CPL, U8Reg::X, U8Reg::X, 0, 1);
    dict[0x27] = new InstructionDefinition(InstructionGroup::DAA, U8Reg::X, U8Reg::X, 0, 1);

    dict[0xB8] = new InstructionDefinition(InstructionGroup::CP, U8Reg::B,  U8Reg::A, 0, 1);
    dict[0xB9] = new InstructionDefinition(InstructionGroup::CP, U8Reg::C,  U8Reg::A, 0, 1);
    dict[0xBA] = new InstructionDefinition(InstructionGroup::CP, U8Reg::D,  U8Reg::A, 0, 1);
    dict[0xBB] = new InstructionDefinition(InstructionGroup::CP, U8Reg::E,  U8Reg::A, 0, 1);
    dict[0xBC] = new InstructionDefinition(InstructionGroup::CP, U8Reg::H,  U8Reg::A, 0, 1);
    dict[0xBD] = new InstructionDefinition(InstructionGroup::CP, U8Reg::L,  U8Reg::A, 0, 1);
    dict[0xBE] = new InstructionDefinition(InstructionGroup::CP, U8Reg::HL, U8Reg::A, 0, 2);
    dict[0xBF] = new InstructionDefinition(InstructionGroup::CP, U8Reg::A,  U8Reg::A, 0, 1);
    dict[0xFE] = new InstructionDefinition(InstructionGroup::CP, U8Reg::D8, U8Reg::A, 1, 2);

    dict[0x05] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::B,  U8Reg::X, 0, 1);
    dict[0x0D] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::C,  U8Reg::X, 0, 1);
    dict[0x15] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::D,  U8Reg::X, 0, 1);
    dict[0x1D] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::E,  U8Reg::X, 0, 1);
    dict[0x25] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::H,  U8Reg::X, 0, 1);
    dict[0x2D] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::L,  U8Reg::X, 0, 1);
    dict[0x3D] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::A,  U8Reg::X, 0, 1);
    dict[0x35] = new InstructionDefinition(InstructionGroup::DEC8, U8Reg::HL, U8Reg::X, 0, 3);

    dict[0x2B] = new InstructionDefinition(InstructionGroup::DEC16, U8Reg::HL, U8Reg::X, 0, 2);
    dict[0x0B] = new InstructionDefinition(InstructionGroup::DEC16, U8Reg::BC, U8Reg::X, 0, 2);
    dict[0x1B] = new InstructionDefinition(InstructionGroup::DEC16, U8Reg::DE, U8Reg::X, 0, 2);
    dict[0x3B] = new InstructionDefinition(InstructionGroup::DEC16, U8Reg::SP, U8Reg::X, 0, 2);

    dict[0x04] = new InstructionDefinition(InstructionGroup::INC, U8Reg::B,  U8Reg::X, 0, 1);
    dict[0x0C] = new InstructionDefinition(InstructionGroup::INC, U8Reg::C,  U8Reg::X, 0, 1);
    dict[0x14] = new InstructionDefinition(InstructionGroup::INC, U8Reg::D,  U8Reg::X, 0, 1);
    dict[0x1C] = new InstructionDefinition(InstructionGroup::INC, U8Reg::E,  U8Reg::X, 0, 1);
    dict[0x24] = new InstructionDefinition(InstructionGroup::INC, U8Reg::H,  U8Reg::X, 0, 1);
    dict[0x2C] = new InstructionDefinition(InstructionGroup::INC, U8Reg::L,  U8Reg::X, 0, 1);
    dict[0x3C] = new InstructionDefinition(InstructionGroup::INC, U8Reg::A,  U8Reg::X, 0, 1);
    dict[0x34] = new InstructionDefinition(InstructionGroup::INC, U8Reg::HL, U8Reg::X, 0, 2);

    dict[0x03] = new InstructionDefinition(InstructionGroup::INC16, U8Reg::BC, U8Reg::X, 0, 2);
    dict[0x13] = new InstructionDefinition(InstructionGroup::INC16, U8Reg::DE, U8Reg::X, 0, 2);
    dict[0x23] = new InstructionDefinition(InstructionGroup::INC16, U8Reg::HL, U8Reg::X, 0, 2);
    dict[0x33] = new InstructionDefinition(InstructionGroup::INC16, U8Reg::SP, U8Reg::X, 0, 2);

    dict[0xF3] = new InstructionDefinition(InstructionGroup::DI,   U8Reg::X, U8Reg::X, 0, 1);
    dict[0xFB] = new InstructionDefinition(InstructionGroup::EI,   U8Reg::X, U8Reg::X, 0, 1);
    dict[0x76] = new InstructionDefinition(InstructionGroup::HALT, U8Reg::X, U8Reg::X, 0, 1);
    dict[0xD9] = new InstructionDefinition(InstructionGroup::RETI, U8Reg::X, U8Reg::X, 0, 4);
    dict[0x10] = new InstructionDefinition(InstructionGroup::STOP, U8Reg::X, U8Reg::X, 1, 0);
    dict[0x37] = new InstructionDefinition(InstructionGroup::SCF,  U8Reg::X, U8Reg::X, 0, 1);

    //LOAD

    dict[0x40] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::B, 0, 1);
    dict[0x41] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::C, 0, 1);
    dict[0x42] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::D, 0, 1);
    dict[0x43] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::E, 0, 1);
    dict[0x44] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::H, 0, 1);
    dict[0x45] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::L, 0, 1);
    dict[0x47] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::A, 0, 1);

    dict[0x48] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::B, 0, 1);
    dict[0x49] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::C, 0, 1);
    dict[0x4A] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::D, 0, 1);
    dict[0x4B] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::E, 0, 1);
    dict[0x4C] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::H, 0, 1);
    dict[0x4D] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::L, 0, 1);
    dict[0x4F] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::A, 0, 1);

    dict[0x50] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::B, 0, 1);
    dict[0x51] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::C, 0, 1);
    dict[0x52] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::D, 0, 1);
    dict[0x53] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::E, 0, 1);
    dict[0x54] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::H, 0, 1);
    dict[0x55] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::L, 0, 1);
    dict[0x57] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::A, 0, 1);

    dict[0x58] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::B, 0, 1);
    dict[0x59] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::C, 0, 1);
    dict[0x5A] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::D, 0, 1);
    dict[0x5B] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::E, 0, 1);
    dict[0x5C] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::H, 0, 1);
    dict[0x5D] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::L, 0, 1);
    dict[0x5F] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::A, 0, 1);

    dict[0x60] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::B, 0, 1);
    dict[0x61] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::C, 0, 1);
    dict[0x62] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::D, 0, 1);
    dict[0x63] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::E, 0, 1);
    dict[0x64] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::H, 0, 1);
    dict[0x65] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::L, 0, 1);
    dict[0x67] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::A, 0, 1);

    dict[0x68] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::B, 0, 1);
    dict[0x69] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::C, 0, 1);
    dict[0x6A] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::D, 0, 1);
    dict[0x6B] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::E, 0, 1);
    dict[0x6C] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::H, 0, 1);
    dict[0x6D] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::L, 0, 1);
    dict[0x6F] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::A, 0, 1);

    dict[0x70] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::B, 0, 2);
    dict[0x71] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::C, 0, 2);
    dict[0x72] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::D, 0, 2);
    dict[0x73] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::E, 0, 2);
    dict[0x74] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::H, 0, 2);
    dict[0x75] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::L, 0, 2);
    dict[0x77] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::A, 0, 2);

    dict[0x78] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::B, 0, 1);
    dict[0x79] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::C, 0, 1);
    dict[0x7A] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::D, 0, 1);
    dict[0x7B] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::E, 0, 1);
    dict[0x7C] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::H, 0, 1);
    dict[0x7D] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::L, 0, 1);
    dict[0x7F] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::A, 0, 1);

    dict[0x46] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::HL, 0, 2);
    dict[0x4E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::HL, 0, 2);
    dict[0x56] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::HL, 0, 2);
    dict[0x5E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::HL, 0, 2);
    dict[0x66] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::HL, 0, 2);
    dict[0x6E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::HL, 0, 2);
    dict[0x7E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::HL, 0, 2);

    dict[0x06] = new InstructionDefinition(InstructionGroup::LD, U8Reg::B,  U8Reg::D8, 1, 2);
    dict[0x0E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::C,  U8Reg::D8, 1, 2);
    dict[0x16] = new InstructionDefinition(InstructionGroup::LD, U8Reg::D,  U8Reg::D8, 1, 2);
    dict[0x1E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::E,  U8Reg::D8, 1, 2);
    dict[0x26] = new InstructionDefinition(InstructionGroup::LD, U8Reg::H,  U8Reg::D8, 1, 2);
    dict[0x2E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::L,  U8Reg::D8, 1, 2);
    dict[0x3E] = new InstructionDefinition(InstructionGroup::LD, U8Reg::A,  U8Reg::D8, 1, 2);
    dict[0x36] = new InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::D8, 1, 3);

    dict[0x02] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::BC, U8Reg::A, 0, 2);
    dict[0x12] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::DE, U8Reg::A, 0, 2);
    dict[0x0a] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::BC, 0, 2);
    dict[0x1a] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::DE, 0, 2);
    dict[0xfa] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::D16, 2, 4);
    dict[0xea] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::D16, U8Reg::A, 2, 4);
    dict[0x22] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::HLI, U8Reg::A, 0, 2);
    dict[0x32] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::HLD, U8Reg::A, 0, 2);
    dict[0x2a] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::HLI, 0, 2);
    dict[0x3a] = new InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::HLD, 0, 2);

    dict[0x01] = new InstructionDefinition(InstructionGroup::LD16, U8Reg::BC, U8Reg::D16, 2, 3);
    dict[0x11] = new InstructionDefinition(InstructionGroup::LD16, U8Reg::DE, U8Reg::D16, 2, 3);
    dict[0x21] = new InstructionDefinition(InstructionGroup::LD16, U8Reg::HL, U8Reg::D16, 2, 3);
    dict[0x31] = new InstructionDefinition(InstructionGroup::LD16, U8Reg::SP, U8Reg::D16, 2, 3);

    dict[0x08] = new InstructionDefinition(InstructionGroup::LDS, U8Reg::D16, U8Reg::SP, 2, 5);
    dict[0xf9] = new InstructionDefinition(InstructionGroup::LDS, U8Reg::SP, U8Reg::HL, 0, 1);
    dict[0xf8] = new InstructionDefinition(InstructionGroup::LDS, U8Reg::HL, U8Reg::SPE, 1, 3);

    dict[0xf0] = new InstructionDefinition(InstructionGroup::LDH, U8Reg::A, U8Reg::D8, 1, 3);
    dict[0xf2] = new InstructionDefinition(InstructionGroup::LDH, U8Reg::A, U8Reg::C, 0, 2);
    dict[0xe2] = new InstructionDefinition(InstructionGroup::LDH, U8Reg::C, U8Reg::A, 0, 2);
    dict[0xe0] = new InstructionDefinition(InstructionGroup::LDH, U8Reg::D8, U8Reg::A, 1, 3);

    ////// CORRIGIR
    dict[0xb0] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::B, 0, 1);
    dict[0xb1] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::C, 0, 1);
    dict[0xb2] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::D, 0, 1);
    dict[0xb3] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::E, 0, 1);
    dict[0xb4] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::H, 0, 1);
    dict[0xb5] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::L, 0, 1);
    dict[0xb7] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::A, 0, 1);
    dict[0xb6] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::HL, 0, 2);
    dict[0xf6] = new InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::D8, 1, 2);

    dict[0xf1] = new InstructionDefinition(InstructionGroup::POP, U8Reg::AF, U8Reg::AF, 0, 3);
    dict[0xc1] = new InstructionDefinition(InstructionGroup::POP, U8Reg::BC, U8Reg::BC, 0, 3);
    dict[0xd1] = new InstructionDefinition(InstructionGroup::POP, U8Reg::DE, U8Reg::DE, 0, 3);
    dict[0xe1] = new InstructionDefinition(InstructionGroup::POP, U8Reg::HL, U8Reg::HL, 0, 3);

    dict[0xf5] = new InstructionDefinition(InstructionGroup::PUSH, U8Reg::AF, U8Reg::AF, 0, 4);
    dict[0xc5] = new InstructionDefinition(InstructionGroup::PUSH, U8Reg::BC, U8Reg::BC, 0, 4);
    dict[0xd5] = new InstructionDefinition(InstructionGroup::PUSH, U8Reg::DE, U8Reg::DE, 0, 4);
    dict[0xe5] = new InstructionDefinition(InstructionGroup::PUSH, U8Reg::HL, U8Reg::HL, 0, 4);

    ////// CORRIGIR
    dict[0x90] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::B, 0, 1);
    dict[0x91] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::C, 0, 1);
    dict[0x92] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::D, 0, 1);
    dict[0x93] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::E, 0, 1);
    dict[0x94] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::H, 0, 1);
    dict[0x95] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::L, 0, 1);
    dict[0x96] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::HL, 0, 2);
    dict[0x97] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::A, 0, 1);
    dict[0xd6] = new InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::D8, 1, 2);


    ////// CORRIGIR
    dict[0x98] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::B, 0, 1);
    dict[0x99] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::C, 0, 1);
    dict[0x9a] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::D, 0, 1);
    dict[0x9b] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::E, 0, 1);
    dict[0x9c] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::H, 0, 1);
    dict[0x9d] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::L, 0, 1);
    dict[0x9e] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::HL, 0, 2);
    dict[0x9f] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::A, 0, 1);
    dict[0xde] = new InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::D8, 1, 2);

    ////// CORRIGIR
    dict[0xa8] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::B, 0, 1);
    dict[0xa9] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::C, 0, 1);
    dict[0xaa] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::D, 0, 1);
    dict[0xab] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::E, 0, 1);
    dict[0xac] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::H, 0, 1);
    dict[0xad] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::L, 0, 1);
    dict[0xae] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::HL, 0, 2);
    dict[0xaf] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::A, 0, 1);
    dict[0xee] = new InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::D8, 1, 2);

    dict[0x07] = new InstructionDefinition(InstructionGroup::RLCA, U8Reg::A, U8Reg::A, 0, 1);
    dict[0x0f] = new InstructionDefinition(InstructionGroup::RRCA, U8Reg::A, U8Reg::A, 0, 1);
    dict[0x17] = new InstructionDefinition(InstructionGroup::RLA, U8Reg::A, U8Reg::A, 0, 1);
    dict[0x1f] = new InstructionDefinition(InstructionGroup::RRA, U8Reg::A, U8Reg::A, 0, 1);

    dict[0xc7] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V0, U8Reg::X, 0, 4);
    dict[0xd7] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V1, U8Reg::X, 0, 4);
    dict[0xe7] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V2, U8Reg::X, 0, 4);
    dict[0xf7] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V3, U8Reg::X, 0, 4);
    dict[0xcf] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V4, U8Reg::X, 0, 4);
    dict[0xdf] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V5, U8Reg::X, 0, 4);
    dict[0xef] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V6, U8Reg::X, 0, 4);
    dict[0xff] = new InstructionDefinition(InstructionGroup::RST, U8Reg::V7, U8Reg::X, 0, 4);


    //PREFIX INSTRUCITONS
    dictPrefix[0x10] = new InstructionDefinition(InstructionGroup::RL, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x11] = new InstructionDefinition(InstructionGroup::RL, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x12] = new InstructionDefinition(InstructionGroup::RL, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x13] = new InstructionDefinition(InstructionGroup::RL, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x14] = new InstructionDefinition(InstructionGroup::RL, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x15] = new InstructionDefinition(InstructionGroup::RL, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x16] = new InstructionDefinition(InstructionGroup::RL, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x17] = new InstructionDefinition(InstructionGroup::RL, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x00] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x01] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x02] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x03] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x04] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x05] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x06] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x07] = new InstructionDefinition(InstructionGroup::RLC, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x18] = new InstructionDefinition(InstructionGroup::RR, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x19] = new InstructionDefinition(InstructionGroup::RR, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x1a] = new InstructionDefinition(InstructionGroup::RR, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x1b] = new InstructionDefinition(InstructionGroup::RR, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x1c] = new InstructionDefinition(InstructionGroup::RR, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x1d] = new InstructionDefinition(InstructionGroup::RR, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x1e] = new InstructionDefinition(InstructionGroup::RR, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x1f] = new InstructionDefinition(InstructionGroup::RR, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x08] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x09] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x0a] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x0b] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x0c] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x0d] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x0e] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x0f] = new InstructionDefinition(InstructionGroup::RRC, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x40] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x41] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x42] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x43] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x44] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x45] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x46] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT0, 0, 3);
    dictPrefix[0x47] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    dictPrefix[0x48] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x49] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x4a] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x4b] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x4c] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x4d] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x4e] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT1, 0, 3);
    dictPrefix[0x4f] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    dictPrefix[0x50] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x51] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x52] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x53] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x54] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x55] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x56] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT2, 0, 3);
    dictPrefix[0x57] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    dictPrefix[0x58] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x59] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x5a] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x5b] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x5c] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x5d] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x5e] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT3, 0, 3);
    dictPrefix[0x5f] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    dictPrefix[0x60] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x61] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x62] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x63] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x64] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x65] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0x66] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT4, 0, 3);
    dictPrefix[0x67] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    dictPrefix[0x68] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x69] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x6a] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x6b] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x6c] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x6d] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0x6e] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT5, 0, 3);
    dictPrefix[0x6f] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    dictPrefix[0x70] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x71] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x72] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x73] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x74] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x75] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0x76] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT6, 0, 3);
    dictPrefix[0x77] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    dictPrefix[0x78] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x79] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x7a] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x7b] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x7c] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x7d] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0x7e] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT7, 0, 3);
    dictPrefix[0x7f] = new InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    dictPrefix[0x38] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x39] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x3a] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x3b] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x3c] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x3d] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x3e] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x3f] = new InstructionDefinition(InstructionGroup::SRL, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x30] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x31] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x32] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x33] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x34] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x35] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x36] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x37] = new InstructionDefinition(InstructionGroup::SWAP, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x80] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x81] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x82] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x83] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x84] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x85] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0x86] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT0, 0, 4);
    dictPrefix[0x87] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    dictPrefix[0x88] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x89] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x8a] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x8b] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x8c] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x8d] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0x8e] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT1, 0, 4);
    dictPrefix[0x8f] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    dictPrefix[0x90] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x91] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x92] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x93] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x94] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x95] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0x96] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT2, 0, 4);
    dictPrefix[0x97] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    dictPrefix[0x98] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x99] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x9a] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x9b] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x9c] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x9d] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0x9e] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT3, 0, 4);
    dictPrefix[0x9f] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    dictPrefix[0xa0] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa1] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa2] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa3] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa4] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa5] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xa6] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT4, 0, 4);
    dictPrefix[0xa7] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    dictPrefix[0xa8] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xa9] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xaa] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xab] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xac] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xad] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xae] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT5, 0, 4);
    dictPrefix[0xaf] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    dictPrefix[0xb0] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb1] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb2] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb3] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb4] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb5] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xb6] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT6, 0, 4);
    dictPrefix[0xb7] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    dictPrefix[0xb8] = new InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xb9] = new InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xba] = new InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xbb] = new InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xbc] = new InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xbd] = new InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xbe] = new InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT7, 0, 4);
    dictPrefix[0xbf] = new InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    dictPrefix[0xc0] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc1] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc2] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc3] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc4] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc5] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    dictPrefix[0xc6] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT0, 0, 4);
    dictPrefix[0xc7] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    dictPrefix[0xc8] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xc9] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xca] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xcb] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xcc] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xcd] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    dictPrefix[0xce] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT1, 0, 4);
    dictPrefix[0xcf] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    dictPrefix[0xd0] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd1] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd2] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd3] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd4] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd5] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    dictPrefix[0xd6] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT2, 0, 4);
    dictPrefix[0xd7] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    dictPrefix[0xd8] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xd9] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xda] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xdb] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xdc] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xdd] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    dictPrefix[0xde] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT3, 0, 4);
    dictPrefix[0xdf] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    dictPrefix[0xe0] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe1] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe2] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe3] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe4] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe5] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    dictPrefix[0xe6] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT4, 0, 4);
    dictPrefix[0xe7] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    dictPrefix[0xe8] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xe9] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xea] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xeb] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xec] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xed] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    dictPrefix[0xee] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT5, 0, 4);
    dictPrefix[0xef] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    dictPrefix[0xf0] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf1] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf2] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf3] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf4] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf5] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    dictPrefix[0xf6] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT6, 0, 4);
    dictPrefix[0xf7] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    dictPrefix[0xf8] = new InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xf9] = new InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xfa] = new InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xfb] = new InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xfc] = new InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xfd] = new InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    dictPrefix[0xfe] = new InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT7, 0, 4);
    dictPrefix[0xff] = new InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    dictPrefix[0x20] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x21] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x22] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x23] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x24] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x25] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x26] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x27] = new InstructionDefinition(InstructionGroup::SLA, U8Reg::A, U8Reg::A, 0, 2);

    dictPrefix[0x28] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::B, U8Reg::B, 0, 2);
    dictPrefix[0x29] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::C, U8Reg::C, 0, 2);
    dictPrefix[0x2a] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::D, U8Reg::D, 0, 2);
    dictPrefix[0x2b] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::E, U8Reg::E, 0, 2);
    dictPrefix[0x2c] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::H, U8Reg::H, 0, 2);
    dictPrefix[0x2d] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::L, U8Reg::L, 0, 2);
    dictPrefix[0x2e] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::HL, U8Reg::HL, 0, 4);
    dictPrefix[0x2f] = new InstructionDefinition(InstructionGroup::SRA, U8Reg::A, U8Reg::A, 0, 2);
}