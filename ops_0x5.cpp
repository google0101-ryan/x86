#include "cpu.hpp"
#include "modrm.hpp"

void push_r32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0x50;
    cpu->push32(cpu->gpregs[reg].regs_32);
    cpu->ip.regs_32++;
}