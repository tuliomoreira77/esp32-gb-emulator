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
    new(&dict[0x00]) InstructionDefinition(InstructionGroup::NOP, U8Reg::X, U8Reg::X, 0, 1);

    new(&dict[0x80]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::B,  U8Reg::A, 0, 1);
    new(&dict[0x81]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::C,  U8Reg::A, 0, 1);
    new(&dict[0x82]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::D,  U8Reg::A, 0, 1);
    new(&dict[0x83]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::E,  U8Reg::A, 0, 1);
    new(&dict[0x84]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::H,  U8Reg::A, 0, 1);
    new(&dict[0x85]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::L,  U8Reg::A, 0, 1);
    new(&dict[0x86]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::HL, U8Reg::A, 0, 2);
    new(&dict[0x87]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::A,  U8Reg::A, 0, 1);
    new(&dict[0xC6]) InstructionDefinition(InstructionGroup::ADD,  U8Reg::D8, U8Reg::A, 1, 2);

    new(&dict[0x88]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::B,  U8Reg::A, 0, 1);
    new(&dict[0x89]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::C,  U8Reg::A, 0, 1);
    new(&dict[0x8A]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::D,  U8Reg::A, 0, 1);
    new(&dict[0x8B]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::E,  U8Reg::A, 0, 1);
    new(&dict[0x8C]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::H,  U8Reg::A, 0, 1);
    new(&dict[0x8D]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::L,  U8Reg::A, 0, 1);
    new(&dict[0x8E]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::HL, U8Reg::A, 0, 2);
    new(&dict[0x8F]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::A,  U8Reg::A, 0, 1);
    new(&dict[0xCE]) InstructionDefinition(InstructionGroup::ADDC, U8Reg::D8, U8Reg::A, 1, 2);

    new(&dict[0xA0]) InstructionDefinition(InstructionGroup::AND, U8Reg::B,  U8Reg::A, 0, 1);
    new(&dict[0xA1]) InstructionDefinition(InstructionGroup::AND, U8Reg::C,  U8Reg::A, 0, 1);
    new(&dict[0xA2]) InstructionDefinition(InstructionGroup::AND, U8Reg::D,  U8Reg::A, 0, 1);
    new(&dict[0xA3]) InstructionDefinition(InstructionGroup::AND, U8Reg::E,  U8Reg::A, 0, 1);
    new(&dict[0xA4]) InstructionDefinition(InstructionGroup::AND, U8Reg::H,  U8Reg::A, 0, 1);
    new(&dict[0xA5]) InstructionDefinition(InstructionGroup::AND, U8Reg::L,  U8Reg::A, 0, 1);
    new(&dict[0xA6]) InstructionDefinition(InstructionGroup::AND, U8Reg::HL, U8Reg::A, 0, 1);
    new(&dict[0xA7]) InstructionDefinition(InstructionGroup::AND, U8Reg::A,  U8Reg::A, 0, 1);
    new(&dict[0xE6]) InstructionDefinition(InstructionGroup::AND, U8Reg::D8, U8Reg::A, 1, 2);

    new(&dict[0xE9]) InstructionDefinition(InstructionGroup::JP, U8Reg::HL, U8Reg::X, 0, 1);
    new(&dict[0xC3]) InstructionDefinition(InstructionGroup::JP, U8Reg::D16, U8Reg::X, 2, 4);
    new(&dict[0xC2]) InstructionDefinition(InstructionGroup::JP, U8Reg::NZ, U8Reg::X, 2, 3);
    new(&dict[0xD2]) InstructionDefinition(InstructionGroup::JP, U8Reg::NC, U8Reg::X, 2, 3);
    new(&dict[0xCA]) InstructionDefinition(InstructionGroup::JP, U8Reg::Z, U8Reg::X, 2, 3);
    new(&dict[0xDA]) InstructionDefinition(InstructionGroup::JP, U8Reg::CR, U8Reg::X, 2, 3);

    new(&dict[0x18]) InstructionDefinition(InstructionGroup::JPR, U8Reg::D16, U8Reg::X, 1, 2);
    new(&dict[0x20]) InstructionDefinition(InstructionGroup::JPR, U8Reg::NZ, U8Reg::X, 1, 2);
    new(&dict[0x30]) InstructionDefinition(InstructionGroup::JPR, U8Reg::NC, U8Reg::X, 1, 2);
    new(&dict[0x28]) InstructionDefinition(InstructionGroup::JPR, U8Reg::Z, U8Reg::X, 1, 2);
    new(&dict[0x38]) InstructionDefinition(InstructionGroup::JPR, U8Reg::CR, U8Reg::X, 1, 2);

    new(&dict[0xCD]) InstructionDefinition(InstructionGroup::CALL, U8Reg::D16, U8Reg::X, 2, 6);
    new(&dict[0xC4]) InstructionDefinition(InstructionGroup::CALL, U8Reg::NZ, U8Reg::X, 2, 3);
    new(&dict[0xD4]) InstructionDefinition(InstructionGroup::CALL, U8Reg::NC, U8Reg::X, 2, 3);
    new(&dict[0xCC]) InstructionDefinition(InstructionGroup::CALL, U8Reg::Z, U8Reg::X, 2, 3);
    new(&dict[0xDC]) InstructionDefinition(InstructionGroup::CALL, U8Reg::CR, U8Reg::X, 2, 3);

    new(&dict[0xC9]) InstructionDefinition(InstructionGroup::RET, U8Reg::D16, U8Reg::X, 0, 4);
    new(&dict[0xC0]) InstructionDefinition(InstructionGroup::RET, U8Reg::NZ, U8Reg::X, 0, 2);
    new(&dict[0xD0]) InstructionDefinition(InstructionGroup::RET, U8Reg::NC, U8Reg::X, 0, 2);
    new(&dict[0xC8]) InstructionDefinition(InstructionGroup::RET, U8Reg::Z, U8Reg::X, 0, 2);
    new(&dict[0xD8]) InstructionDefinition(InstructionGroup::RET, U8Reg::CR, U8Reg::X, 0, 2);

    new(&dict[0x09]) InstructionDefinition(InstructionGroup::ADDHL, U8Reg::BC, U8Reg::X, 0, 2);
    new(&dict[0x19]) InstructionDefinition(InstructionGroup::ADDHL, U8Reg::DE, U8Reg::X, 0, 2);
    new(&dict[0x29]) InstructionDefinition(InstructionGroup::ADDHL, U8Reg::HL, U8Reg::X, 0, 2);
    new(&dict[0x39]) InstructionDefinition(InstructionGroup::ADDHL, U8Reg::SP, U8Reg::X, 0, 2);

    new(&dict[0xE8]) InstructionDefinition(InstructionGroup::ADDSP, U8Reg::D8, U8Reg::X, 1, 2);

    new(&dict[0x3F]) InstructionDefinition(InstructionGroup::CCF, U8Reg::X, U8Reg::X, 0, 1);
    new(&dict[0x2F]) InstructionDefinition(InstructionGroup::CPL, U8Reg::X, U8Reg::X, 0, 1);
    new(&dict[0x27]) InstructionDefinition(InstructionGroup::DAA, U8Reg::X, U8Reg::X, 0, 1);

    new(&dict[0xB8]) InstructionDefinition(InstructionGroup::CP, U8Reg::B,  U8Reg::A, 0, 1);
    new(&dict[0xB9]) InstructionDefinition(InstructionGroup::CP, U8Reg::C,  U8Reg::A, 0, 1);
    new(&dict[0xBA]) InstructionDefinition(InstructionGroup::CP, U8Reg::D,  U8Reg::A, 0, 1);
    new(&dict[0xBB]) InstructionDefinition(InstructionGroup::CP, U8Reg::E,  U8Reg::A, 0, 1);
    new(&dict[0xBC]) InstructionDefinition(InstructionGroup::CP, U8Reg::H,  U8Reg::A, 0, 1);
    new(&dict[0xBD]) InstructionDefinition(InstructionGroup::CP, U8Reg::L,  U8Reg::A, 0, 1);
    new(&dict[0xBE]) InstructionDefinition(InstructionGroup::CP, U8Reg::HL, U8Reg::A, 0, 2);
    new(&dict[0xBF]) InstructionDefinition(InstructionGroup::CP, U8Reg::A,  U8Reg::A, 0, 1);
    new(&dict[0xFE]) InstructionDefinition(InstructionGroup::CP, U8Reg::D8, U8Reg::A, 1, 2);

    new(&dict[0x05]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::B,  U8Reg::X, 0, 1);
    new(&dict[0x0D]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::C,  U8Reg::X, 0, 1);
    new(&dict[0x15]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::D,  U8Reg::X, 0, 1);
    new(&dict[0x1D]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::E,  U8Reg::X, 0, 1);
    new(&dict[0x25]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::H,  U8Reg::X, 0, 1);
    new(&dict[0x2D]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::L,  U8Reg::X, 0, 1);
    new(&dict[0x3D]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::A,  U8Reg::X, 0, 1);
    new(&dict[0x35]) InstructionDefinition(InstructionGroup::DEC8, U8Reg::HL, U8Reg::X, 0, 3);

    new(&dict[0x2B]) InstructionDefinition(InstructionGroup::DEC16, U8Reg::HL, U8Reg::X, 0, 2);
    new(&dict[0x0B]) InstructionDefinition(InstructionGroup::DEC16, U8Reg::BC, U8Reg::X, 0, 2);
    new(&dict[0x1B]) InstructionDefinition(InstructionGroup::DEC16, U8Reg::DE, U8Reg::X, 0, 2);
    new(&dict[0x3B]) InstructionDefinition(InstructionGroup::DEC16, U8Reg::SP, U8Reg::X, 0, 2);

    new(&dict[0x04]) InstructionDefinition(InstructionGroup::INC, U8Reg::B,  U8Reg::X, 0, 1);
    new(&dict[0x0C]) InstructionDefinition(InstructionGroup::INC, U8Reg::C,  U8Reg::X, 0, 1);
    new(&dict[0x14]) InstructionDefinition(InstructionGroup::INC, U8Reg::D,  U8Reg::X, 0, 1);
    new(&dict[0x1C]) InstructionDefinition(InstructionGroup::INC, U8Reg::E,  U8Reg::X, 0, 1);
    new(&dict[0x24]) InstructionDefinition(InstructionGroup::INC, U8Reg::H,  U8Reg::X, 0, 1);
    new(&dict[0x2C]) InstructionDefinition(InstructionGroup::INC, U8Reg::L,  U8Reg::X, 0, 1);
    new(&dict[0x3C]) InstructionDefinition(InstructionGroup::INC, U8Reg::A,  U8Reg::X, 0, 1);
    new(&dict[0x34]) InstructionDefinition(InstructionGroup::INC, U8Reg::HL, U8Reg::X, 0, 2);

    new(&dict[0x03]) InstructionDefinition(InstructionGroup::INC16, U8Reg::BC, U8Reg::X, 0, 2);
    new(&dict[0x13]) InstructionDefinition(InstructionGroup::INC16, U8Reg::DE, U8Reg::X, 0, 2);
    new(&dict[0x23]) InstructionDefinition(InstructionGroup::INC16, U8Reg::HL, U8Reg::X, 0, 2);
    new(&dict[0x33]) InstructionDefinition(InstructionGroup::INC16, U8Reg::SP, U8Reg::X, 0, 2);

    new(&dict[0xF3]) InstructionDefinition(InstructionGroup::DI,   U8Reg::X, U8Reg::X, 0, 1);
    new(&dict[0xFB]) InstructionDefinition(InstructionGroup::EI,   U8Reg::X, U8Reg::X, 0, 1);
    new(&dict[0x76]) InstructionDefinition(InstructionGroup::HALT, U8Reg::X, U8Reg::X, 0, 1);
    new(&dict[0xD9]) InstructionDefinition(InstructionGroup::RETI, U8Reg::X, U8Reg::X, 0, 4);
    new(&dict[0x10]) InstructionDefinition(InstructionGroup::STOP, U8Reg::X, U8Reg::X, 1, 0);
    new(&dict[0x37]) InstructionDefinition(InstructionGroup::SCF,  U8Reg::X, U8Reg::X, 0, 1);

    //LOAD

    new(&dict[0x40]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::B, 0, 1);
    new(&dict[0x41]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::C, 0, 1);
    new(&dict[0x42]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::D, 0, 1);
    new(&dict[0x43]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::E, 0, 1);
    new(&dict[0x44]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::H, 0, 1);
    new(&dict[0x45]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::L, 0, 1);
    new(&dict[0x47]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::A, 0, 1);

    new(&dict[0x48]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::B, 0, 1);
    new(&dict[0x49]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::C, 0, 1);
    new(&dict[0x4A]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::D, 0, 1);
    new(&dict[0x4B]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::E, 0, 1);
    new(&dict[0x4C]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::H, 0, 1);
    new(&dict[0x4D]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::L, 0, 1);
    new(&dict[0x4F]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::A, 0, 1);

    new(&dict[0x50]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::B, 0, 1);
    new(&dict[0x51]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::C, 0, 1);
    new(&dict[0x52]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::D, 0, 1);
    new(&dict[0x53]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::E, 0, 1);
    new(&dict[0x54]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::H, 0, 1);
    new(&dict[0x55]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::L, 0, 1);
    new(&dict[0x57]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::A, 0, 1);

    new(&dict[0x58]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::B, 0, 1);
    new(&dict[0x59]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::C, 0, 1);
    new(&dict[0x5A]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::D, 0, 1);
    new(&dict[0x5B]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::E, 0, 1);
    new(&dict[0x5C]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::H, 0, 1);
    new(&dict[0x5D]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::L, 0, 1);
    new(&dict[0x5F]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::A, 0, 1);

    new(&dict[0x60]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::B, 0, 1);
    new(&dict[0x61]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::C, 0, 1);
    new(&dict[0x62]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::D, 0, 1);
    new(&dict[0x63]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::E, 0, 1);
    new(&dict[0x64]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::H, 0, 1);
    new(&dict[0x65]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::L, 0, 1);
    new(&dict[0x67]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::A, 0, 1);

    new(&dict[0x68]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::B, 0, 1);
    new(&dict[0x69]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::C, 0, 1);
    new(&dict[0x6A]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::D, 0, 1);
    new(&dict[0x6B]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::E, 0, 1);
    new(&dict[0x6C]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::H, 0, 1);
    new(&dict[0x6D]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::L, 0, 1);
    new(&dict[0x6F]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::A, 0, 1);

    new(&dict[0x70]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::B, 0, 2);
    new(&dict[0x71]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::C, 0, 2);
    new(&dict[0x72]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::D, 0, 2);
    new(&dict[0x73]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::E, 0, 2);
    new(&dict[0x74]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::H, 0, 2);
    new(&dict[0x75]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::L, 0, 2);
    new(&dict[0x77]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::A, 0, 2);

    new(&dict[0x78]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::B, 0, 1);
    new(&dict[0x79]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::C, 0, 1);
    new(&dict[0x7A]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::D, 0, 1);
    new(&dict[0x7B]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::E, 0, 1);
    new(&dict[0x7C]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::H, 0, 1);
    new(&dict[0x7D]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::L, 0, 1);
    new(&dict[0x7F]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::A, 0, 1);

    new(&dict[0x46]) InstructionDefinition(InstructionGroup::LD, U8Reg::B, U8Reg::HL, 0, 2);
    new(&dict[0x4E]) InstructionDefinition(InstructionGroup::LD, U8Reg::C, U8Reg::HL, 0, 2);
    new(&dict[0x56]) InstructionDefinition(InstructionGroup::LD, U8Reg::D, U8Reg::HL, 0, 2);
    new(&dict[0x5E]) InstructionDefinition(InstructionGroup::LD, U8Reg::E, U8Reg::HL, 0, 2);
    new(&dict[0x66]) InstructionDefinition(InstructionGroup::LD, U8Reg::H, U8Reg::HL, 0, 2);
    new(&dict[0x6E]) InstructionDefinition(InstructionGroup::LD, U8Reg::L, U8Reg::HL, 0, 2);
    new(&dict[0x7E]) InstructionDefinition(InstructionGroup::LD, U8Reg::A, U8Reg::HL, 0, 2);

    new(&dict[0x06]) InstructionDefinition(InstructionGroup::LD, U8Reg::B,  U8Reg::D8, 1, 2);
    new(&dict[0x0E]) InstructionDefinition(InstructionGroup::LD, U8Reg::C,  U8Reg::D8, 1, 2);
    new(&dict[0x16]) InstructionDefinition(InstructionGroup::LD, U8Reg::D,  U8Reg::D8, 1, 2);
    new(&dict[0x1E]) InstructionDefinition(InstructionGroup::LD, U8Reg::E,  U8Reg::D8, 1, 2);
    new(&dict[0x26]) InstructionDefinition(InstructionGroup::LD, U8Reg::H,  U8Reg::D8, 1, 2);
    new(&dict[0x2E]) InstructionDefinition(InstructionGroup::LD, U8Reg::L,  U8Reg::D8, 1, 2);
    new(&dict[0x3E]) InstructionDefinition(InstructionGroup::LD, U8Reg::A,  U8Reg::D8, 1, 2);
    new(&dict[0x36]) InstructionDefinition(InstructionGroup::LD, U8Reg::HL, U8Reg::D8, 1, 3);

    new(&dict[0x02]) InstructionDefinition(InstructionGroup::LDP, U8Reg::BC, U8Reg::A, 0, 2);
    new(&dict[0x12]) InstructionDefinition(InstructionGroup::LDP, U8Reg::DE, U8Reg::A, 0, 2);
    new(&dict[0x0a]) InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::BC, 0, 2);
    new(&dict[0x1a]) InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::DE, 0, 2);
    new(&dict[0xfa]) InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::D16, 2, 4);
    new(&dict[0xea]) InstructionDefinition(InstructionGroup::LDP, U8Reg::D16, U8Reg::A, 2, 4);
    new(&dict[0x22]) InstructionDefinition(InstructionGroup::LDP, U8Reg::HLI, U8Reg::A, 0, 2);
    new(&dict[0x32]) InstructionDefinition(InstructionGroup::LDP, U8Reg::HLD, U8Reg::A, 0, 2);
    new(&dict[0x2a]) InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::HLI, 0, 2);
    new(&dict[0x3a]) InstructionDefinition(InstructionGroup::LDP, U8Reg::A, U8Reg::HLD, 0, 2);

    new(&dict[0x01]) InstructionDefinition(InstructionGroup::LD16, U8Reg::BC, U8Reg::D16, 2, 3);
    new(&dict[0x11]) InstructionDefinition(InstructionGroup::LD16, U8Reg::DE, U8Reg::D16, 2, 3);
    new(&dict[0x21]) InstructionDefinition(InstructionGroup::LD16, U8Reg::HL, U8Reg::D16, 2, 3);
    new(&dict[0x31]) InstructionDefinition(InstructionGroup::LD16, U8Reg::SP, U8Reg::D16, 2, 3);

    new(&dict[0x08]) InstructionDefinition(InstructionGroup::LDS, U8Reg::D16, U8Reg::SP, 2, 5);
    new(&dict[0xf9]) InstructionDefinition(InstructionGroup::LDS, U8Reg::SP, U8Reg::HL, 0, 1);
    new(&dict[0xf8]) InstructionDefinition(InstructionGroup::LDS, U8Reg::HL, U8Reg::SPE, 1, 3);

    new(&dict[0xf0]) InstructionDefinition(InstructionGroup::LDH, U8Reg::A, U8Reg::D8, 1, 3);
    new(&dict[0xf2]) InstructionDefinition(InstructionGroup::LDH, U8Reg::A, U8Reg::C, 0, 2);
    new(&dict[0xe2]) InstructionDefinition(InstructionGroup::LDH, U8Reg::C, U8Reg::A, 0, 2);
    new(&dict[0xe0]) InstructionDefinition(InstructionGroup::LDH, U8Reg::D8, U8Reg::A, 1, 3);

    ////// CORRIGIR
    new(&dict[0xb0]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::B, 0, 1);
    new(&dict[0xb1]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::C, 0, 1);
    new(&dict[0xb2]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::D, 0, 1);
    new(&dict[0xb3]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::E, 0, 1);
    new(&dict[0xb4]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::H, 0, 1);
    new(&dict[0xb5]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::L, 0, 1);
    new(&dict[0xb7]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0xb6]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::HL, 0, 2);
    new(&dict[0xf6]) InstructionDefinition(InstructionGroup::OR, U8Reg::A, U8Reg::D8, 1, 2);

    new(&dict[0xf1]) InstructionDefinition(InstructionGroup::POP, U8Reg::AF, U8Reg::AF, 0, 3);
    new(&dict[0xc1]) InstructionDefinition(InstructionGroup::POP, U8Reg::BC, U8Reg::BC, 0, 3);
    new(&dict[0xd1]) InstructionDefinition(InstructionGroup::POP, U8Reg::DE, U8Reg::DE, 0, 3);
    new(&dict[0xe1]) InstructionDefinition(InstructionGroup::POP, U8Reg::HL, U8Reg::HL, 0, 3);

    new(&dict[0xf5]) InstructionDefinition(InstructionGroup::PUSH, U8Reg::AF, U8Reg::AF, 0, 4);
    new(&dict[0xc5]) InstructionDefinition(InstructionGroup::PUSH, U8Reg::BC, U8Reg::BC, 0, 4);
    new(&dict[0xd5]) InstructionDefinition(InstructionGroup::PUSH, U8Reg::DE, U8Reg::DE, 0, 4);
    new(&dict[0xe5]) InstructionDefinition(InstructionGroup::PUSH, U8Reg::HL, U8Reg::HL, 0, 4);

    ////// CORRIGIR
    new(&dict[0x90]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::B, 0, 1);
    new(&dict[0x91]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::C, 0, 1);
    new(&dict[0x92]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::D, 0, 1);
    new(&dict[0x93]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::E, 0, 1);
    new(&dict[0x94]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::H, 0, 1);
    new(&dict[0x95]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::L, 0, 1);
    new(&dict[0x96]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::HL, 0, 2);
    new(&dict[0x97]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0xd6]) InstructionDefinition(InstructionGroup::SUB, U8Reg::A, U8Reg::D8, 1, 2);


    ////// CORRIGIR
    new(&dict[0x98]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::B, 0, 1);
    new(&dict[0x99]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::C, 0, 1);
    new(&dict[0x9a]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::D, 0, 1);
    new(&dict[0x9b]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::E, 0, 1);
    new(&dict[0x9c]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::H, 0, 1);
    new(&dict[0x9d]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::L, 0, 1);
    new(&dict[0x9e]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::HL, 0, 2);
    new(&dict[0x9f]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0xde]) InstructionDefinition(InstructionGroup::SUBC, U8Reg::A, U8Reg::D8, 1, 2);

    ////// CORRIGIR
    new(&dict[0xa8]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::B, 0, 1);
    new(&dict[0xa9]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::C, 0, 1);
    new(&dict[0xaa]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::D, 0, 1);
    new(&dict[0xab]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::E, 0, 1);
    new(&dict[0xac]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::H, 0, 1);
    new(&dict[0xad]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::L, 0, 1);
    new(&dict[0xae]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::HL, 0, 2);
    new(&dict[0xaf]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0xee]) InstructionDefinition(InstructionGroup::XOR, U8Reg::A, U8Reg::D8, 1, 2);

    new(&dict[0x07]) InstructionDefinition(InstructionGroup::RLCA, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0x0f]) InstructionDefinition(InstructionGroup::RRCA, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0x17]) InstructionDefinition(InstructionGroup::RLA, U8Reg::A, U8Reg::A, 0, 1);
    new(&dict[0x1f]) InstructionDefinition(InstructionGroup::RRA, U8Reg::A, U8Reg::A, 0, 1);

    new(&dict[0xc7]) InstructionDefinition(InstructionGroup::RST, U8Reg::V0, U8Reg::X, 0, 4);
    new(&dict[0xd7]) InstructionDefinition(InstructionGroup::RST, U8Reg::V1, U8Reg::X, 0, 4);
    new(&dict[0xe7]) InstructionDefinition(InstructionGroup::RST, U8Reg::V2, U8Reg::X, 0, 4);
    new(&dict[0xf7]) InstructionDefinition(InstructionGroup::RST, U8Reg::V3, U8Reg::X, 0, 4);
    new(&dict[0xcf]) InstructionDefinition(InstructionGroup::RST, U8Reg::V4, U8Reg::X, 0, 4);
    new(&dict[0xdf]) InstructionDefinition(InstructionGroup::RST, U8Reg::V5, U8Reg::X, 0, 4);
    new(&dict[0xef]) InstructionDefinition(InstructionGroup::RST, U8Reg::V6, U8Reg::X, 0, 4);
    new(&dict[0xff]) InstructionDefinition(InstructionGroup::RST, U8Reg::V7, U8Reg::X, 0, 4);


    //PREFIX INSTRUCITONS
    new(&dictPrefix[0x10]) InstructionDefinition(InstructionGroup::RL, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x11]) InstructionDefinition(InstructionGroup::RL, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x12]) InstructionDefinition(InstructionGroup::RL, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x13]) InstructionDefinition(InstructionGroup::RL, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x14]) InstructionDefinition(InstructionGroup::RL, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x15]) InstructionDefinition(InstructionGroup::RL, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x16]) InstructionDefinition(InstructionGroup::RL, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x17]) InstructionDefinition(InstructionGroup::RL, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x00]) InstructionDefinition(InstructionGroup::RLC, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x01]) InstructionDefinition(InstructionGroup::RLC, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x02]) InstructionDefinition(InstructionGroup::RLC, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x03]) InstructionDefinition(InstructionGroup::RLC, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x04]) InstructionDefinition(InstructionGroup::RLC, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x05]) InstructionDefinition(InstructionGroup::RLC, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x06]) InstructionDefinition(InstructionGroup::RLC, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x07]) InstructionDefinition(InstructionGroup::RLC, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x18]) InstructionDefinition(InstructionGroup::RR, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x19]) InstructionDefinition(InstructionGroup::RR, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x1a]) InstructionDefinition(InstructionGroup::RR, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x1b]) InstructionDefinition(InstructionGroup::RR, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x1c]) InstructionDefinition(InstructionGroup::RR, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x1d]) InstructionDefinition(InstructionGroup::RR, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x1e]) InstructionDefinition(InstructionGroup::RR, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x1f]) InstructionDefinition(InstructionGroup::RR, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x08]) InstructionDefinition(InstructionGroup::RRC, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x09]) InstructionDefinition(InstructionGroup::RRC, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x0a]) InstructionDefinition(InstructionGroup::RRC, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x0b]) InstructionDefinition(InstructionGroup::RRC, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x0c]) InstructionDefinition(InstructionGroup::RRC, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x0d]) InstructionDefinition(InstructionGroup::RRC, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x0e]) InstructionDefinition(InstructionGroup::RRC, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x0f]) InstructionDefinition(InstructionGroup::RRC, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x40]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x41]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x42]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x43]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x44]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x45]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x46]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT0, 0, 3);
    new(&dictPrefix[0x47]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    new(&dictPrefix[0x48]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x49]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x4a]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x4b]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x4c]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x4d]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x4e]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT1, 0, 3);
    new(&dictPrefix[0x4f]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    new(&dictPrefix[0x50]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x51]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x52]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x53]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x54]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x55]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x56]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT2, 0, 3);
    new(&dictPrefix[0x57]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    new(&dictPrefix[0x58]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x59]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x5a]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x5b]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x5c]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x5d]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x5e]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT3, 0, 3);
    new(&dictPrefix[0x5f]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    new(&dictPrefix[0x60]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x61]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x62]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x63]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x64]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x65]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0x66]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT4, 0, 3);
    new(&dictPrefix[0x67]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    new(&dictPrefix[0x68]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x69]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x6a]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x6b]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x6c]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x6d]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0x6e]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT5, 0, 3);
    new(&dictPrefix[0x6f]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    new(&dictPrefix[0x70]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x71]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x72]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x73]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x74]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x75]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0x76]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT6, 0, 3);
    new(&dictPrefix[0x77]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    new(&dictPrefix[0x78]) InstructionDefinition(InstructionGroup::BIT, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x79]) InstructionDefinition(InstructionGroup::BIT, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x7a]) InstructionDefinition(InstructionGroup::BIT, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x7b]) InstructionDefinition(InstructionGroup::BIT, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x7c]) InstructionDefinition(InstructionGroup::BIT, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x7d]) InstructionDefinition(InstructionGroup::BIT, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0x7e]) InstructionDefinition(InstructionGroup::BIT, U8Reg::HL, U8Reg::C_BIT7, 0, 3);
    new(&dictPrefix[0x7f]) InstructionDefinition(InstructionGroup::BIT, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    new(&dictPrefix[0x38]) InstructionDefinition(InstructionGroup::SRL, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x39]) InstructionDefinition(InstructionGroup::SRL, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x3a]) InstructionDefinition(InstructionGroup::SRL, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x3b]) InstructionDefinition(InstructionGroup::SRL, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x3c]) InstructionDefinition(InstructionGroup::SRL, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x3d]) InstructionDefinition(InstructionGroup::SRL, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x3e]) InstructionDefinition(InstructionGroup::SRL, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x3f]) InstructionDefinition(InstructionGroup::SRL, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x30]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x31]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x32]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x33]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x34]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x35]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x36]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x37]) InstructionDefinition(InstructionGroup::SWAP, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x80]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x81]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x82]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x83]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x84]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x85]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0x86]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT0, 0, 4);
    new(&dictPrefix[0x87]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    new(&dictPrefix[0x88]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x89]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x8a]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x8b]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x8c]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x8d]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0x8e]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT1, 0, 4);
    new(&dictPrefix[0x8f]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    new(&dictPrefix[0x90]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x91]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x92]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x93]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x94]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x95]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0x96]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT2, 0, 4);
    new(&dictPrefix[0x97]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    new(&dictPrefix[0x98]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x99]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x9a]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x9b]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x9c]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x9d]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0x9e]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT3, 0, 4);
    new(&dictPrefix[0x9f]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    new(&dictPrefix[0xa0]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa1]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa2]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa3]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa4]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa5]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xa6]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT4, 0, 4);
    new(&dictPrefix[0xa7]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    new(&dictPrefix[0xa8]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xa9]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xaa]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xab]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xac]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xad]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xae]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT5, 0, 4);
    new(&dictPrefix[0xaf]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    new(&dictPrefix[0xb0]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb1]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb2]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb3]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb4]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb5]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xb6]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT6, 0, 4);
    new(&dictPrefix[0xb7]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    new(&dictPrefix[0xb8]) InstructionDefinition(InstructionGroup::RES, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xb9]) InstructionDefinition(InstructionGroup::RES, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xba]) InstructionDefinition(InstructionGroup::RES, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xbb]) InstructionDefinition(InstructionGroup::RES, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xbc]) InstructionDefinition(InstructionGroup::RES, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xbd]) InstructionDefinition(InstructionGroup::RES, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xbe]) InstructionDefinition(InstructionGroup::RES, U8Reg::HL, U8Reg::C_BIT7, 0, 4);
    new(&dictPrefix[0xbf]) InstructionDefinition(InstructionGroup::RES, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    new(&dictPrefix[0xc0]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc1]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc2]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc3]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc4]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc5]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT0, 0, 2);
    new(&dictPrefix[0xc6]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT0, 0, 4);
    new(&dictPrefix[0xc7]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT0, 0, 2);

    new(&dictPrefix[0xc8]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xc9]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xca]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xcb]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xcc]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xcd]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT1, 0, 2);
    new(&dictPrefix[0xce]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT1, 0, 4);
    new(&dictPrefix[0xcf]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT1, 0, 2);

    new(&dictPrefix[0xd0]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd1]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd2]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd3]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd4]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd5]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT2, 0, 2);
    new(&dictPrefix[0xd6]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT2, 0, 4);
    new(&dictPrefix[0xd7]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT2, 0, 2);

    new(&dictPrefix[0xd8]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xd9]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xda]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xdb]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xdc]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xdd]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT3, 0, 2);
    new(&dictPrefix[0xde]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT3, 0, 4);
    new(&dictPrefix[0xdf]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT3, 0, 2);

    new(&dictPrefix[0xe0]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe1]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe2]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe3]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe4]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe5]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT4, 0, 2);
    new(&dictPrefix[0xe6]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT4, 0, 4);
    new(&dictPrefix[0xe7]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT4, 0, 2);

    new(&dictPrefix[0xe8]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xe9]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xea]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xeb]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xec]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xed]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT5, 0, 2);
    new(&dictPrefix[0xee]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT5, 0, 4);
    new(&dictPrefix[0xef]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT5, 0, 2);

    new(&dictPrefix[0xf0]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf1]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf2]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf3]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf4]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf5]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT6, 0, 2);
    new(&dictPrefix[0xf6]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT6, 0, 4);
    new(&dictPrefix[0xf7]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT6, 0, 2);

    new(&dictPrefix[0xf8]) InstructionDefinition(InstructionGroup::SET, U8Reg::B, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xf9]) InstructionDefinition(InstructionGroup::SET, U8Reg::C, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xfa]) InstructionDefinition(InstructionGroup::SET, U8Reg::D, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xfb]) InstructionDefinition(InstructionGroup::SET, U8Reg::E, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xfc]) InstructionDefinition(InstructionGroup::SET, U8Reg::H, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xfd]) InstructionDefinition(InstructionGroup::SET, U8Reg::L, U8Reg::C_BIT7, 0, 2);
    new(&dictPrefix[0xfe]) InstructionDefinition(InstructionGroup::SET, U8Reg::HL, U8Reg::C_BIT7, 0, 4);
    new(&dictPrefix[0xff]) InstructionDefinition(InstructionGroup::SET, U8Reg::A, U8Reg::C_BIT7, 0, 2);

    new(&dictPrefix[0x20]) InstructionDefinition(InstructionGroup::SLA, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x21]) InstructionDefinition(InstructionGroup::SLA, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x22]) InstructionDefinition(InstructionGroup::SLA, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x23]) InstructionDefinition(InstructionGroup::SLA, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x24]) InstructionDefinition(InstructionGroup::SLA, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x25]) InstructionDefinition(InstructionGroup::SLA, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x26]) InstructionDefinition(InstructionGroup::SLA, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x27]) InstructionDefinition(InstructionGroup::SLA, U8Reg::A, U8Reg::A, 0, 2);

    new(&dictPrefix[0x28]) InstructionDefinition(InstructionGroup::SRA, U8Reg::B, U8Reg::B, 0, 2);
    new(&dictPrefix[0x29]) InstructionDefinition(InstructionGroup::SRA, U8Reg::C, U8Reg::C, 0, 2);
    new(&dictPrefix[0x2a]) InstructionDefinition(InstructionGroup::SRA, U8Reg::D, U8Reg::D, 0, 2);
    new(&dictPrefix[0x2b]) InstructionDefinition(InstructionGroup::SRA, U8Reg::E, U8Reg::E, 0, 2);
    new(&dictPrefix[0x2c]) InstructionDefinition(InstructionGroup::SRA, U8Reg::H, U8Reg::H, 0, 2);
    new(&dictPrefix[0x2d]) InstructionDefinition(InstructionGroup::SRA, U8Reg::L, U8Reg::L, 0, 2);
    new(&dictPrefix[0x2e]) InstructionDefinition(InstructionGroup::SRA, U8Reg::HL, U8Reg::HL, 0, 4);
    new(&dictPrefix[0x2f]) InstructionDefinition(InstructionGroup::SRA, U8Reg::A, U8Reg::A, 0, 2);
}