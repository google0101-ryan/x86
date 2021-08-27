#include "cpu.hpp"
#include "modrm.hpp"
#include <stdio.h>
#include <stdlib.h>

void lgdt(ModRM* modrm, Pentium* cpu)
{
    uint32_t address = calc_memory_address(cpu, modrm);
    uint16_t limit = cpu->bus->read16(cpu->seg_to_linear(SGRegister::DS, address));
    uint32_t base = cpu->bus->read32(cpu->seg_to_linear(SGRegister::DS, address + 2));
    cpu->gdtr.size = limit;
    cpu->gdtr.offset = cpu->seg_to_linear(SGRegister::DS, base);
    printf("LGDT with size 0x%x and offset 0x%x\n", cpu->gdtr.size, cpu->gdtr.offset);
}

void lidt(ModRM* modrm, Pentium* cpu)
{
    uint32_t address = calc_memory_address(cpu, modrm);
    uint16_t limit = cpu->bus->read16(cpu->seg_to_linear(SGRegister::DS, address));
    uint32_t base = cpu->bus->read32(cpu->seg_to_linear(SGRegister::DS, address + 2));
    cpu->idtr.offset = cpu->seg_to_linear(SGRegister::DS, base);
    cpu->idtr.size = limit;
    printf("LIDT with size 0x%x and offset 0x%x\n", cpu->idtr.size, cpu->idtr.offset);
}

void code_0f_01(Pentium* cpu)
{
    cpu->ip.regs_32 += 2;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);

    switch (modrm.opcode)
    {
    case 2:
        lgdt(&modrm, cpu);
        break;
    case 3:
        lidt(&modrm, cpu);
        break;
    default:
        printf("Not implemented: Op: 0F01 with ModR/M Op: %d\n", modrm.opcode);
        exit(-1);
    }
}