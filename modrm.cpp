#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "modrm.hpp"

ModRM create_modrm()
{
    ModRM modrm;
    SIB sib = {};
    modrm.sib = sib;
    return modrm;
}

void parse_modrm(ModRM* modrm, Pentium* cpu)
{
    uint8_t code;

    code = cpu->bus->read(cpu->getLinearAddr());

    modrm->mod = ((code & 0xC0) >> 6);
    modrm->opcode = ((code & 0x38) >> 3);
    modrm->rm = code & 0x07;

    cpu->ip.regs_32++;

    if (!cpu->isPE)
    {
        if (modrm->mod == 0 && modrm->rm == 6)
        {
            modrm->disp16 = cpu->bus->read16(cpu->getLinearAddr());
            cpu->ip.regs_32 += 2;
            return;
        }
    }

    if (modrm->mod != 3 && modrm->rm == 4)
    {
        uint8_t sib_byte = cpu->bus->read(cpu->getLinearAddr());
        modrm->sib.scale = ((sib_byte & 0xC0) >> 6);
        modrm->sib.index = ((sib_byte & 0x38) >> 3);
        modrm->sib.base = sib_byte & 0x07;
        cpu->ip.regs_32++;
    }

    if ((modrm->mod == 0 && modrm->rm == 5) || (modrm->mod == 0 && modrm->sib.base == 5) || modrm->mod == 2)
    {
        modrm->disp32 = cpu->bus->read32(cpu->getLinearAddr());
        cpu->ip.regs_32 += 4;
    }
    else if (modrm->mod == 1)
    {
        modrm->disp8 = cpu->bus->read(cpu->getLinearAddr());
        cpu->ip.regs_32++;
    }
}

uint32_t calc_cib_address(Pentium* cpu, ModRM* modrm)
{
    uint32_t base_val = cpu->gpregs[modrm->sib.base].regs_32;
    uint32_t scale = pow(2, modrm->sib.scale);
    uint32_t index_val = cpu->gpregs[modrm->sib.index].regs_32;

    printf("modrm sib mod %d index: %d base %d\n", modrm->mod, modrm->sib.index, modrm->sib.base);

    if (modrm->mod == 0)
    {
        if (modrm->sib.index == 4 && modrm->sib.base == 5)
            return modrm->disp32;
        if (modrm->sib.index == 4)
            return base_val;
        if (modrm->sib.base == 5)
            return (index_val * scale) + modrm->disp32;
        return index_val * scale + base_val;
    }
    if (modrm->mod == 1)
    {
        if (modrm->sib.index == 4)
            return base_val + modrm->disp8;
        return base_val + (index_val * scale) + modrm->disp8;
    }
    if (modrm->mod == 2)
    {
        if (modrm->sib.index == 4)
            return base_val + modrm->disp32;
        return base_val + (index_val * scale) + modrm->disp32;
    }

    printf("Invalid Mod value for SIB");
    exit(-1);
}

uint32_t calc_memory_address(Pentium* cpu, ModRM* modrm)
{
    if (!cpu->isPE)
    {
        if (modrm->mod == 0 && modrm->rm == 6)
        {
            return modrm->disp16;
        }
    }
    /* Mod:0 [reg] */
    if (modrm->mod == 0)
    {
        /* SIB (Mod: 00 R/M: 100) */
        if (modrm->rm == 4)
        {
            return calc_cib_address(cpu, modrm);
        }
        /* Mod:0 R/M: 101 uses disp32 as memory address. */
        else if (modrm->rm == 5)
        {
            return modrm->disp32;
        }
        else
        {
            return cpu->gpregs[modrm->rm].regs_32;
        }
    }
    else if (modrm->mod == 1)
    {
        /* SIB (Mod: 01 R/M: 100) */
        if (modrm->rm == 4)
        {
            return calc_cib_address(cpu, modrm);
        }
        else
        {
            return cpu->gpregs[modrm->rm].regs_32 + modrm->disp8;
        }
    }
    else if (modrm->mod == 2)
    {
        /* SIB (MOD: 10 R/M: 100) */
        if (modrm->rm == 4)
        {
            return calc_cib_address(cpu, modrm);
        }
        /* Mod: 10 R/M: 000 - 011, 101 - 111 uses [reg] + disp32 */
        else
        {
            return cpu->gpregs[modrm->rm].regs_32 + modrm->disp32;
        }
    }
    else
    {
        printf("ModRM with mod: 00 - 10 are only implemented.");
        exit(-1);
    }
}

uint32_t get_rm32(Pentium* cpu, ModRM* modrm)
{
    if (modrm->mod == 3)
    {
        return cpu->gpregs[modrm->rm].regs_32;
    }
    else
    {
        uint32_t address = calc_memory_address(cpu, modrm);
        return cpu->bus->read32(cpu->seg_to_linear(SGRegister::DS, address));
    }
}

void set_rm32(Pentium *cpu, ModRM* modrm, uint32_t value)
{
    if (modrm->mod == 3)
    {
        cpu->gpregs[modrm->rm].regs_32 = value;
    }
    else
    {
        uint32_t address = calc_memory_address(cpu, modrm);
        cpu->bus->write32(cpu->seg_to_linear(SGRegister::DS, address), value);
    }
}