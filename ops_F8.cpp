#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>
#include <stdlib.h>

void jz32(Pentium* cpu)
{
    int diff = cpu->eflags & ZERO_FLAG ? cpu->bus->read32(cpu->getLinearAddr()) : 0;
    cpu->ip.regs_32 += (diff + 6);
}

void jnz32(Pentium* cpu)
{
    int diff = cpu->eflags & ZERO_FLAG ? 0 : cpu->bus->read32(cpu->getLinearAddr());
    cpu->ip.regs_32 += (diff + 6);
}