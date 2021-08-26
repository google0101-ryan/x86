#include "bus.hpp"
#include <stddef.h>

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

constexpr size_t operand_size_8 = 7;
constexpr size_t operand_size_16 = 15;
constexpr size_t operand_size_32 = 31;
constexpr uint32_t eflags_cf = (1 << 0);
constexpr uint32_t eflags_pf = (1 << 2);
constexpr uint32_t eflags_af = (1 << 4);
constexpr uint32_t eflags_zf = (1 << 6);
constexpr uint32_t eflags_sf = (1 << 7);
constexpr uint32_t eflags_if = (1 << 9);
constexpr uint32_t eflags_df = (1 << 10);
constexpr uint32_t eflags_of = (1 << 11);
constexpr uint32_t eflags_all =
    (eflags_cf | eflags_pf | eflags_af | eflags_zf | eflags_sf | eflags_of);

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

typedef struct
{
    uint8_t seg, size_override;
} prefix_t;

struct ModRM {
    uint8_t rm : 3;
    uint8_t reg : 3;
    uint8_t mod : 2;
} __attribute__((packed));

class Pentium
{
public:
    prefix_t prefix;

    typedef void (instr_t)(Pentium*);

    instr_t *instrs[256];
    
    Register gpregs[num_gpregs];

    Segment sgregs[num_sgregs];

    Register ip;

    IDTDescriptor idtr;

    GDTDescriptor gdtr;

    bool isPE = false;
    bool firstClock = true;
    uint32_t eflags, eflags_dirty, last_op1, last_op2, last_result, last_add_result, last_size;
    uint8_t raw_modrm;
    ModRM* modrm;
    uint32_t decoded_addr;
    uint16_t read_modrm_rm16();
    uint32_t modrm_to_address(uint8_t mod, uint8_t rm);
    uint32_t seg_to_linear(SGRegister reg, uint32_t offset);
public:
    CPUState state;
    Bus* bus; // We expose the bus for instructions
    Pentium(Bus* bus) : bus(bus) {modrm = (ModRM*)&raw_modrm; reset();}
    uint32_t getLinearAddr();
    void reset();
    void clock();
    void write_sgreg(SGRegister seg, uint32_t value);
    void write_ip(uint16_t value);
    uint16_t get_ip();
};