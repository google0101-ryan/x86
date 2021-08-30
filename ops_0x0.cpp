#include "modrm.hpp"
#include "cpu.hpp"
#include <stdio.h>

void push_es(Pentium* cpu)
{
    cpu->push32(cpu->sgregs[(int)SGRegister::ES].base);
    cpu->ip.regs_32++;
    printf("PUSH ES (0x%x)\n", cpu->sgregs[(int)SGRegister::ES].base);
}

void pop_es(Pentium* cpu)
{
    cpu->sgregs[(int)SGRegister::ES].base = cpu->pop32();
    cpu->ip.regs_32++;
}