#include "CPU.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

// TODO: Add optional debug tracing
// TODO: Move all utility functions into an auxillary file
// TODO: Lots of OPCODES! Also, I probably need to figure out the ModR/M format, this hacky stuff isn't
// Going to work forever (probably)

bool getParity(unsigned int n)
{
    bool parity = 0;
    while (n)
    {
        parity = !parity;
        n = n & (n - 1);
    }
    return parity;
}

void CPU::SetCarry(int is_carry)
{
    if (is_carry)
    {
        eflags->CF = 1;
    } else
    {
        eflags->CF = 0;
    }
}

void CPU::SetOverflow(int is_overflow)
{
    if (is_overflow)
    {
        eflags->OF = 1;
    } else
    {
        eflags->OF = 0;
    }
}

void CPU::updateEflags(uint8_t value1, uint8_t value2, uint16_t result)
{
    int sign1 = value1 >> 7;
    int sign2 = value2 >> 7;
    int signr = (result >> 7) & 1;
    //SetCarry(result != value1 + value2);
    SetOverflow(sign1 == sign2 && sign1 != signr);
}

void CPU::printflags()
{
    printf("FLAGS:\n");
    printf("CF: %d ", eflags->CF);
    printf("PF: %d ", eflags->PF);
    printf("AF: %d ", eflags->AF);
    printf("ZF: %d ", eflags->ZF);
    printf("SF: %d ", eflags->SF);
    printf("TF: %d ", eflags->TF);
    printf("IF: %d ", eflags->IF);
    printf("DF: %d ", eflags->DF);
    printf("OF: %d ", eflags->OF);
    printf("IOPL: %d ", eflags->IOPL);
    printf("NT: %d\n", eflags->NT);
    
}

uint8_t CPU::getop(uint8_t opcode)
{
    if (opcode == 0x66)
    {
        prefix = 0x66;
        return getop(ram->read(physaddr(eip++, cs)));
    }
    if (opcode != 0x04 && opcode != 0xCD && opcode != 0xF0 && 
    opcode != 0xAC && opcode != 0xF4 && opcode != 0x8b && opcode != 0xe9 &&
    opcode != 0x3C && opcode != 0x74 && opcode != 0xeb)
    {
        return (opcode & 0xF0);
    } else {
        return opcode;
    }
}

uint32_t CPU::physaddr(uint32_t offset, uint16_t seg)
{
    uint32_t p_base = ((uint32_t)seg) << 4;
    return p_base + offset;
}

CPU::CPU(Memory* mem)
{
    this->ram = mem;
    eflags = (flags*)malloc(sizeof(eflags));
    Reset();
}

void CPU::Reset()
{
    ax.reg = bx.reg = cx.reg = dx.reg;
    cs = ds = es = fs = gs = ss = 0;
    cr0 = cr1 = cr2 = cr3 = cr4 = 0;
    idtr.base = gdtr.base = ldtr.base = 0;
    idtr.limit = gdtr.limit = ldtr.base = 0;
    memset(eflags, 0, sizeof(eflags));
    eflags->IF = 1;
    eflags->reserved = 1;
    eflags->reserved2 = 1;
    eflags->reserved3 = 1;
    eflags->reserved4 = 0;
    eip = 0x7c00;
    proted = false;
    isEqual = false;
}

void CPU::Execute()
{
    while (!halted)
    {
        Clock();
    }
}

void CPU::Clock()
{
    uint8_t opcode = ram->read(eip++);
    Execute(opcode);
}

void CPU::Execute(uint8_t opcode)
{
    uint8_t op = getop(opcode);
    //printf("Opcode: 0x%x\n", opcode);
    //printf("Op: 0x%x\n", op);
    if (opcode == 0x0f)
    {
        op = ram->read(physaddr(eip++, cs));
    }
    switch(op)
    {
    case 0x04:
    {
        uint8_t data = ram->read(eip++);
        uint8_t r8 = ax.l;
        uint8_t rm8 = data;
        uint16_t result = (uint16_t)r8 + (uint16_t)rm8;
        ax.l = result;
        updateEflags(r8, rm8, result);
        if (ax.l != result)
        {
            SetCarry(1); // Really hacky, but it works!
        }
        if (getParity(ax.l))
        {
            eflags->PF = 1;
        }
        if (ax.l == 0)
        {
            eflags->ZF = 1;
        }
    }
    break;
    case 0xb0: // MOV
    if (opcode < 0xB8 && prefix != 0x66)
    {
        mov_r8_imm(opcode);
    } else if (prefix == 0x66) {
        mov_r32_imm(op);
    } else {
        mov_r16_imm(opcode);
    }
        break;
    case 0xCD: // INT
        int_imm8();
        break;
    case 0xF0: // CLI
        eflags->IF = 0;
        break;
    case 0xAC: // LODSB
        ax.l = ram->read(si.i);
        si.i++;
        break;
    case 0xF4: // HLT
        //printf("HLT\n");
        halted = true;
        break;
    case 0x8b:
    {
        if ((ram->read(physaddr(eip++, cs))) == 0x36)
        {
            mov_r16_imm(0xEE);
        }
        break;
    }
    case 0xe9:
    {
        int32_t rel = ram->read(physaddr(eip++, cs));
        rel |= ram->read(physaddr(eip++, cs)) << 8;
        rel |= ram->read(physaddr(eip++, cs)) << 16;
        rel |= ram->read(physaddr(eip++, cs)) << 24;
        eip += (rel + 5);
        break;
    }
    case 0xeb:
    {
        int8_t rel = ram->read(physaddr(eip++, cs));
        eip += rel;
        break;
    }
    case 0x3C:
    {
        if (ax.l == ram->read(physaddr(eip++, cs)))
        {
            isEqual = true;
        }
        break;
    }
    case 0x74:
    {
        uint8_t rel = ram->read(physaddr(eip++, cs));
        if (isEqual)
        {
            eip += rel;
        }
        break;
    }
    default:
        //printf("UNKNOWN OPCODE: 0x%x\n", op);
        break;
    }
}

// MOV r8, imm8
void CPU::mov_r8_imm(uint8_t opcode)
{
    uint8_t data = ram->read(eip++);
    uint8_t reg = opcode - 0xB0;
    switch(reg)
    {
    case 0x4:
        ax.h = data;
        //printf("MOV AH, 0x%02x\n", data);
        break;
    case 0xE:
        si.i = data;
        break;
    case 0x0:
        ax.l = data;
        //printf("MOV AL, 0x%02x\n", data);
        break;
    }
}

void CPU::mov_r16_imm(uint8_t opcode)
{
    uint8_t reg = opcode - 0xB8;
    switch(reg)
    {
    case 0x4:
    {
        uint16_t data = ram->read(physaddr(eip++, cs));
        data |= ram->read(physaddr(eip++, cs)) << 8;
        sp.ei = data;
        //printf("MOV SP, 0x%04x\n", data);
    }
    break;
    case 0x36:
    {
        uint16_t data = ram->read(physaddr(eip++, cs));
        data |= ram->read(physaddr(eip++, cs)) << 8;
        si.i = data;
    }
        break;
    }
}

void CPU::mov_r32_imm(uint8_t opcode)
{
    uint8_t reg = opcode - 0xB0;
    //printf("MOV CODE 0x%x\n", opcode);
    uint32_t data;
    data = ram->read(physaddr(eip++, cs));
    data |= ram->read(physaddr(eip++, cs)) << 8;
    data |= ram->read(physaddr(eip++, cs)) << 16;
    data |= ram->read(physaddr(eip++, cs)) << 24;
    //printf("DATA: 0x%x\n", data);
    switch (reg)
    {
    case 0x0:
        ax.reg = data;
        break;
    default:
        printf("UNKNOWN REGCODE 0x%x\n", reg);
        break;
    }
}

// Interrupts
void CPU::int_imm8()
{
    uint8_t interrupt = ram->read(eip++);
    //printf("INT 0x%x\n", interrupt);
    if (!proted && eflags->IF == 1)
    {
        eflags->IF = 0; // Make other interrupts wait
        switch (interrupt)
        {
        case 0x10:
            if (ax.h == 0x0e)
            {
                printf("%c", ax.l);
            }
        }
        eflags->IF = 1;
    }
}

void CPU::Dump()
{
    // We add a newline in front to avoid conflicting with any int 0x10 characters
    printf("\n\nAL: 0x%02x AH: 0x%02x AX: 0x%04x EAX: 0x%04x\n", ax.l, ax.h, ax.hl, ax.reg);
    printf("BL: 0x%02x BH: 0x%02x BX: 0x%04x EBX: 0x%04x\n", bx.l, bx.h, bx.hl, bx.reg);
    printf("CL: 0x%02x CH: 0x%02x CX: 0x%04x ECX: 0x%04x\n", cx.l, cx.h, cx.hl, cx.reg);
    printf("SI: 0x%02x ESI: 0x%02x\n", si.i, si.ei);
    printf("ESP: 0x%x\n", sp.ei);
    printf("EIP: 0x%x\n", eip);
    printflags();
}