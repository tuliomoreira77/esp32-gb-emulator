#include "cpu.h"
#include "instructions.h"

CPU::CPU(MemoryBus *bus) {
    registers = new Registers();
    alu = new Calculator();
    instrucionDict = new InstructionDict();
    currentInstruction = new Instruction();

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
};

/*
void CPU::breakPoint(Instruction* instruction) {
    uint16_t points[] = {0xC88F};
    for(int i=0; i < 1; i++) {
        if(programCounter == points[i]) {
            debugInstr(instruction);
            debugReg();
        }
    }
}

void CPU::debugReg() {
    Serial.print("NEXT PC: ");
    Serial.println(programCounter, HEX);

    Serial.print("AF: ");
    Serial.println(registers->getAF(), HEX);

    Serial.print("BC: ");
    Serial.println(registers->getBC(), HEX);

    Serial.print("DE: ");
    Serial.println(registers->getDE(), HEX);

    Serial.print("HL: ");
    Serial.println(registers->getHL(), HEX);

    Serial.print("SP: ");
    Serial.println(stackPointer, HEX);
    Serial.println("");
}

void CPU::debugInstr(Instruction* instruction) {
    Serial.print(instruction->definition->group);
    Serial.print(" ");
    Serial.print(instruction->definition->def_1);
    Serial.print(" ");
    Serial.print(instruction->definition->def_2);
    Serial.print("    ");
    Serial.print(instruction->operands[0], HEX);
    Serial.print(" ");
    Serial.print(instruction->operands[1], HEX);
    Serial.println("");
}
*/


uint16_t CPU::executeStep() {
    clockCycle = 0;
    uint8_t pendingInterrupts = verifyPendingInterrupts();

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

        fetchInstruction();
        instructionRouter(currentInstruction);
        //breakPoint(instruction);
    }
    return clockCycle;
}

__attribute__((hot, always_inline))
inline void CPU::fetchInstruction() {
    uint8_t* rawInstruction = bus->fetchBlock(programCounter);

    InstructionDefinition *instructionDefinition = nullptr;

    if (rawInstruction[0] == 0xCB) {
        rawInstruction = rawInstruction + 1;
        programCounter = programCounter + 1;
        instructionDefinition = instrucionDict->get(rawInstruction[0], true);
    } else {
        instructionDefinition = instrucionDict->get(rawInstruction[0], false);
    }

    currentInstruction->definition = instructionDefinition;

    uint8_t operandsNumber = instructionDefinition->operandsNumber;
    currentInstruction->operands[0] = operandsNumber > 0 ? rawInstruction[1] : 0x00;
    currentInstruction->operands[1] = operandsNumber > 1 ? rawInstruction[2] : 0x00;

    programCounter = programCounter + 1 + operandsNumber;
    clockCycle = clockCycle + instructionDefinition->cycles;
};

__attribute__((hot, always_inline))
inline void CPU::instructionRouter(Instruction* instruction) {
    const uint8_t g = static_cast<uint8_t>(instruction->definition->group);

    if (g <= LDS) {
        if (g == LD)   { basicLoad(instruction);   return; }
        if (g == LDP)  { pointerLoad(instruction); return; }
        if (g == LDH)  { loadHigh(instruction);    return; }
        if (g == LD16) { load16(instruction);      return; }
        /* g == LDS */ { loadStack(instruction);   return; }
    }

    if (g <= DAA) {
        if (g == ADD || g == ADDC) { addU8(instruction); return; }

        if (g == SUB || g == SUBC) { subU8(instruction); return; }

        if (g == AND) { andU8(instruction); return; }
        if (g == OR)  { orU8(instruction);  return; }
        if (g == XOR) { xorU8(instruction); return; }

        if (g == CP)  { compare(instruction);       return; }
        /* g == DAA */{ decimalAdjust(instruction); return; }
    }

    if (g <= DEC16) {
        if (g == INC)   { incrementU8(instruction);  return; }
        if (g == DEC8)  { decrementU8(instruction);  return; }
        if (g == INC16) { incrementU16(instruction); return; }
        /* g == DEC16 */{ decrementU16(instruction); return; }
    }

    if (g <= ADDSP) {
        if (g == ADDHL) { addU16(instruction);       return; }
        /* g == ADDSP */{ addSignedSP(instruction);  return; }
    }

    if (g <= RST) {
        if (g == JP)   { directJump(instruction);   return; }
        if (g == JPR)  { relativeJump(instruction); return; }
        if (g == CALL) { call(instruction);         return; }
        if (g == RET)  { ret(instruction);          return; }
        if (g == RETI) { reti(instruction);         return; }
        /* g == RST */ { rst(instruction);          return; }
    }

    if (g <= POP) {
        if (g == PUSH) { push(instruction); return; }
        /* g == POP */ { pop(instruction);  return; }
    }

    if (g <= SWAP) {
        if (g == RL)   { rotateLeft(instruction);        return; }
        if (g == RLC)  { rotateLeftCarry(instruction);   return; }
        if (g == RR)   { rotateRight(instruction);       return; }
        if (g == RRC)  { rotateRightCarry(instruction);  return; }

        if (g == RLCA) { rotateLeftACarry(instruction);  return; }
        if (g == RLA)  { rotateLeftA(instruction);       return; }
        if (g == RRCA) { rotateRightACarry(instruction); return; }
        if (g == RRA)  { rotateRightA(instruction);      return; }

        if (g == SLA)  { shiftLeftA(instruction);        return; }
        if (g == SRA)  { shiftRightA(instruction);       return; }
        if (g == SRL)  { shiftRightL(instruction);       return; }
        /* g == SWAP */{ swap(instruction);              return; }
    }

    if (g <= RES) {
        if (g == BIT) { bitOp(instruction); return; }
        if (g == SET) { set(instruction);   return; }
        /* g == RES */{ reset(instruction); return; }
    }

    if (g <= CPL) {
        if (g == CCF) { complementCarryFlag(instruction);  return; }
        if (g == SCF) { setCarryFlag(instruction);         return; }
        /* g == CPL */{ complementAcumulator(instruction); return; }
    }

    if (g == DI)   { disableInterrupts(instruction); return; }
    if (g == EI)   { enableInterrupts(instruction);  return; }
    if (g == HALT) { halt(instruction);              return; }
    if (g == NOP)  { noOp(instruction);              return; }
    if (g == STOP) { noOp(instruction);              return; }
}


void CPU::noOp(Instruction *instruction) {   }


void CPU::addU8(Instruction *instruction) {
    InstructionGroup group = instruction->definition->group;
    U8Reg definition = instruction->definition->def_1;

    uint8_t carry = (registers->carry && group == ADDC) ? 1 : 0;
    uint8_t operand = 0;

    switch (definition) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;
        
        default:
            operand = registers->get8(definition);
            break;
    }

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


void CPU::addU16(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint16_t accumulator = registers->getHL();
    uint16_t operand = 0;

    switch (operandDef) {
        case U8Reg::SP:
            operand = stackPointer;
            break;

        default:
            operand = registers->get16(operandDef);
            break;
    }

    uint16_t result = alu->addU16(accumulator, operand);

    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(accumulator, operand, 11);
    registers->carry = alu->overflow;

    registers->setHL(result);
}


void CPU::addSignedSP(Instruction* instruction) {
    uint16_t accumulator = stackPointer;
    int8_t operand = static_cast<int8_t>(instruction->operands[0]);

    uint16_t result = alu->addAsSig(accumulator, operand);

    registers->zero = false;
    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(accumulator, operand, 3);
    registers->carry = alu->verifyOverflow(accumulator, operand, 7);

    stackPointer = result;
}


void CPU::andU8(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t operand = 0;
    switch (operandDef) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;

        default:
            operand = registers->get8(operandDef);
            break;
    }

    uint8_t accumulator = registers->getA();
    uint8_t result = alu->andU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = true;
    registers->carry = false;

    registers->setA(result);
}


void CPU::orU8(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_2;

    uint8_t operand = 0;
    switch (operandDef) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;

        default:
            operand = registers->get8(operandDef);
            break;
    }

    uint8_t accumulator = registers->getA();
    uint8_t result = alu->orU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;

    registers->setA(result);
}


void CPU::xorU8(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_2;

    uint8_t operand = 0;
    switch (operandDef) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;

        default:
            operand = registers->get8(operandDef);
            break;
    }

    uint8_t accumulator = registers->getA();
    uint8_t result = alu->xorU8(accumulator, operand);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;

    registers->setA(result);
}

void CPU::subU8(Instruction* instruction) {
    InstructionGroup baseInst = instruction->definition->group;
    U8Reg operandDef = instruction->definition->def_2;

    bool useCarry = (baseInst == InstructionGroup::SUBC);
    uint8_t carry = (useCarry && registers->carry) ? 1 : 0;

    uint8_t operand = 0;
    switch (operandDef) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;

        default:
            operand = registers->get8(operandDef);
            break;
    }

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

void CPU::decimalAdjust(Instruction* instruction) {
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
}


void CPU::compare(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t operand = 0;
    switch (operandDef) {
        case U8Reg::HL:
            operand = bus->readByte(registers->getHL());
            break;

        case U8Reg::D8:
            operand = instruction->operands[0];
            break;

        default:
            operand = registers->get8(operandDef);
            break;
    }

    uint8_t a = registers->getA();

    registers->zero = (operand == a);
    registers->negative = true;
    registers->halfCarry = alu->verifyBorrow(a, operand, 3);
    registers->carry = (operand > a);
}


void CPU::decrementU8(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t value = 0;
    switch (operandDef) {
        case U8Reg::HL:
            value = bus->readByte(registers->getHL());
            break;

        default:
            value = registers->get8(operandDef);
            break;
    }

    uint8_t result = alu->subU8(value, 1);

    registers->zero = (result == 0);
    registers->negative = true;
    registers->halfCarry = alu->verifyBorrow(value, 1, 3);

    switch (operandDef) {
        case U8Reg::HL:
            bus->writeByte(registers->getHL(), result);
            break;

        default:
            registers->set8(operandDef, result);
            break;
    }
}

void CPU::incrementU8(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t value = 0;
    switch (operandDef) {
        case U8Reg::HL:
            value = bus->readByte(registers->getHL());
            break;

        default:
            value = registers->get8(operandDef);
            break;
    }

    uint8_t result = alu->addU8(value, 1);

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = alu->verifyOverflow(value, 1, 3);

    switch (operandDef) {
        case U8Reg::HL:
            bus->writeByte(registers->getHL(), result);
            break;

        default:
            registers->set8(operandDef, result);
            break;
    }
}

void CPU::decrementU16(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint16_t value = 0;
    switch (operandDef) {
        case U8Reg::SP:
            value = stackPointer;
            break;
        default:
            value = registers->get16(operandDef);
            break;
    }

    uint16_t result = alu->subU16(value, 1);
    switch (operandDef) {
        case U8Reg::SP:
            stackPointer = result;
            break;
        default:
            registers->set16(operandDef, result);
            break;
    }
}

void CPU::incrementU16(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint16_t value = 0;
    switch (operandDef) {
        case U8Reg::SP:
            value = stackPointer;
            break;
        default:
            value = registers->get16(operandDef);
            break;
    }

    uint16_t result = alu->addU16(value, 1);
    switch (operandDef) {
        case U8Reg::SP:
            stackPointer = result;
            break;
        default:
            registers->set16(operandDef, result);
            break;
    }
}

void CPU::directJump(Instruction* instruction){
    U8Reg addressDef = instruction->definition->def_1;

    uint16_t jumpAddress = 0;
    bool jump = false;

    switch (addressDef) {
        case U8Reg::HL:
            jumpAddress = registers->getHL();
            jump = true;
            break;
        case U8Reg::D16:
            jumpAddress = build16FromOperands(instruction);
            jump = true;
            clockCycle += 1;
            break;
        case U8Reg::NZ:
            if (!registers->zero) {
                jumpAddress = build16FromOperands(instruction);
                jump = true;
                clockCycle += 1;
            }
            break;

        case U8Reg::Z:
            if (registers->zero) {
                jumpAddress = build16FromOperands(instruction);
                jump = true;
                clockCycle += 1;
            }
            break;
        case U8Reg::NC:
            if (!registers->carry) {
                jumpAddress = build16FromOperands(instruction);
                jump = true;
                clockCycle += 1;
            }
            break;
        case U8Reg::CR:
            if (registers->carry) {
                jumpAddress = build16FromOperands(instruction);
                jump = true;
                clockCycle += 1;
            }
            break;
    }

    if (jump == true) {
        programCounter = jumpAddress;
    }
}

void CPU::relativeJump(Instruction* instruction){
    U8Reg addressDef = instruction->definition->def_1;

    int8_t offset = instruction->operands[0];
    bool jump = false;

    switch (addressDef) {
        case U8Reg::D16:
            jump = true;
            break;
        case U8Reg::NZ:
            if (!registers->zero) {
                jump = true;
            }
            break;

        case U8Reg::Z:
            if (registers->zero) {
                jump = true;
            }
            break;
        case U8Reg::NC:
            if (!registers->carry) {
                jump = true;
            }
            break;
        case U8Reg::CR:
            if (registers->carry) {
                jump = true;
            }
            break;
    }

    if (jump == true) {
        uint16_t nextAddr = alu->addAsSig(programCounter, offset);
        programCounter = nextAddr;
    }
}

void CPU::call(Instruction* insctruction) {
    U8Reg addressDef = insctruction->definition->def_1;

    uint16_t callAddress = build16FromOperands(insctruction);
    bool call = false;

    switch (addressDef) {
        case U8Reg::D16:
            call = true;
            break;
        case U8Reg::NZ:
            if (!registers->zero) {
                call = true;
                clockCycle += 3;
            }
            break;

        case U8Reg::Z:
            if (registers->zero) {
                call = true;
                clockCycle += 3;
            }
            break;
        case U8Reg::NC:
            if (!registers->carry) {
                call = true;
                clockCycle += 3;
            }
            break;
        case U8Reg::CR:
            if (registers->carry) {
                call = true;
                clockCycle += 3;
            }
            break;
    }

    if (call == true) {
        pushToStack(programCounter);
        programCounter = callAddress;
    }
}


void CPU::ret(Instruction* insctruction) {
    U8Reg addressDef = insctruction->definition->def_1;

    bool ret = false;

    switch (addressDef) {
        case U8Reg::D16:
            ret = true;
            break;
        case U8Reg::NZ:
            if (!registers->zero) {
                ret = true;
                clockCycle += 3;
            }
            break;

        case U8Reg::Z:
            if (registers->zero) {
                ret = true;
                clockCycle += 3;
            }
            break;
        case U8Reg::NC:
            if (!registers->carry) {
                ret = true;
                clockCycle += 3;
            }
            break;
        case U8Reg::CR:
            if (registers->carry) {
                ret = true;
                clockCycle += 3;
            }
            break;
    }

    if (ret == true) {
        uint16_t returnAddress = popFromStack();
        programCounter = returnAddress;
    }
}


void CPU::rst(Instruction* instruction) {
    U8Reg def = instruction->definition->def_1;
    uint16_t callAddr = vectorLookup[def];
    pushToStack(programCounter);
    programCounter = callAddr;
}


void CPU::complementCarryFlag(Instruction* instruction) {
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = !registers->carry;
}


void CPU::complementAcumulator(Instruction* instruction) {
    uint8_t result = alu->notU8(registers->getA());
    registers->setA(result);

    registers->negative = true;
    registers->halfCarry = true;
}

void CPU::rotateLeftA(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateLeft);
    registers->zero = false;
}

void CPU::rotateLeftACarry(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateLeftCarry);
    registers->zero = false;
}

void CPU::rotateRightA(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateRight);
    registers->zero = false;
}

void CPU::rotateRightACarry(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateRightCarry);
    registers->zero = false;
}

void CPU::rotateLeft(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateLeft);
}

void CPU::rotateLeftCarry(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateLeftCarry);
}

void CPU::rotateRight(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateRight);
}

void CPU::rotateRightCarry(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::rotateRightCarry);
}

void CPU::shiftRightL(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::shiftRightLogical);
}

void CPU::shiftRightA(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::shiftRightA);
}

void CPU::shiftLeftA(Instruction* instruction) {
    shiftRotationOpr(instruction, &Calculator::shiftLeftA);
}

void CPU::shiftRotationOpr(Instruction* instruction, uint8_t (Calculator::*alu_opr)(uint8_t, uint8_t)) {
    uint8_t carry = registers->carry == true ? 1 : 0;
    uint8_t result = 0;

    U8Reg operandDef = instruction->definition->def_1;
    switch (operandDef) {
        case U8Reg::HL:
            {
                uint8_t value = bus->readByte(registers->getHL());
                result = (alu->*alu_opr)(value, carry);
                bus->writeByte(registers->getHL(), result);
            }
            break;

        default:
            {
                uint8_t value = registers->get8(operandDef);
                result = (alu->*alu_opr)(value, carry);
                registers->set8(operandDef, result);
            }
            break;
    }

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = alu->overflow;
}

void CPU::swap(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t result = 0;
    switch (operandDef) {
        case U8Reg::HL:
            {
                uint8_t value = bus->readByte(registers->getHL());
                result = alu->swapU8(value);
                bus->writeByte(registers->getHL(), result);
            }
            break;

        default:
            {
                uint8_t value = registers->get8(operandDef);
                result = alu->swapU8(value);
                registers->set8(operandDef, result);
            }
            break;
    }

    registers->zero = (result == 0);
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = false;
}

void CPU::bitOp(Instruction* instruction) {
    U8Reg bitDef = instruction->definition->def_2;
    U8Reg operandDef = instruction->definition->def_1;

    uint8_t bitPosition = bitDef-30;
    uint8_t value = 0;

    switch (operandDef) {
        case U8Reg::HL:
            value = bus->readByte(registers->getHL());
            break;

        default:
            value = registers->get8(operandDef);
            break;
    }

    bool bitSet = alu->verifyBit(value, bitPosition);

    registers->zero = !bitSet;
    registers->negative = false;
    registers->halfCarry = true;
}

void CPU::set(Instruction* instruction) {
    setReset(instruction, &Calculator::setBit);
}

void CPU::reset(Instruction* instruction) {
    setReset(instruction, &Calculator::resetBit);
}

void CPU::setReset(Instruction* instruction, uint8_t (Calculator::*alu_opr)(uint8_t, uint8_t)) {
    U8Reg operandDef = instruction->definition->def_1;
    U8Reg bitDef = instruction->definition->def_2;

    uint8_t bitPosition = bitDef-30;
    uint8_t result = 0;
    uint8_t value = 0;

    switch (operandDef) {
        case U8Reg::HL:
            value = bus->readByte(registers->getHL());
            result = (alu->*alu_opr)(value, bitPosition);
            bus->writeByte(registers->getHL(), result);
            break;

        default:
            value = registers->get8(operandDef);
            result = (alu->*alu_opr)(value, bitPosition);
            registers->set8(operandDef, result);
            break;
    }
}

void CPU::setCarryFlag(Instruction* instruction) {
    registers->negative = false;
    registers->halfCarry = false;
    registers->carry = true;
}

void CPU::disableInterrupts(Instruction* instruction) {
    interruptEnable = false;
}

void CPU::enableInterrupts(Instruction* instruction) {
    interruptEnable = true;
}

void CPU::halt(Instruction* instruction) {
    if (interruptEnable == true) {
        isHalt = true;
    } else {
        if (verifyPendingInterrupts() != 0) {
            isHalt = false;
        } else {
            isHalt = true;
        }
    }
}

void CPU::reti(Instruction* instruction) {
    interruptEnable = true;
    programCounter = popFromStack();
}

void CPU::basicLoad(Instruction* instruction) {
    U8Reg destDef = instruction->definition->def_1;
    U8Reg srcDef = instruction->definition->def_2;

    uint8_t value = 0;
    switch (srcDef) {
        case U8Reg::D8:
            value = instruction->operands[0];
            break;

        case U8Reg::HL:
            value = bus->readByte(registers->getHL());
            break;

        default:
            value = registers->get8(srcDef);
            break;
    }

    switch (destDef) {
        case U8Reg::HL:
            bus->writeByte(registers->getHL(), value);
            break;

        default:
            registers->set8(destDef, value);
            break;
    }
}

void CPU::pointerLoad(Instruction* instruction) {
    U8Reg destDef = instruction->definition->def_1;
    U8Reg srcDef = instruction->definition->def_2;

    uint8_t value = 0;
    uint16_t addr = 0;
    switch (srcDef) {
        case U8Reg::A:
            value = registers->getA();
            break;

        case U8Reg::D16:
            addr = build16FromOperands(instruction);
            value = bus->readByte(addr);
            break;

        case U8Reg::HLI:
            addr = registers->getHL();
            value = bus->readByte(addr);
            registers->setHL(addr + 1);
            break;

        case U8Reg::HLD:
            addr = registers->getHL();
            value = bus->readByte(addr);
            registers->setHL(addr - 1);
            break;

        default:
            value = bus->readByte(registers->get16(srcDef));
            break;
    }

    switch (destDef) {
        case U8Reg::A:
            registers->setA(value);
            break;

        case U8Reg::D16:
            addr = build16FromOperands(instruction);
            bus->writeByte(addr, value);
            break;

        case U8Reg::HLI:
            addr = registers->getHL();
            bus->writeByte(addr, value);
            registers->setHL(addr + 1);
            break;

        case U8Reg::HLD:
            addr = registers->getHL();
            bus->writeByte(addr, value);
            registers->setHL(addr - 1);
            break;

        default:
            bus->writeByte(registers->get16(destDef), value);
            break;
    }
}

void CPU::load16(Instruction* instruction) {
    U8Reg destDef = instruction->definition->def_1;

    uint16_t value = build16FromOperands(instruction);

    switch (destDef) {
        case U8Reg::SP:
            stackPointer = value;
            break;
        default:
            registers->set16(destDef, value);
            break;
    }
}

void CPU::loadStack(Instruction* instruction) {
    U8Reg destDef = instruction->definition->def_1;

    uint16_t value = 0;
    uint16_t addr;
    switch (destDef) {
        case U8Reg::D16:
            addr = build16FromOperands(instruction);
            value = stackPointer;
            bus->writeByte(addr, static_cast<uint8_t>(value));
            bus->writeByte(addr + 1, static_cast<uint8_t>(value >> 8));
            break;
        case U8Reg::SP:
            stackPointer = registers->getHL();
            break;
        case U8Reg::HL:
            value = alu->addAsSig(stackPointer, instruction->operands[0]);
            registers->setHL(value);
            registers->zero = false;  
            registers->negative = false;
            registers->halfCarry = alu->verifyOverflow(stackPointer, instruction->operands[0], 3);
            registers->carry = alu->verifyOverflow(stackPointer, instruction->operands[0], 7);
            break;
    }
}


void CPU::loadHigh(Instruction* instruction) {
    U8Reg destDef = instruction->definition->def_1;
    U8Reg srcDef = instruction->definition->def_2;

    uint16_t baseAddr = 0xFF00;
    uint16_t addr = 0;
    uint8_t offset = 0;
    uint8_t value = 0;

    switch (srcDef) {
        case U8Reg::C:
            offset = registers->getC();
            addr = baseAddr + offset;
            value = bus->readByte(addr);
            break;

        case U8Reg::D8:
            offset = instruction->operands[0];
            addr = baseAddr + offset;
            value = bus->readByte(addr);
            break;

        case U8Reg::A:
            value = registers->getA();
            break;
    }

    switch (destDef) {
        case U8Reg::A:
            registers->setA(value);
            break;

        case U8Reg::C:
            offset = registers->getC();
            addr = baseAddr + offset;
            bus->writeByte(addr, value);
            break;

        case U8Reg::D8:
            offset = instruction->operands[0];
            addr = baseAddr + offset;
            bus->writeByte(addr, value);
            break;
    }
}

void CPU::pop(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;
    uint16_t value = popFromStack();
    registers->set16(operandDef, value);
}

void CPU::push(Instruction* instruction) {
    U8Reg operandDef = instruction->definition->def_1;
    uint16_t value = registers->get16(operandDef);
    pushToStack(value);
}

void CPU::pushToStack(uint16_t addr) {
    uint8_t lowByte = static_cast<uint8_t>(addr);
    uint8_t highByte = static_cast<uint8_t>((addr) >> 8);

    stackPointer -= 1;
    bus->writeByte(stackPointer, highByte);
    stackPointer -= 1;
    bus->writeByte(stackPointer, lowByte);
}

uint16_t CPU::popFromStack() {
    uint8_t* bytes = bus->fetchBlock(stackPointer);
    stackPointer += 2;

    return (static_cast<uint16_t>(bytes[1])) << 8 | static_cast<uint16_t>(bytes[0]);
}

uint16_t CPU::build16FromOperands(Instruction* instruction) {
    uint8_t lowByte = instruction->operands[0];
    uint8_t highByte = instruction->operands[1];

    return (static_cast<uint16_t>(highByte)) << 8 | static_cast<uint16_t>(lowByte);
}

void CPU::handleInterrupts(uint8_t pendingInterrupts) {
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

uint8_t CPU::getFirstInterrupt(uint8_t pendingInterrupts) {
    for(uint8_t i=0; i<5; i++) {
        if ((pendingInterrupts & 0x01) !=0) {
            return i;
        }
        pendingInterrupts = pendingInterrupts >> 1;
    }

    return 7;
}

__attribute__((hot, always_inline))
inline uint8_t CPU::verifyPendingInterrupts() {
    uint8_t interruptEnableReg = bus->readVRam(INTERRUPT_ENABLE_REGISTER);
    uint8_t interruptFlag = bus->readVRam(INTERRUPT_FLAG);
    return interruptEnableReg & interruptFlag;
}





