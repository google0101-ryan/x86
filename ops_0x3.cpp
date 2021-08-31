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
    int signr = (result >> 31) & 1;
    cpu->eflags &= ~CARRY_FLAG;
    if (result == 0)
    {
        cpu->eflags |= ZERO_FLAG;
    }
    else
    {
        cpu->eflags &= ~ZERO_FLAG;
    }
    if (signr)
    {
        cpu->eflags |= SIGN_FLAG;
    }
    else
    {
        cpu->eflags &= ~SIGN_FLAG;
    }
    cpu->eflags &= ~OVERFLOW_FLAG;
}

void xor_eax_imm32(Pentium* cpu)
{
    uint32_t eax_val = cpu->gpregs[(int)GPRegister32::EAX].regs_32;
    uint32_t imm32_val = cpu->bus->read32(cpu->getLinearAddr() + 1);
    uint32_t result = eax_val ^ imm32_val;
    cpu->gpregs[(int)GPRegister32::EAX].regs_32 = result;
    cpu->ip.regs_32 += 5;
    int signr = (result >> 31) & 1;
    cpu->eflags &= ~CARRY_FLAG;
    if (result == 0)
    {
        cpu->eflags |= ZERO_FLAG;
    }
    else
    {
        cpu->eflags &= ~ZERO_FLAG;
    }
    if (signr)
    {
        cpu->eflags |= SIGN_FLAG;
    }
    else
    {
        cpu->eflags &= ~SIGN_FLAG;
    }
    cpu->eflags &= ~OVERFLOW_FLAG;
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

void cmp_al_imm8(Pentium* cpu)
{
    uint8_t value = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t al = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    uint16_t result = (uint16_t)al - (uint16_t)value;
    int sign1 = al >> 7;
    int sing2 = result >> 7;
    int signr = (result >> 7) & 1;

    if (result >> 8)
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
    cpu->ip.regs_32 += 2;
}