

#include "cpu.h"
#include "instructions.h"


CPU::CPU(MemoryBus *bus) {
    registers = new Registers();
    alu = new Calculator();
    instrucionDict = new InstructionDict();
    this->bus = bus;

    registers->setAF(0x01b0);
    registers->setBC(0x0013);
    registers->setDE(0x00d8);
    registers->setHL(0x014d);

    bus->writeByte(INTERRUPT_ENABLE_REGISTER, 0x00);
    bus->writeByte(TIMER_CONTROL, 0xF8);
    bus->writeByte(INTERRUPT_FLAG, 0xe1);
    bus->writeByte(LCD_CONTROL, 0x91);
    bus->writeByte(LCD_STAT, 0x81);


    instHandler[NOP] = noOp;
};

uint16_t CPU::executeStep() {
    clockCycle = 0;
    if (isHalt == true) {

    } else {
        Instruction* instruction = getInstruction();
        instructionRouter(instruction);

        delete instruction;
    }

    return clockCycle;
}


Instruction* CPU::getInstruction() {
    uint8_t rawInstruction = bus->readByte(programCounter);
    uint8_t programStep = 1;

    InstructionDefinition *instructionDefinition = nullptr;

    if (rawInstruction == 0xCB) {
        rawInstruction = bus->readByte(programCounter + programStep);
        programStep += 1;
        instructionDefinition = instrucionDict->get(rawInstruction, true);
    } else {
        instructionDefinition = instrucionDict->get(rawInstruction, false);
    }

    operandsBuffer[0] = 0;
    operandsBuffer[1] = 0;
    if (instructionDefinition->operandsNumber > 0) {
        for (int i=0; i < instructionDefinition->operandsNumber; i++) {
            operandsBuffer[i] = bus->readByte(programCounter + programStep);
            programStep += 1;
        }
    }

    Instruction *instruction = new Instruction(instructionDefinition, operandsBuffer);
    programCounter = programCounter + programStep;
    clockCycle = clockCycle + instructionDefinition->cycles;

    return instruction;

};

void CPU::instructionRouter(Instruction* instruction) {
    void (CPU::*handler)(Instruction*) = instHandler[instruction->definition->group];
    (this->*handler)(instruction);
}


void CPU::noOp(Instruction *instruction) {   }