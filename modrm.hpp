#pragma once

#include "cpu.hpp"

typedef struct
{
    uint8_t scale;
    uint8_t index;
    uint8_t base;
} SIB;

typedef struct
{
    /* Mod: 2 bits */
    uint8_t mod;
    /* REG: 3 bits */
    union {                // union: different data at the same location
        uint8_t opcode;    // used to extend opcode (e.g. inc: ff + 000 and dec: ff + 001)
        uint8_t reg_index; // used to specify register (target or source)
    };
    uint8_t rm; // 3 bits
    SIB sib;    // 1 byte
    union {
        int8_t disp8;
        int16_t disp16;
        uint32_t disp32;
    };
} ModRM;

ModRM create_modrm();

void parse_modrm(ModRM* modrm, Pentium* cpu);

uint32_t calc_memory_address(Pentium* cpu, ModRM* modrm);

uint32_t get_rm32(Pentium* cpu, ModRM* modrm);
void set_rm32(Pentium *cpu, ModRM* modrm, uint32_t value);