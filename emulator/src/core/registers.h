#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>
#include <string>
#include "reg_enums.h"

class Registers {

private:
    uint8_t A = 0x00;
    uint8_t B = 0x00;
    uint8_t C = 0x00;
    uint8_t D = 0x00;
    uint8_t E = 0x00;
    uint8_t H = 0x00;
    uint8_t L = 0x00;

public:
    bool zero      = false;
    bool negative  = false;
    bool halfCarry = false;
    bool carry     = false;

    uint8_t getA();
    void setA(uint8_t v);

    uint8_t getB();
    void setB(uint8_t v);

    uint8_t getC();
    void setC(uint8_t v);

    uint8_t getD();
    void setD(uint8_t v);

    uint8_t getE();
    void setE(uint8_t v);

    uint8_t getH();
    void setH(uint8_t v);

    uint8_t getL();
    void setL(uint8_t v);

    uint8_t getF();
    void setF(uint8_t v);

    uint16_t getBC();
    uint16_t getDE();
    uint16_t getHL();
    uint16_t getAF();

    void setBC(uint16_t v);
    void setDE(uint16_t v);
    void setHL(uint16_t v);
    void setAF(uint16_t v);

    uint8_t  get8(U8Reg id);
    void     set8(U8Reg id, uint8_t v);

    uint16_t get16(U8Reg id);
    void     set16(U8Reg id, uint16_t v);
};

#endif // REGISTERS_H
