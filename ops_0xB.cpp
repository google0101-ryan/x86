#include "modrm.hpp"
#include "hw/cpu.hpp"

void mov_r8_imm8(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0xB0;
    if (reg < 4)
        cpu->gpregs[reg].regs_8l = cpu->bus->read(cpu->getLinearAddr() + 1);
    else
        cpu->gpregs[reg].regs_8h = cpu->bus->read(cpu->getLinearAddr() + 1);
    cpu->ip.regs_32 += 2;
}

void mov_r32_imm32(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0xB8;
    uint32_t value= cpu->bus->read32(cpu->getLinearAddr() + 1);
    cpu->gpregs[reg].regs_32 = value;
    cpu->ip.regs_32 += 5;
}

void mov_r16_imm16(Pentium* cpu)
{
    uint8_t reg = cpu->bus->read(cpu->getLinearAddr()) - 0xB8;
    uint16_t value = cpu->bus->read16(cpu->getLinearAddr() + 1);
    cpu->gpregs[reg].regs_16 = value;
    cpu->ip.regs_32 += 3;
}