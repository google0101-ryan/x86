#include "modrm.hpp"
#include "hw/cpu.hpp"
#include <stdio.h>
#include <stdlib.h>

void cld(Pentium* cpu)
{
    cpu->eflags &= ~(DIRECTION_FLAG);
    cpu->ip.regs_32++;
}

void cli(Pentium* cpu)
{
    cpu->eflags &= ~(1 << 9);
    cpu->ip.regs_32++;
}

void code_fe(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch(modrm.opcode)
    {
    default:
        printf("Not implemented: Op: FE with ModR/M Op: %d\n", modrm.opcode);
        break;
    }
}

static void inc_rm32(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32_val = get_rm32(cpu, modrm);
    uint64_t result = rm32_val + 1;
    set_rm32(cpu, modrm, result);
}

static void jmp_rm32(Pentium* cpu, ModRM* modrm)
{
    int32_t address = get_rm32(cpu, modrm);
    cpu->ip.regs_32 = address;
    printf("JMPRM32 0x%x", cpu->getLinearAddr());
}

void mul_rm8(Pentium* cpu, ModRM* modrm)
{
    uint8_t rm8_val = get_rm8(cpu, modrm);
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    uint16_t result = (uint16_t)al_val * (uint16_t)rm8_val;
    uint8_t res_upper_half = result >> 8;
    uint8_t res_lower_half = (uint8_t)result;
    cpu->gpregs[(int)GPRegister8::AH].regs_8h = res_upper_half;
    cpu->gpregs[(int)GPRegister8::AL].regs_8l = res_lower_half;
}

void code_f6(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch(modrm.opcode)
    {
    case 4:
        mul_rm8(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: F6 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}

void code_ff(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch(modrm.opcode)
    {
    case 0:
        inc_rm32(cpu, &modrm);
        break;
    case 4:
        jmp_rm32(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: FF with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}