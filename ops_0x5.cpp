#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>

void push_r32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0x50;
    cpu->push32(cpu->gpregs[reg].regs_32);
    cpu->ip.regs_32++;
    printf("PUSH 0x%x\n", cpu->gpregs[reg].regs_32);
}

void pop_r32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0x58;
    cpu->gpregs[reg].regs_32 = cpu->pop32();
    cpu->ip.regs_32++;
}