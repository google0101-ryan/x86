#include "cpu.hpp"
#include <stdio.h>
#include <stdlib.h>

void jmpf_ptr16(Pentium* cpu)
{
    uint16_t ip = cpu->bus->read16(cpu->getLinearAddr());
    cpu->ip.regs_16 += 2;
    uint16_t cs = cpu->bus->read16(cpu->getLinearAddr());
    cpu->sgregs[(int)SGRegister::CS].base = cs;
    cpu->firstClock = false;
    cpu->ip.regs_16 = ip;
    printf("Jumping to 0x%x:0x%x (0x%x)\n", cs, ip, ((cs << 4) + ip));
}

void do_rm16_imm8(Pentium* cpu)
{
    cpu->raw_modrm = cpu->bus->read16(cpu->getLinearAddr());
    cpu->ip.regs_32 += 2;
    uint16_t val = cpu->read_modrm_rm16();
    uint16_t imm = static_cast<uint16_t>(cpu->bus->read16(cpu->getLinearAddr()));
    cpu->ip.regs_32 += 2;
    printf("ModRM  Mod 0x%x Reg 0x%x Mem 0x%x", cpu->modrm->mod, cpu->modrm->reg, cpu->modrm->rm);
    switch(cpu->modrm->reg)
    {
    case 0x7:
        cmp(val, imm, cpu);
    }
}

uint32_t Pentium::modrm_to_address(uint8_t mod, uint8_t rm)
{
    if (!isPE)
    {
        if (mod == 0)
        {
            switch (rm)
            {
            case 0:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) & 0xFFFF);
                break;
            case 1:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) & 0xFFFF);
                break;
            case 2:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) & 0xFFFF);
                break;
            case 3:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) & 0xFFFF);
                break;
            case 4:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::SI].regs_16));
                break;
            case 5:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::DI].regs_16));
                break;
            case 6:
                return seg_to_linear(SGRegister::DS,
                (bus->read16(getLinearAddr())));
                break;
            case 7:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16));
                break;
            }
        }
        else if (mod == 1)
        {
            switch (rm)
            {
            case 0:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 1:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 2:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 3:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 4:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 5:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 6:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            case 7:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (bus->read(getLinearAddr())) & 0xFFFF);
                break;
            }
        } else if (mod == 2)
        {
            switch (rm)
            {
                case 0:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 1:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 2:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 3:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 4:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::SI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 5:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::DI].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 6:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BP].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            case 7:
                return seg_to_linear(SGRegister::DS,
                (gpregs[(int)GPRegister16::BX].regs_16) +
                (bus->read16(getLinearAddr())) & 0xFFFF);
                break;
            }
        }
    } else
    {
        printf("Warning: Trying to decode a 32-bit address, should probably implement this\n");
    }
    return 0;
}

template <typename T>
T sub(T a, T b, Pentium* cpu)
{
    uint32_t result = a - b;
    cpu->eflags_dirty = eflags_all;
    cpu->last_add_result = a;
    cpu->last_op1 = cpu->last_result = result;
    cpu->last_op2 = b;
    cpu->last_size = sizeof(T) * 8 - 1;
    return result;
}

template uint8_t sub(uint8_t, uint8_t, Pentium*);
template uint16_t sub(uint16_t, uint16_t, Pentium*);
template uint32_t sub(uint32_t, uint32_t, Pentium*);

template <typename T>
void cmp(T a, T b, Pentium* cpu)
{
    sub(a, b, cpu);
}

template void cmp(uint8_t, uint8_t, Pentium*);
template void cmp(uint16_t, uint16_t, Pentium*);
template void cmp(uint32_t, uint32_t, Pentium*);

uint16_t Pentium::read_modrm_rm16()
{
    uint8_t mod = modrm->mod;
    uint8_t rm = modrm->rm;
    if (mod == 3)
        return gpregs[rm].regs_16;
    else
    {
        decoded_addr = modrm_to_address(mod, rm);
        bus->read16(decoded_addr);
    }
}

void Pentium::reset()
{
    for (size_t i = 0; i < num_gpregs; i++) {
        this->gpregs[i].regs_32 = 0;
    }
    for (size_t i = 0; i < num_sgregs; i++) {
        this->sgregs[i].selector = 0;
        this->sgregs[i].base = 0;
        this->sgregs[i].limit = 0;
    }
    this->ip.regs_32 = 0;
    sgregs[(int)SGRegister::CS].base = 0xFFFF0000;
    sgregs[(int)SGRegister::CS].limit = 0xFFFF;
    sgregs[(int)SGRegister::CS].selector = 0xF000;
    ip.regs_16 = 0xFFF0;
    for (int i = 0; i < 256; i++)
    {
        instrs[i] = NULL;
    }
    instrs[0xEA] = jmpf_ptr16;
    instrs[0x83] = do_rm16_imm8;
}

uint32_t Pentium::seg_to_linear(SGRegister reg, uint32_t offset)
{
    return ((sgregs[(int)reg].base) + offset);
}

uint32_t Pentium::getLinearAddr()
{
    if (!isPE && firstClock)
    {
        return ((sgregs[(int)SGRegister::CS].base) + ip.regs_16);
    }
    else if (!isPE && !firstClock)
    {
        return ((sgregs[(int)SGRegister::CS].base << 4) + ip.regs_16);
    }
}

void Pentium::write_sgreg(SGRegister reg, uint32_t value)
{
    sgregs[(int)reg].base = value;
}

void Pentium::write_ip(uint16_t new_ip)
{
    ip.regs_16 = new_ip;
}

uint16_t Pentium::get_ip()
{
    return ip.regs_16;
}

void Pentium::clock()
{
    uint8_t opcode;
    bool isPrefix = true;
    while (isPrefix)
    {
        switch ((opcode = bus->read(getLinearAddr())))
        {
            case 0x2E:
                printf("Prefix overriding to CS, 0x%x\n", sgregs[(int)SGRegister::CS].base);
                prefix.seg = (int)SGRegister::CS;
                ip.regs_32++;
                break;
            case 0x66:
                printf("Operand size override\n");
                prefix.size_override = 1;
                ip.regs_32++;
                break;
            default:
                isPrefix = false;
        }
    }
    opcode = bus->read(getLinearAddr());
    ip.regs_32++;
    if (instrs[opcode] == NULL)
    {
        printf("PANIC: Opcode 0x%x not implemented at 0x%x\n", opcode, getLinearAddr());
        exit(-1);
    }
    instrs[opcode](this);
}

