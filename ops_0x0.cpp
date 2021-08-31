#include "modrm.hpp"
#include "cpu.hpp"
#include <stdio.h>

void add_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32 += 1;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32;
    uint64_t result = (uint64_t)rm32_val + (uint64_t)r32_val;
    set_rm32(cpu, &modrm, result);
}

void add_al_imm8(Pentium* cpu)
{
    uint8_t imm8_val = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    uint16_t result = (uint16_t)al_val + (uint16_t)imm8_val;
    cpu->gpregs[(int)GPRegister8::AL].regs_8l = result;
    cpu->ip.regs_32 += 2;
}

void push_es(Pentium* cpu)
{
    cpu->push32(cpu->sgregs[(int)SGRegister::ES].base);
    cpu->ip.regs_32++;
    printf("PUSH ES (0x%x)\n", cpu->sgregs[(int)SGRegister::ES].base);
}

void pop_es(Pentium* cpu)
{
    cpu->sgregs[(int)SGRegister::ES].base = cpu->pop32();
    cpu->ip.regs_32++;
}

void or_rm16_r16(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint16_t rm16_val = get_rm16(cpu, &modrm);
    uint16_t r16_val = cpu->gpregs[modrm.reg_index].regs_16;
    uint16_t result = rm16_val | r16_val;
    set_rm16(cpu, &modrm, result);
}