#include "cpu.hpp"

void code_83(Pentium*);
void pop_r32(Pentium*);
void jnz32(Pentium* cpu);
void mov_seg_rm32(Pentium* cpu);
void mov_r32_imm32(Pentium* cpu);
void near_jump(Pentium* cpu);
void mov_rm32_r32(Pentium* cpu);
void out_imm8_al(Pentium* cpu);
void in_al_imm8(Pentium* cpu);
void or_al_imm8(Pentium* cpu);
void code_0f_01(Pentium* cpu);
void mov_r32_cr(Pentium* cpu);
void code_81(Pentium* cpu);
void ptr_jump(Pentium*);