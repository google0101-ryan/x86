#include "cpu.hpp"
#include "modrm.hpp"

void and_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint32_t r32 = cpu->gpregs[modrm.reg_index].regs_32;
    uint32_t result = rm32_val & r32;
    set_rm32(cpu, &modrm, result);
}