#include "modrm.hpp"
#include "cpu.hpp"
#include <stdio.h>

void out_imm8_al(Pentium* cpu)
{
    uint16_t address = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    cpu->iobus->out8(address, al_val);
    cpu->ip.regs_32 += 2;
    printf("OUT 0x%x, AL\n", address);
}

void jecxz(Pentium* cpu)
{
    uint32_t ecx_val = cpu->gpregs[(int)GPRegister32::ECX].regs_32;
    if (ecx_val == 0)
    {
        int8_t offset = cpu->bus->read(cpu->getLinearAddr() + 1);
        cpu->ip.regs_32 += (offset + 2);
    }
    else
    {
        cpu->ip.regs_32 += 2;
    }
}

void in_al_imm8(Pentium* cpu)
{
    uint16_t address = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t value = cpu->iobus->in8(address);
    cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    cpu->ip.regs_32 += 2;
}

void short_jump(Pentium* cpu)
{
    int8_t offset = cpu->bus->read(cpu->getLinearAddr() + 1);
    cpu->ip.regs_32 += (offset + 2);
}

/*
 * jmp ptr16:16/32: 7 bytes
 * jumps in ptr16:16/32.
 * 1 byte: op (EA)
 * 2 byte: cs value
 * 2 byte: eip
*/
void ptr_jump(Pentium* cpu)
{
    uint32_t eip_val;
    uint16_t cs_val;
    if (cpu->isPE)
    {
        eip_val = cpu->bus->read32(cpu->getLinearAddr() + 1);
        cs_val = cpu->bus->read16(cpu->getLinearAddr() + 5);
    }
    else
    {
        eip_val = (uint32_t)cpu->bus->read16(cpu->getLinearAddr() + 1);
        cs_val = cpu->bus->read16(cpu->getLinearAddr() + 3);
    }
    cpu->sgregs[(int)SGRegister::CS].base = cs_val;
    cpu->ip.regs_32 = eip_val;
    cpu->firstClock = false;
}