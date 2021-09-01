#include "hw/cpu.hpp"
#include "modrm.hpp"

void sub_r32_rm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32;
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint64_t result = (uint64_t)r32_val - (uint64_t)rm32_val;
    cpu->gpregs[modrm.reg_index].regs_32 = result;
    cpu->update_eflags_sub(r32_val, rm32_val, result);
}

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

void and_ax_imm16(Pentium* cpu)
{
    uint16_t imm16_val = cpu->bus->read16(cpu->getLinearAddr() + 1);
    uint16_t ax_val = cpu->gpregs[(int)GPRegister16::AX].regs_16;
    uint16_t result = ax_val & imm16_val;
    cpu->gpregs[(int)GPRegister16::AX].regs_16 = result;
    cpu->ip.regs_32 += 2;
}