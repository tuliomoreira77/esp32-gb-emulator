#include <Arduino.h>
#include "registers.h"


uint8_t Registers::getA() {
    return A;
}
void Registers::setA(uint8_t v) {
    A = v;
}

uint8_t Registers::getB() {
    return B;
}
void Registers::setB(uint8_t v) {
    B = v;
}

uint8_t Registers::getC() {
    return C;
}
void Registers::setC(uint8_t v) {
    C = v;
}

uint8_t Registers::getD() {
    return D;
}
void Registers::setD(uint8_t v) {
    D = v;
}

uint8_t Registers::getE() {
    return E;
}
void Registers::setE(uint8_t v) {
    E = v;
}

uint8_t Registers::getH() {
    return H;
}
void Registers::setH(uint8_t v) {
    H = v;
}

uint8_t Registers::getL() {
    return L;
}
void Registers::setL(uint8_t v) {
    L = v;
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
    return (B << 8) | C;
}

uint16_t Registers::getDE() {
    return (D << 8) | E;
}

uint16_t Registers::getHL() {
    return (H << 8) | L;
}

uint16_t Registers::getAF() {
    return (A << 8) | getF();
}

void Registers::setBC(uint16_t v) {
    B = v >> 8;
    C = v & 0xFF;
}

void Registers::setDE(uint16_t v) {
    D = v >> 8;
    E = v & 0xFF;
}

void Registers::setHL(uint16_t v) {
    H = v >> 8;
    L = v & 0xFF;
}

void Registers::setAF(uint16_t v) {
    A = v >> 8;
    setF(v & 0xFF);
}

uint8_t Registers::get8(U8Reg id) {
    if (id == U8Reg::A) return A;
    if (id == U8Reg::B) return B;
    if (id == U8Reg::C) return C;
    if (id == U8Reg::D) return D;
    if (id == U8Reg::E) return E;
    if (id == U8Reg::H) return H;
    if (id == U8Reg::L) return L;
    if (id == U8Reg::F) return getF();

    return 0x00;
}

void Registers::set8(U8Reg id, uint8_t v) {
    if (id == U8Reg::A) { A = v; return; }
    if (id == U8Reg::B) { B = v; return; }
    if (id == U8Reg::C) { C = v; return; }
    if (id == U8Reg::D) { D = v; return; }
    if (id == U8Reg::E) { E = v; return; }
    if (id == U8Reg::H) { H = v; return; }
    if (id == U8Reg::L) { L = v; return; }
    if (id == U8Reg::F) { setF(v); return; }
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

