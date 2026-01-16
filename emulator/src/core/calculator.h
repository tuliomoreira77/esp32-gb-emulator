#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>
#include <array>

class Calculator {
public:
    bool overflow = false;

public:
    uint8_t addU8(uint8_t a, uint8_t b);
    uint16_t addU16(uint16_t a, uint16_t b);
    uint16_t addAsSig(uint16_t u16, uint8_t u8);

    uint8_t subU8(uint8_t a, uint8_t b);
    uint16_t subU16(uint16_t a, uint16_t b);

    uint8_t andU8(uint8_t a, uint8_t b);
    uint8_t orU8(uint8_t a, uint8_t b);
    uint8_t xorU8(uint8_t a, uint8_t b);

    int8_t toSigned(uint8_t a);

    uint8_t rotateLeft(uint8_t operand, uint8_t carry);
    uint8_t rotateLeftCarry(uint8_t operand, uint8_t carry);
    uint8_t rotateRight(uint8_t operand, uint8_t carry);
    uint8_t rotateRightCarry(uint8_t operand, uint8_t carry);
    uint8_t shiftRightLogical(uint8_t operand, uint8_t carry);
    uint8_t shiftRightA(uint8_t operand, uint8_t carry);
    uint8_t shiftLeftA(uint8_t operand, uint8_t carry);

    uint8_t swapU8(uint8_t operand);

    uint8_t resetBit(uint8_t operand, uint8_t bit);
    uint8_t setBit(uint8_t operand, uint8_t bit);

    bool verifyOverflow(uint16_t a, uint16_t b, uint8_t bit);
    bool verifyBorrow(uint16_t a, uint16_t b, uint8_t bit);
    bool verifyBit(uint8_t a, uint8_t bit);

    uint8_t notU8(uint8_t a);
};

#endif // CALCULATOR_H
