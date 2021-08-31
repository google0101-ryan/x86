#include "cpu.hpp"
#include "modrm.hpp"

void stosb(Pentium* cpu)
{
    uint8_t al_val = cpu->gpregs[(int)GPRegister8::AL].regs_8l;
    uint32_t edi_val = cpu->gpregs[(int)GPRegister32::EDI].regs_32;
    cpu->bus->write(cpu->seg_to_linear(SGRegister::ES, edi_val), al_val);
    if (cpu->eflags & DIRECTION_FLAG)
    {
        cpu->gpregs[(int)GPRegister32::EDI].regs_32 = edi_val - 1;
    }
    else
    {
        cpu->gpregs[(int)GPRegister32::EDI].regs_32 = edi_val + 1;
    }
    cpu->ip.regs_32++;
}

void movsb(Pentium* cpu)
{
    uint32_t esi_val = cpu->gpregs[(int)GPRegister32::ESI].regs_32;
    uint32_t edi_val = cpu->gpregs[(int)GPRegister32::EDI].regs_32;
    uint8_t ds_esi_val = cpu->bus->read(cpu->seg_to_linear(SGRegister::DS, esi_val));
    cpu->bus->write(cpu->seg_to_linear(SGRegister::ES, edi_val), ds_esi_val);
    if (cpu->eflags & DIRECTION_FLAG)
    {
        cpu->gpregs[(int)GPRegister32::EDI].regs_32 = edi_val - 1;
    }
    else
    {
        cpu->gpregs[(int)GPRegister32::EDI].regs_32 = edi_val + 1;
    }
    if (cpu->eflags & DIRECTION_FLAG)
    {
        cpu->gpregs[(int)GPRegister32::ESI].regs_32 = esi_val - 1;
    }
    else
    {
        cpu->gpregs[(int)GPRegister32::ESI].regs_32 = esi_val + 1;
    }
    cpu->ip.regs_32++;
}

void mov_moffs16_ax(Pentium* cpu)
{
    uint32_t offset = cpu->bus->read32(cpu->getLinearAddr() + 1);
    uint16_t ax_val = cpu->gpregs[(int)GPRegister16::AX].regs_16;
    cpu->bus->write16(cpu->seg_to_linear(SGRegister::DS, offset), ax_val);
    cpu->ip.regs_32 += 5;
}