#include "hw/cpu.hpp"
#include "modrm.hpp"

#define DEFINE_NEAR_JX(flag, flag_name) \
    void j##flag##32(Pentium* cpu) \
    { \
        int diff = cpu->eflags & flag_name ? cpu->bus->read32(cpu->getLinearAddr() + 2) : 0; \
        cpu->ip.regs_32 += (diff + 6); \
    } \
    void jn##flag##32(Pentium* cpu) \
    { \
        int diff = cpu->eflags & flag_name ? 0 : cpu->bus->read32(cpu->getLinearAddr() + 2); \
        cpu->ip.regs_32 += (diff + 6); \
    }

DEFINE_NEAR_JX(c, CARRY_FLAG);
DEFINE_NEAR_JX(z, ZERO_FLAG);

DEFINE_NEAR_JX(s, SIGN_FLAG);

#undef DEFINE_NEAR_JX

void ja32(Pentium* cpu)
{
    int diff = (!cpu->eflags & CARRY_FLAG || !cpu->eflags & ZERO_FLAG) ? cpu->bus->read32(cpu->getLinearAddr()) : 0;
    cpu->ip.regs_32 += (diff + 6);
}