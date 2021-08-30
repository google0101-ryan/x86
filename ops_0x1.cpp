#include "modrm.hpp"
#include "cpu.hpp"

void pop_ds(Pentium* cpu)
{
    cpu->sgregs[(int)SGRegister::DS].base = cpu->pop32();
    cpu->ip.regs_32++;
}