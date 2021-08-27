#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>
#include <stdlib.h>

void mov_seg_rm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32 = get_rm32(cpu, &modrm);
    cpu->sgregs[modrm.reg_index].base = rm32;
}

void mov_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t r32 = cpu->gpregs[modrm.reg_index].regs_32;
    set_rm32(cpu, &modrm, r32);
}

void or_rm32_imm32(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm32 = cpu->bus->read32(cpu->getLinearAddr());
    cpu->ip.regs_32 += 4;

    uint32_t result = rm32 | imm32;
    set_rm32(cpu, modrm, result);
}

void code_81(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch (modrm.opcode)
    {
    case 1:
        or_rm32_imm32(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 81 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}

void cmp_rm32_imm8(Pentium* cpu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm8 = (int32_t)cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;
    cpu->update_eflags_sub(rm32, imm8, result);
    printf("CMP 0x%x, 0x%x\n", rm32, imm8);
}

void code_83(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();

    parse_modrm(&modrm, cpu);

    switch (modrm.opcode)   
    {
    case 7:
        cmp_rm32_imm8(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 83 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
        break;
    }
}