#include "hw/cpu.hpp"
#include "modrm.hpp"

void xchg_r32_r32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0x90;
    uint32_t original_val = cpu->gpregs[reg].regs_32;
    cpu->gpregs[reg].regs_32 = cpu->gpregs[(int)GPRegister32::EAX].regs_32;
    cpu->gpregs[(int)GPRegister32::EAX].regs_32 = original_val;
    cpu->ip.regs_32++;
}