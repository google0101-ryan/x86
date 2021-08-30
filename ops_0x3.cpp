#include "cpu.hpp"
#include "modrm.hpp"

void xor_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32;
    uint32_t result = rm32_val ^ r32_val;
    set_rm32(cpu, &modrm, result);
}

void cmp_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32;
    uint64_t result = (uint64_t)rm32_val - (uint64_t)r32_val;
    int sign1 = rm32_val >> 31;
    int sing2 = r32_val >> 31;
    int signr = (result >> 31) & 1;

    if (result >> 32)
    {
        cpu->eflags |= CARRY_FLAG;
    }
    else
    {
        cpu->eflags &= ~CARRY_FLAG;
    }
    if (result == 0)
    {
        cpu->eflags |= ZERO_FLAG;
    }
    else
    {
        cpu->eflags &= ~ZERO_FLAG;
    }
}