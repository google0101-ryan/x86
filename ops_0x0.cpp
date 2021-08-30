#include "modrm.hpp"
#include "cpu.hpp"

void push_es(Pentium* cpu)
{
    cpu->push32(cpu->sgregs[(int)SGRegister::ES].base);
    cpu->ip.regs_32++;
}

void pop_es(Pentium* cpu)
{
    cpu->sgregs[(int)SGRegister::ES].base = cpu->pop32();
    cpu->ip.regs_32++;
}