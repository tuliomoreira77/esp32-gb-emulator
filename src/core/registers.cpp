#include "registers.h"


uint8_t Registers::getA() {
    return regs[U8Reg::A];
}
void Registers::setA(uint8_t v) {
    regs[U8Reg::A] = v;
}

uint8_t Registers::getB() {
    return regs[U8Reg::B];
}
void Registers::setB(uint8_t v) {
    regs[U8Reg::B] = v;
}

uint8_t Registers::getC() {
    return regs[U8Reg::C];
}
void Registers::setC(uint8_t v) {
    regs[U8Reg::C] = v;
}

uint8_t Registers::getD() {
    return regs[U8Reg::D];
}
void Registers::setD(uint8_t v) {
    regs[U8Reg::D] = v;
}

uint8_t Registers::getE() {
    return regs[U8Reg::E];
}
void Registers::setE(uint8_t v) {
    regs[U8Reg::E] = v;
}

uint8_t Registers::getH() {
    return regs[U8Reg::H];
}
void Registers::setH(uint8_t v) {
    regs[U8Reg::H] = v;
}

uint8_t Registers::getL() {
    return regs[U8Reg::L];
}
void Registers::setL(uint8_t v) {
    regs[U8Reg::L] = v;
}

uint8_t Registers::getF() {
    return (zero << 7) |
            (negative << 6) |
            (halfCarry << 5) |
            (carry << 4);
}

void Registers::setF(uint8_t v) {
    zero      = (v & 0b10000000) != 0;
    negative  = (v & 0b01000000) != 0;
    halfCarry = (v & 0b00100000) != 0;
    carry     = (v & 0b00010000) != 0;
}

uint16_t Registers::getBC() {
    return (regs[U8Reg::B] << 8) | regs[U8Reg::C];
}

uint16_t Registers::getDE() {
    return (regs[U8Reg::D] << 8) | regs[U8Reg::E];
}

uint16_t Registers::getHL() {
    return (regs[U8Reg::H] << 8) | regs[U8Reg::L];
}

uint16_t Registers::getAF() {
    return (regs[U8Reg::A] << 8) | getF();
}

void Registers::setBC(uint16_t v) {
    regs[U8Reg::B] = v >> 8;
    regs[U8Reg::C] = v & 0xFF;
}

void Registers::setDE(uint16_t v) {
    regs[U8Reg::D] = v >> 8;
    regs[U8Reg::E] = v & 0xFF;
}

void Registers::setHL(uint16_t v) {
    regs[U8Reg::H] = v >> 8;
    regs[U8Reg::L] = v & 0xFF;
}

void Registers::setAF(uint16_t v) {
    regs[U8Reg::A] = v >> 8;
    setF(v & 0xFF);
}

uint8_t Registers::get8(U8Reg id) {
    if (id < U8Reg::F) {
        return regs[id];
    }
    if (id == U8Reg::F) {
        return getF();
    }
    return 0x00;
}

void Registers::set8(U8Reg id, uint8_t v) {
    if (id < U8Reg::F) {
        regs[id] = v;
        return;
    }
    if (id == U8Reg::F) {
        setF(v);
    }
}

uint16_t Registers::get16(U8Reg id) {
    if (id == U8Reg::HL) return getHL();
    if (id == U8Reg::BC) return getBC();
    if (id == U8Reg::DE) return getDE();
    if (id == U8Reg::AF) return getAF();

    return 0x0000;
}

void Registers::set16(U8Reg id, uint16_t v) {
    if (id == U8Reg::HL) { setHL(v); return; }
    if (id == U8Reg::BC) { setBC(v); return; }
    if (id == U8Reg::DE) { setDE(v); return; }
    if (id == U8Reg::AF) { setAF(v); return; }
}

