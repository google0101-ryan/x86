#include "hw/cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>

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

void xor_rm16_r16(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint16_t rm16_val = get_rm16(cpu, &modrm);
    uint32_t r16_val = cpu->gpregs[modrm.reg_index].regs_16;
    uint32_t result = rm16_val ^ r16_val;
    set_rm16(cpu, &modrm, result);
    int signr = (result >> 15) & 1;
    printf("XOR 0x%x, 0x%x\n", modrm.rm, modrm.reg_index);
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

void xor_rm8_r8(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint8_t rm8_val = get_rm8(cpu, &modrm);
    uint8_t r8_val;
    if (modrm.reg_index < 4)
        r8_val = cpu->gpregs[modrm.reg_index].regs_8h;
    else
        r8_val = cpu->gpregs[modrm.reg_index].regs_8l;
    uint8_t result = rm8_val ^ r8_val;
    set_rm16(cpu, &modrm, result);
    int signr = (result >> 7) & 1;
    printf("XOR 0x%x, 0x%x\n", modrm.rm, modrm.reg_index);
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