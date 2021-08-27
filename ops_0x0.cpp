#include "cpu.hpp"
#include <stdio.h>

void or_al_imm8(Pentium* cpu)
{
    uint8_t imm8_val = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    uint8_t result = al_val | imm8_val;
    cpu->gpregs[(int)GPRegister8::AL].regs_8l = result;
    cpu->ip.regs_32 += 2;
}