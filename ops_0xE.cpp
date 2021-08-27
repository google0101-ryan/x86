#include "cpu.hpp"
#include <stdio.h>

void near_jump(Pentium* cpu)
{
    int32_t diff = cpu->bus->read32(cpu->getLinearAddr() + 1);
    cpu->ip.regs_32 += (diff + 5);
    printf("Jumping to 0x%x\n", cpu->getLinearAddr());
}

void out_imm8_al(Pentium* cpu)
{
    uint16_t address = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    cpu->iobus->out8(address, al_val);
    printf("Out 0x%x, 0x%x\n", address, al_val);
    cpu->ip.regs_32 += 2;
}

void in_al_imm8(Pentium* cpu)
{
    uint16_t address = cpu->bus->read(cpu->getLinearAddr() + 1);
    uint8_t value = cpu->iobus->in8(address);
    printf("IN AL, 0x%x\n", address);
    cpu->ip.regs_32 += 2;
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void check_prot_mode(Pentium* cpu)
{
    if (cpu->isPE)
        return;
    uint8_t cr0_pe = (cpu->cr[0] & (1));
    printf("cr0: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(cpu->cr[0]));
    uint16_t gdt_index = (cpu->sgregs[(int)SGRegister::CS].base >> 3);
    uint16_t gdt_entry_count = (cpu->gdtr.size + 1) / 8;
    printf("cr0_pe: %d gdt_index: %d gdt_entry_count %d\n", cr0_pe, gdt_index, gdt_entry_count);
    if (cr0_pe && gdt_index != 0 && gdt_entry_count > gdt_index)
        cpu->isPE = 1;
}

void ptr_jump(Pentium* cpu)
{
    uint32_t eip_val;
    uint16_t cs_val;
    cpu->ip.regs_32++;
    if (cpu->isPE)
    {
        eip_val = cpu->bus->read16(cpu->getLinearAddr()) | (cpu->bus->read16(cpu->getLinearAddr() + 2) << 16);
        cpu->ip.regs_32 += 4;
        cs_val = cpu->bus->read16(cpu->getLinearAddr());
    }
    else
    {
        eip_val = (uint32_t)cpu->bus->read16(cpu->getLinearAddr());
        cpu->ip.regs_32 += 2;
        cs_val = cpu->bus->read16(cpu->getLinearAddr());
    }
    cpu->sgregs[(int)SGRegister::CS].base = cs_val;
    cpu->ip.regs_32 = eip_val;
    cpu->firstClock = false;
    check_prot_mode(cpu);
    printf("%s\n", cpu->isPE ? "true" : "false");
    printf("Jumping to 0x%x:0x%x (0x%x)\n", cs_val, eip_val, cpu->getLinearAddr());
}