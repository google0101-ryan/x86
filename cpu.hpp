#pragma once

#include "bus.hpp"
#include "IOBus.hpp"
#include <stddef.h>

class Pentium;

union Register {
    uint32_t regs_32;
    uint16_t regs_16;
    struct {
        uint8_t regs_8l;
        uint8_t regs_8h;
    };
};

struct Segment {
    uint32_t base, limit;
    uint16_t selector;
};

constexpr size_t num_gpregs = 8;
constexpr size_t num_sgregs = 6;

#define CARRY_FLAG (1)
#define ZERO_FLAG (1 << 6)
#define SIGN_FLAG (1 << 7)
#define INT_ENABLE_FLAG (1 << 9)
#define DIRECTION_FLAG (1 << 10)
#define OVERFLOW_FLAG (1 << 11)

enum class GPRegister8 : uint8_t {
    AL = 0,
    CL,
    DL,
    BL,
    AH,
    CH,
    DH,
    BH,
};

enum class GPRegister16 : uint8_t {
    AX = 0,
    CX,
    DX,
    BX,
    SP,
    BP,
    SI,
    DI,
};

enum class GPRegister32 : uint8_t {
    EAX = 0,
    ECX,
    EDX,
    EBX,
    ESP,
    EBP,
    ESI,
    EDI,
};

enum class SGRegister : int {
    CS = 0,
    DS,
    SS,
    ES,
    FS,
    GS,
};

enum class CPUState {
    STOPPED,
    HALTED,
    RUNNING,
};

struct IDTDescriptor {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct IDTEntry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t attributes;
    uint16_t offset_high;
} __attribute__((packed));

struct GDTDescriptor {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high : 4;
    uint8_t flags : 4;
    uint8_t base_high;
} __attribute__((packed));

class Pentium
{
public:

    typedef void (instr_t)(Pentium*);

    instr_t *instrs[256];
    instr_t *two_byte_instrs[256];
    
    Register gpregs[num_gpregs];

    Segment sgregs[num_sgregs];

    uint32_t cr[8];

    Register ip;

    IDTDescriptor idtr;

    GDTDescriptor gdtr;

    bool isPE = false;
    bool firstClock = true;
    uint32_t eflags;
    uint32_t decoded_addr;
    uint16_t read_modrm_rm16();
    uint32_t modrm_to_address(uint8_t mod, uint8_t rm);
    uint32_t seg_to_linear(SGRegister reg, uint32_t offset);
    uint32_t pop32();
    void push32(uint32_t value);
public:
    CPUState state;
    Bus* bus; // We expose the bus for instructions
    IOBus* iobus;
    Pentium(Bus* bus, IOBus* iobus) : bus(bus), iobus(iobus) {reset();}
    uint32_t getLinearAddr();
    void update_eflags_sub(uint32_t op1, uint32_t op2, uint64_t result);
    void reset();
    void clock();
    void write_sgreg(SGRegister seg, uint32_t value);
    void write_ip(uint16_t value);
    uint16_t get_ip();
};