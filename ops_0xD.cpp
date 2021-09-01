#include "hw/cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>

void code_dd(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    printf("Unimplemented FPU Opcode DD with opcode %d\n", modrm.opcode);
}