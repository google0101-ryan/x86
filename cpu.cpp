#include "instruction_defs.hpp"
#include "cpu.hpp"
#include <stdio.h>
#include <stdlib.h>

void Pentium::update_eflags_sub(uint32_t op1, uint32_t op2, uint64_t result)
{
    int sign1 = op1 >> 31;
    int sign2 = op2 >> 31;
    int signr = (result >> 31) & 1;

    if (result >> 32)
    {
        eflags |= CARRY_FLAG;
    }
    else
    {
        eflags &= ~CARRY_FLAG;
    }
    if (result == 0)
    {
        eflags |= ZERO_FLAG;
    }
    else
    {
        eflags &= ~ZERO_FLAG;
    }
    if (signr)
    {
        eflags |= SIGN_FLAG;
    }
    else
    {
        eflags &= ~SIGN_FLAG;
    }
    if (sign1 != sign2 && sign1 != signr)
    {
        eflags |= OVERFLOW_FLAG;
    }
    else
    {
        eflags &= ~OVERFLOW_FLAG;
    }
}

uint32_t Pentium::pop32()
{
    uint32_t addr = seg_to_linear(SGRegister::DS, gpregs[(int)GPRegister32::ESP].regs_32);
    return bus->read32(addr);
}

void two_byte_instruction(Pentium* cpu)
{
    uint8_t opcode = cpu->bus->read(cpu->getLinearAddr() + 1);
    if (cpu->two_byte_instrs[opcode] == NULL)
    {
        printf("EIP: %08x Op: 0f %x not implemented.\n", cpu->ip.regs_32, opcode);
        exit(-1);
    }
    cpu->two_byte_instrs[opcode](cpu);
}

void operand_override(Pentium* cpu)
{
    cpu->ip.regs_32++;
    uint8_t op = cpu->bus->read(cpu->getLinearAddr());
    printf("Opcode 0x66 0x%x\n", op);
    if (cpu->isPE)
    {

    }
    else
    {
        if (op >= 0xB8 && op <= 0xBF)
        {
            mov_r32_imm32(cpu);
            return;
        }
    switch (op)
    {
    case 0x83:
        code_83(cpu);
    }
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
        two_byte_instrs[i] = NULL;
    }
    instrs[0x0C] = or_al_imm8;
    instrs[0x0F] = two_byte_instruction;
    instrs[0xEA] = ptr_jump;
    instrs[0xE4] = in_al_imm8;
    instrs[0xE6] = out_imm8_al;
    instrs[0xE9] = near_jump;
    instrs[0x59] = pop_r32;
    instrs[0x66] = operand_override;
    instrs[0x8E] = mov_seg_rm32;
    instrs[0x81] = code_81;
    instrs[0x89] = mov_rm32_r32;
    for (int i = 0; i < 8; i++)
    {
        instrs[0xB8 + i] = mov_r32_imm32;
    }
    two_byte_instrs[0x01] = code_0f_01;
    two_byte_instrs[0x20] = mov_r32_cr;
    two_byte_instrs[0x22] = mov_cr_r32;
    two_byte_instrs[0x85] = jnz32;
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
    printf("0x%x\n", getLinearAddr());
    opcode = bus->read(getLinearAddr());
    printf("0x%x\n", opcode);
    
    if (opcode == 0x2E)
    {
        ip.regs_32++;
        opcode = bus->read(getLinearAddr());
    }
    if (instrs[opcode] == NULL)
    {
        printf("PANIC: Opcode 0x%x not implemented at 0x%x\n", opcode, getLinearAddr());
        exit(-1);
    }
    instrs[opcode](this);
}

