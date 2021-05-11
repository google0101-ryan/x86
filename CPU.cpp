#include "CPU.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

// TODO: Add optional debug tracing
// TODO: Move all utility functions into an auxillary file
// TODO: Lots of OPCODES! Also, I probably need to figure out the ModR/M format, this hacky stuff isn't
// Going to work forever (probably)

uint32_t read_gdt_entry_base(uint32_t entry1, uint32_t entry2)
{
    uint32_t base_low = (entry1 >> 16);
    uint32_t base_mid = (entry2 & 0xFF);
    uint32_t base_high = (entry2 >> 24);
    return base_low | (base_mid << 16) | (base_high << 24);
}

void check_entry_access(uint8_t access_byte, uint8_t cpl, uint8_t write, uint8_t exec)
{
    if (write)
    {
        uint8_t rw = access_byte & 2;
        if (!rw)
        {
            printf("GDT ACCESS ERROR: not writable\n");
            exit(1);
        }
    }
    if (exec)
    {
        uint8_t d_exec = access_byte & 8;
        if (!d_exec)
        {
            printf("GDT ACCESS ERROR: not executable\n");
            exit(1);
        }
    }
    uint8_t dpl = (access_byte >> 5) & 3;
    if (dpl < cpl)
    {
        printf("GDT ACCESS ERROR: entry privelege not met\n");
        exit(1);
    }
}

void check_entry_limit(uint32_t entry1, uint32_t entry2, uint32_t offset)
{
    uint32_t limit_low = entry1 & 0xFFFF;
    uint32_t limit_high = (entry2 >> 16) & 0xF;
    uint32_t limit = limit_low | (limit_high << 16);

    uint8_t flags = (entry2 >> 20) & 0xF;
    uint8_t g = flags & 8;
    if (g)
    {
        uint64_t limit_val = limit * (2 ^ 12);
        if ((uint64_t)offset > limit_val)
        {
            printf("GDT ACCESS ERROR: beyond entry limit\n");
            exit(1);
        }
    }
    else
    {
        if (offset > limit)
        {
            printf("GDT ACCESS ERROR: beyond entry limit\n");
            exit(1);
        }
    }
}

uint32_t CPU::getlinearaddr(uint16_t seg, uint32_t offset, uint8_t write, uint8_t exec)
{
    int entry_index = seg >> 3;
    uint8_t ti = seg & (1 << 2);
    uint8_t cpl = seg & 3;
    if (ti)
    {
        return 0;
    }
    else
    {
        uint32_t entry_addr = gdtr.base + (entry_index * 8);
        if (entry_addr > (gdtr.base + gdtr.limit + 1))
        {
            printf("GDT ACCESS ERROR: beyond table limit\n");
            exit(1);
        }
        uint32_t entry1 = ram->read(entry_addr++);
        entry1 |= ram->read(entry_addr++) << 8;
        entry1 |= ram->read(entry_addr++) << 16;
        entry1 |= ram->read(entry_addr++) << 24;
        uint32_t entry2 = ram->read(entry_addr++);
        entry2 |= ram->read(entry_addr++) << 8;
        entry2 |= ram->read(entry_addr++) << 16;
        entry2 |= ram->read(entry_addr++) << 24;
        uint8_t access_byte = (entry2 >> 8) & 0xFF;
        check_entry_access(access_byte, cpl, write, exec);

        uint32_t base = read_gdt_entry_base(entry1, entry2);
        check_entry_limit(entry1, entry2, offset);

        return base + offset;
    }
}

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
        return getop(ram->read(physaddr(eip++, cs, 0)));
    }
    if (opcode != 0x04 && opcode != 0xCD && opcode != 0xF0 && 
    opcode != 0xAC && opcode != 0xF4 && opcode != 0x8b && opcode != 0xe9 &&
    opcode != 0x3C && opcode != 0x74 && opcode != 0xeb && opcode != 0x01 &&
    opcode != 0x83 && opcode != 0xEA && opcode != 0x4f && opcode != 0xE8 &&
    opcode != 0xC3)
    {
        return (opcode & 0xF0);
    } else {
        return opcode;
    }
}

uint32_t CPU::physaddr(uint32_t offset, uint16_t seg, uint8_t write)
{
    uint8_t exec = 0;
    if (seg == cs)
    {
        exec = 1;
    }
    if (proted)
    {
        uint32_t linear = getlinearaddr(seg, offset, write, exec);
        return linear;
    }
    uint32_t p_base = ((uint32_t)seg) << 4;
    return p_base + offset;
}

CPU::CPU(Memory* mem, bool debug)
{
    this->ram = mem;
    eflags = (flags*)malloc(sizeof(eflags));
    this->debug = debug;
    Reset();
}

void CPU::Reset()
{
    ax.reg = bx.reg = cx.reg = dx.reg;
    cs = ds = es = fs = gs = ss = 0;
    cr0 = cr1 = cr2 = cr3 = cr4 = 0;
    idtr.base = gdtr.base = ldtr.base = 0;
    idtr.limit = gdtr.limit = ldtr.base = 0;
    memset(eflags, 0, sizeof(*eflags));
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
        op = ram->read(physaddr(eip++, cs, 0));
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
        if (debug)
        printf("ADD AL, 0x%x\n", data);
    }
    break;
    case 0xb0: // MOV
    if (!proted)
    {
        if (opcode < 0xB8 && prefix != 0x66)
        {
            mov_r8_imm(opcode);
        } else if (prefix == 0x66) {
            mov_r32_imm(op);
        } else {
            mov_r16_imm(opcode);
        }
    } else {
        if (opcode < 0xB8 && prefix != 0x66)
        {
            mov_r8_imm(opcode);
        } else if (prefix == 0x66) {
            mov_r16_imm(op);
        } else {
            mov_r32_imm(opcode);
        }
    }
        break;
    case 0xCD: // INT
        int_imm8();
        break;
    case 0xF0: // CLI
        eflags->IF = 0;
        if (debug)
        printf("CLI\n");
        break;
    case 0xAC: // LODSB
        ax.l = ram->read(si.i);
        si.i++;
        if (debug)
        printf("LODSB\n");
        break;
    case 0xF4: // HLT
        if(debug)
        printf("HLT\n");
        halted = true;
        break;
    case 0x8b:
    {
        if ((ram->read(physaddr(eip++, cs, 0))) == 0x36)
        {
            mov_r16_imm(0xEE);
        }
        break;
    }
    case 0xe9:
    {
        int32_t rel = ram->read(physaddr(eip++, cs, 0));
        rel |= ram->read(physaddr(eip++, cs, 0)) << 8;
        rel |= ram->read(physaddr(eip++, cs, 0)) << 16;
        rel |= ram->read(physaddr(eip++, cs, 0)) << 24;
        eip += (rel + 5);
        break;
    }
    case 0xeb:
    {
        int8_t rel = ram->read(physaddr(eip++, cs, 0));
        eip += rel;
        if (debug)
        printf("JMP 0x%02x\n", eip);
        break;
    }
    case 0x3C:
    {
        uint8_t data = ram->read(physaddr(eip++, cs, 0));
        if (ax.l == data)
        {
            isEqual = true;
        }
        if (debug)
        printf("CMP AL, 0x%x\n", data);
        break;
    }
    case 0x74:
    {
        uint8_t rel = ram->read(physaddr(eip++, cs, 0));
        if (isEqual)
        {
            eip += rel;
        }
        if (debug)
        printf("JE 0x%02x\n", eip);
        break;
    }
    case 0x1:
    {
        uint8_t data = ram->read(physaddr(eip++, cs, 0));
        //printf("DATA: 0x%x", data);
        if (data == 0x16)
        {
            uint16_t address = ram->read(physaddr(eip++, cs, 0));
            address |= ram->read(physaddr(eip++, cs, 0)) << 8;
            uint16_t limit = ram->read(physaddr(address, ds, 0));
            limit |= ram->read(physaddr(address + 1, ds, 0)) << 8;
            gdtr.limit = limit;
            uint32_t base = ram->read(physaddr(address + 2, ds, 0));
            base |= ram->read(physaddr(address + 3, ds, 0)) << 8;
            base |= ram->read(physaddr(address + 4, ds, 0)) << 16;
            base |= ram->read(physaddr(address + 5, ds, 0)) << 24;
            gdtr.base = physaddr(base, ds, 0);
            if (debug)
            printf("LGDT 0x%02x\n", address);
        }
        break;
    }
    case 0x20:
    {
        uint8_t data = ram->read(physaddr(eip++, cs, 0));
        if (data == 0xc0)
        {
            ax.reg = cr0;
        }
        if (debug)
            printf("MOV AX, CR0\n");
        break;
    }
    case 0x22:
    {
        uint8_t reg = ram->read(physaddr(eip++, cs, 0));
        if (reg == 0xc0)
        {
            cr0 = ax.reg;
        }
        if (debug)
            printf("MOV CR0, AX\n");
        break;
    }
    case 0x83:
    {
        uint8_t reg = ram->read(physaddr(eip++, cs, 0));
        uint8_t data = ram->read(physaddr(eip++, cs, 0));
        if (reg == 0xc8)
        {
            ax.reg |= data;
        }
        if (debug)
        printf("OR AX, 0x%x\n", data);
        break;
    }
    case 0xEA:
    {
        uint32_t eip_val;
        uint16_t cs_val;
        if (proted)
        {
            uint16_t tmp = ram->read(physaddr(eip++, cs, 0));
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 8;
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 16;
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 24;
            eip = tmp;
            tmp = ram->read(physaddr(eip++, cs, 0));
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 8;
            cs_val = tmp;
        }
        else
        {
            uint32_t tmp = ram->read(physaddr(eip++, cs, 0));
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 8;
            eip_val = tmp;
            tmp = ram->read(physaddr(eip++, cs, 0));
            tmp |= ram->read(physaddr(eip++, cs, 0)) << 8;
            cs_val = tmp;
        }
        cs = cs_val;
        eip = eip_val;
        if (debug)
        printf("LJMP 0x%02x\n", eip_val);
        if (proted)
            break;
        uint8_t CR0_PE = cr0 & 1;
        uint16_t gdt_index = cs >> 3;
        uint16_t gdt_entry_count = (gdtr.limit + 1) / 8;
        if (CR0_PE && gdt_index != 0 && gdt_entry_count > gdt_index)
            proted = true;
        break;
    }
    case 0xe8:
    {
        uint16_t data = ram->read(physaddr(eip++, cs, 0));
        data |= ram->read(physaddr(eip++, cs, 0));
        sp.ei -= 4;
        pusheip();
        eip += data;
        if (debug)
        printf("CALL 0x%02x\n", eip);
    }
    break;
    case 0xC3:
    {
        eip = popeip();
        if (debug)
        printf("RET 0x%x\n", eip);
    }   
    break;
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
        if (debug)
        printf("MOV AH, 0x%02x\n", data);
        break;
    case 0xE:
        si.i = data;
        break;
    case 0x0:
        ax.l = data;
        if (debug)
        printf("MOV AL, 0x%02x\n", data);
        break;
    }
}

void CPU::mov_r16_imm(uint8_t opcode)
{
    uint8_t reg = opcode - 0xB8;
    switch(reg)
    {
    case 0x0:
    {
        uint16_t data = ram->read(physaddr(eip++, cs, 0));
        data |= ram->read(physaddr(eip++, cs, 0)) << 8;
        ax.hl = data;
        if (debug)
            printf("MOV AX, 0x%02x\n", data);
    }
    break;
    case 0x4:
    {
        uint16_t data = ram->read(physaddr(eip++, cs, 0));
        data |= ram->read(physaddr(eip++, cs, 0)) << 8;
        sp.ei = data;
        if (debug)
        printf("MOV SP, 0x%04x\n", data);
    }
    break;
    case 0x06:
    {
        uint8_t reg = ram->read(physaddr(eip++, cs, 0));
        switch (reg)
        {
        case 0xd8:
            ds = ax.hl;
            if (debug)
                printf("MOV DS, AX\n");
            break;
        default:
            break;
        }
        break;
    }
    case 0x36:
    {
        uint16_t data = ram->read(physaddr(eip++, cs, 0));
        data |= ram->read(physaddr(eip++, cs, 0)) << 8;
        si.i = data;
        if (debug)
            printf("MOV SI, 0x%02x\n", data);
    }
        break;
    }
    
}

void CPU::mov_r32_imm(uint8_t opcode)
{
    uint8_t reg = opcode - 0xB0;
    //printf("MOV CODE 0x%x\n", opcode);
    uint32_t data;
    data = ram->read(physaddr(eip++, cs, 0));
    data |= ram->read(physaddr(eip++, cs, 0)) << 8;
    data |= ram->read(physaddr(eip++, cs, 0)) << 16;
    data |= ram->read(physaddr(eip++, cs, 0)) << 24;
    //printf("DATA: 0x%x\n", data);
    switch (reg)
    {
    case 0x0:
        ax.reg = data;
        if (debug)
        printf("MOV AX, 0x%02x", data);
        break;
    default:
        //printf("UNKNOWN REGCODE 0x%x\n", reg);
        break;
    }
}

// Interrupts
void CPU::int_imm8()
{
    uint8_t interrupt = ram->read(eip++);
    if (debug)
    printf("INT 0x%x\n", interrupt);
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
            if (ax.h == 0x00)
            {
                system("clear");
            }
        }
        eflags->IF = 1; // Restore interrupt state
    }
}

void CPU::Dump()
{
    // We add a newline in front to avoid conflicting with any int 0x10 characters
    printf("\n\nAL: 0x%02x AH: 0x%02x AX: 0x%04x EAX: 0x%04x\n", ax.l, ax.h, ax.hl, ax.reg);
    printf("BL: 0x%02x BH: 0x%02x BX: 0x%04x EBX: 0x%04x\n", bx.l, bx.h, bx.hl, bx.reg);
    printf("CL: 0x%02x CH: 0x%02x CX: 0x%04x ECX: 0x%04x\n", cx.l, cx.h, cx.hl, cx.reg);
    printf("DL: 0x%02x DH: 0x%02x DX: 0x%04x EDX: 0x%04x\n", dx.l, dx.h, dx.hl, dx.reg);
    printf("SI: 0x%02x ESI: 0x%02x\n", si.i, si.ei);
    printf("GDT: limit: 0x%x base 0x%04x\n", gdtr.limit, gdtr.base);
    printf("ESP: 0x%x\n", sp.ei);
    printf("EIP: 0x%x\n", eip);
    printf("CR0: 0x%04x (PE: %s)\n", cr0, cr0 & 1 ? "1" : "0");
    printf("CS: 0x%02x DS: 0x%02x ES: 0x%02x\n", cs, ds, es);
    printf("Debug: %d\n", debug);
    printflags();
}

void CPU::pusheip()
{
    uint32_t offset = sp.ei - 4;
    sp.ei = offset;
    uint32_t p_addr = physaddr(offset, ss, 1);
    for (int i = 0; i < 4; i++)
    {
        ram->write(p_addr + i, eip >> (i * 8));
    }
}

uint32_t CPU::popeip()
{
    uint32_t offset = sp.ei;
    uint32_t p_addr = physaddr(offset, ss, 0);
    uint32_t value;
    value = ram->read(p_addr) << (0 * 8);
    value |= ram->read(p_addr + 1) << (1 * 8);
    value |= ram->read(p_addr + 2) << (2 * 8);
    value |= ram->read(p_addr + 3) << (3 * 8);
    sp.ei = offset + 4;
    return value;
}