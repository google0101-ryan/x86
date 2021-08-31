#include "modrm.hpp"
#include "cpu.hpp"

void mov_r8_imm8(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0xB0;
    if (reg < 4)
        cpu->gpregs[reg].regs_8l = cpu->bus->read(cpu->getLinearAddr() + 1);
    else
        cpu->gpregs[reg].regs_8h = cpu->bus->read(cpu->getLinearAddr() + 1);
    cpu->ip.regs_32 += 2;
}