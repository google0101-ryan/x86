#include "modrm.hpp"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void pop_r32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0x58;
    cpu->gpregs[reg].regs_32 = cpu->pop32();
    cpu->ip.regs_32++;
}