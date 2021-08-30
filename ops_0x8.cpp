#include "modrm.hpp"
#include "cpu.hpp"
#include <stdio.h>
#include <stdlib.h>

static void cmp_rm8_imm8(Pentium* cpu, ModRM* modrm)
{
    uint8_t rm8 = get_rm8(cpu, modrm);
    uint8_t imm8 = cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint16_t result = (uint16_t)rm8 - (uint16_t)imm8;
    int sign1 = rm8 >> 31;
    int sing2 = imm8 >> 31;
    int signr = (result >> 31) & 1;

    if (result >> 16)
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

void code_80(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch(modrm.opcode)
    {
    case 7:
        cmp_rm8_imm8(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 80 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}

/*
 * cmp rm32 imm8: 3 bytes
 * compares RM32 value and imm8 value by subtracting in order
 * Op code 83 and ModR/M op code 111 executes this
 * 1 byte: shared op (83)
 * 1 byte: ModR/M
 * 1 byte: imm8 to subtract
*/
static void cmp_rm32_imm8(Pentium* cpu, ModRM *modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm8 = (int32_t)cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;
    int sign1 = rm32 >> 31;
    int sing2 = imm8 >> 31;
    int signr = (result >> 31) & 1;
    printf("CMP 0x%x, 0x%x (0x%x)\n", rm32, imm8, result);

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

void sub_rm32_imm8(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm8 = (int32_t)cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint64_t result = (uint64_t)rm32 - (uint64_t)imm8;
    set_rm32(cpu, modrm, result);
    int sign1 = rm32 >> 31;
    int sing2 = imm8 >> 31;
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

static void or_rm32_imm8(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm8 = (int32_t)cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint32_t result = rm32 | imm8;
    set_rm32(cpu, modrm, result);
}

static void and_rm32_imm8(Pentium* cpu, ModRM* modrm)
{
    uint32_t rm32 = get_rm32(cpu, modrm);
    uint32_t imm8 = (int32_t)cpu->bus->read(cpu->getLinearAddr());
    cpu->ip.regs_32++;

    uint32_t result = rm32 & imm8;
    set_rm32(cpu, modrm, result);
}

void code_83(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch (modrm.opcode)
    {
    case 1:
        or_rm32_imm8(cpu, &modrm);
        break;
    case 4:
        and_rm32_imm8(cpu, &modrm);
        break;
    case 5:
        sub_rm32_imm8(cpu, &modrm);
        break;
    case 7:
        cmp_rm32_imm8(cpu, &modrm);
        break;
    default:
        printf("Not implemented: Op: 83 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}

void mov_rm8_r8(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    uint32_t r8 = cpu->gpregs[modrm.reg_index].regs_8h;
    set_rm8(cpu, &modrm, r8);
}

/*
 * mov rm32 r32: 2 bytes
 * Copies value of register specified by reg to ModR/M
 * 1 byte: op (89)
 * 1 byte: ModR/M
*/
void mov_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t r32 = cpu->gpregs[modrm.reg_index].regs_32;
    set_rm32(cpu, &modrm, r32);
}

void mov_r8_rm8(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm8 = get_rm8(cpu, &modrm);
    cpu->gpregs[rm8].regs_8h = rm8;
}

void mov_r32_rm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32 = get_rm32(cpu, &modrm);
    cpu->gpregs[modrm.reg_index].regs_32 = rm32;
}

/*
 * mov seg rm32: 2|3 bytes
 * Copies rm32 to segment register
 * 1 byte: op (8E)
 * 1|2 byte ModR/M
*/
void mov_seg_rm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32 = get_rm32(cpu, &modrm);
    printf("MOV 0x%x, 0x%x\n", rm32, modrm.reg_index);
    if (modrm.reg_index == 0)
    {
        return;
    }
    cpu->sgregs[modrm.reg_index].base = rm32;
}