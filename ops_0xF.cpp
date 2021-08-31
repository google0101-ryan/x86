#include "modrm.hpp"
#include "cpu.hpp"
#include <stdio.h>
#include <stdlib.h>

void cld(Pentium* cpu)
{
    cpu->eflags &= ~(DIRECTION_FLAG);
    cpu->ip.regs_32++;
}

static void inc_rm32(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32_val = get_rm32(cpu, modrm);
    uint64_t result = rm32_val + 1;
    set_rm32(cpu, modrm, result);
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
    default:
        printf("Not implemented: Op: FF with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}