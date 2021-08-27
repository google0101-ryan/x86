#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>
#include <stdlib.h>

void mov_r32_cr(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t cr_val = cpu->cr[modrm.rm];
    cpu->gpregs[modrm.rm].regs_32 = cr_val;
    cpu->ip.regs_32++;
    printf("Mov r32, cr%d\n", modrm.rm);
}

void mov_cr_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t r32_val = cpu->gpregs[modrm.rm].regs_32;
    cpu->cr[modrm.rm] = r32_val;
}