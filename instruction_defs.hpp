#include "cpu.hpp"

/*0x20*/
void and_rm32_r32(Pentium* cpu);

void cmp_rm32_r32(Pentium* cpu);

/*0x50*/
void push_r32(Pentium* cpu);

void jge(Pentium* cpu);

void xor_rm32_r32(Pentium* cpu);

/*0x80*/
void code_80(Pentium* cpu);
void code_83(Pentium* cpu);
void mov_rm32_r32(Pentium* cpu);
void mov_r8_rm8(Pentium* cpu);
void mov_r32_rm32(Pentium* cpu);
void mov_seg_rm32(Pentium* cpu);

/*0xC0*/
void code_c1(Pentium* cpu);
void mov_rm32_imm32(Pentium* cpu);

/*0xE0*/
void out_imm8_al(Pentium* cpu);
void short_jump(Pentium* cpu);
void ptr_jump(Pentium* cpu);

/*0x0F 0x80*/
void jnz32(Pentium* cpu);
void jc32(Pentium* cpu);
void ja32(Pentium* cpu);