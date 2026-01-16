#include "calculator.h"

uint8_t Calculator::addU8(uint8_t a, uint8_t b) {
    uint16_t result = a + b;
    overflow = result > 0xFF;
    return static_cast<uint8_t>(result);
}

uint16_t Calculator::addU16(uint16_t a, uint16_t b) {
    uint32_t result = a + b;
    overflow = result > 0xFFFF;
    return static_cast<uint16_t>(result);
}

uint16_t Calculator::addAsSig(uint16_t u16, uint8_t u8) {
    int8_t signedByte = toSigned(u8);
    uint32_t result = static_cast<uint32_t>(u16) + signedByte;
    overflow = result > 0xFFFF;
    return static_cast<uint16_t>(result);
}

uint8_t Calculator::subU8(uint8_t a, uint8_t b) {
    uint8_t result = static_cast<uint8_t>(a - b);
    overflow = b > a;
    return result;
}

uint16_t Calculator::subU16(uint16_t a, uint16_t b) {
    uint16_t result = a - b;
    overflow = b > a;
    return result;
}

uint8_t Calculator::andU8(uint8_t a, uint8_t b) {
    overflow = false;
    return a & b;
}

uint8_t Calculator::orU8(uint8_t a, uint8_t b) {
    overflow = false;
    return a | b;
}

uint8_t Calculator::xorU8(uint8_t a, uint8_t b) {
    overflow = false;
    return a ^ b;
}

int8_t Calculator::toSigned(uint8_t a) {
    overflow = false;
    return static_cast<int8_t>(a);
}

uint8_t Calculator::rotateLeft(uint8_t operand, uint8_t carry) {
    overflow = operand & 0x80;
    return (operand << 1) | (carry & 1);
}

uint8_t Calculator::rotateLeftCarry(uint8_t operand, uint8_t carry) {
    uint8_t carryOut = (operand & 0x80) >> 7;
    overflow = carryOut;
    return (operand << 1) | carryOut;
}

uint8_t Calculator::rotateRight(uint8_t operand, uint8_t carry) {
    uint8_t lowbit = operand & 0x01;
    overflow = lowbit;
    return (operand >> 1) | ((carry & 1) << 7);
}

uint8_t Calculator::rotateRightCarry(uint8_t operand, uint8_t carry) {
    uint8_t lowbit = operand & 1;
    overflow = lowbit;
    return (operand >> 1) | (lowbit << 7);
}

uint8_t Calculator::shiftRightLogical(uint8_t operand, uint8_t carry) {
    uint8_t lowbit = operand & 1;
    overflow = lowbit;
    return operand >> 1;
}

uint8_t Calculator::shiftRightA(uint8_t operand, uint8_t carry) {
    uint8_t lowbit = operand & 1;
    overflow = lowbit;
    return (operand >> 1) | (operand & 0x80);
}

uint8_t Calculator::shiftLeftA(uint8_t operand, uint8_t carry) {
    uint8_t topbit = operand >> 7;
    overflow = topbit;
    return (operand << 1);
}

uint8_t Calculator::swapU8(uint8_t operand) {
    return ((operand & 0x0F) << 4) | (operand >> 4);
}

uint8_t Calculator::resetBit(uint8_t operand, uint8_t bit) {
    uint8_t mask = ~(0x01 << bit);
    return operand & mask;
}

uint8_t Calculator::setBit(uint8_t operand, uint8_t bit) {
    uint8_t mask = (0x01 << bit);
    return operand | mask;
}

bool Calculator::verifyOverflow(uint16_t a, uint16_t b, uint8_t bit) {
    uint16_t mask = ((0xFFFFu >> (15 - bit)) & 0xFFFFu);
    return ((a & mask) + (b & mask)) > mask;
}

bool Calculator::verifyBorrow(uint16_t a, uint16_t b, uint8_t bit) {
    uint16_t mask = ((0xFFFFu >> (15 - bit)) & 0xFFFFu);
    return (b & mask) > (a & mask);
}

bool Calculator::verifyBit(uint8_t a, uint8_t bit) {
    return (a >> bit) & 0x01;
}

uint8_t Calculator::notU8(uint8_t a) {
    return ~a;
}
