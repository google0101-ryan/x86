#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>
#include <stdlib.h>

void mov_r32_imm32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0xB8;
    uint32_t value = cpu->bus->read32(cpu->getLinearAddr() + 1);
    cpu->gpregs[reg].regs_32 = value;
    cpu->ip.regs_32 += 5;
}