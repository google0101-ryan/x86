#include "modrm.hpp"
#include "hw/cpu.hpp"

void pop_ds(Pentium* cpu)
{
    cpu->sgregs[(int)SGRegister::DS].base = cpu->pop32();
    cpu->ip.regs_32++;
}

void adc_r32_rm32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32;
    uint32_t rm32_val = get_rm32(cpu, &modrm) + (cpu->eflags & CARRY_FLAG);
    uint64_t result = (uint64_t)r32_val + (uint64_t)rm32_val;
    cpu->gpregs[modrm.reg_index].regs_32 = result;
    cpu->update_eflags_add(r32_val, rm32_val, result);
}

void push_ss(Pentium* cpu)
{
    cpu->push32(cpu->sgregs[(int)SGRegister::SS].base);
    cpu->ip.regs_32++;
}

void push_ds(Pentium* cpu)
{
    cpu->push32(cpu->sgregs[(int)SGRegister::DS].base);
    cpu->ip.regs_32++;
}

void sbb_rm32_r32(Pentium* cpu)
{
    cpu->ip.regs_32++;
    ModRM modrm = create_modrm();
    parse_modrm(&modrm, cpu);
    uint32_t rm32_val = get_rm32(cpu, &modrm);
    uint32_t r32_val = cpu->gpregs[modrm.reg_index].regs_32 + (cpu->eflags & CARRY_FLAG);
    uint64_t result = (uint64_t)rm32_val - (uint64_t)r32_val;
    set_rm32(cpu, &modrm, result);
    cpu->update_eflags_sub(rm32_val, r32_val, result);
}