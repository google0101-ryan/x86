#include "cpu.hpp"
#include "modrm.hpp"
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    uint32_t result;
    uint8_t carry;
} ShiftRes32;

void mov_rm32_imm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t value = cpu->bus->read32(cpu->getLinearAddr());
    cpu->ip.regs_32 += 4;
    set_rm32(cpu, &modrm, value);
}

void shr_rm32_imm8(Pentium* cpu, ModRM* modrm)
{
    uint8_t imm8_val = cpu->bus->read(cpu->getLinearAddr());
    while (imm8_val > 32)
    {
        imm8_val -= 32;
    }
    cpu->ip.regs_32++;
    uint64_t ext_val = (uint64_t)get_rm32(cpu, modrm);
    uint64_t ext_carry = (uint64_t)cpu->eflags & CARRY_FLAG;

    uint64_t cf_added = (ext_val << 1) | ext_carry;
    uint64_t result = cf_added >> imm8_val;

    ShiftRes32 res;
    res.result = (uint32_t)(result >> 1);
    res.carry = (result & 1) != 0;
    set_rm32(cpu, modrm, res.result);
    if (res.carry)
    {
        cpu->eflags |= CARRY_FLAG;
    }
    else
    {
        cpu->eflags &= ~CARRY_FLAG;
    }
}

void code_c1(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch (modrm.opcode)
    {
    case 5:
        shr_rm32_imm8(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: C1 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}