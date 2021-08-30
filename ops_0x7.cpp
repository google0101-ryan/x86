#include "modrm.hpp"
#include "cpu.hpp"

void jge(Pentium* cpu)
{
    int diff = ((cpu->eflags & SIGN_FLAG) == (cpu->eflags & OVERFLOW_FLAG)) ? cpu->bus->read(cpu->getLinearAddr() + 1) : 0;
    cpu->ip.regs_32 += (diff + 2);
}