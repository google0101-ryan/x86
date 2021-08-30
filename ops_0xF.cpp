#include "modrm.hpp"
#include "cpu.hpp"

void cld(Pentium* cpu)
{
    cpu->eflags &= ~(DIRECTION_FLAG);
    cpu->ip.regs_32++;
}