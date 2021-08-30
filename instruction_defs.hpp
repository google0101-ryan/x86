#include "cpu.hpp"

/*0x00*/
void push_es(Pentium* cpu);
void pop_es(Pentium* cpu);
void les(Pentium* cpu);

/*0x10*/
void pop_ds(Pentium* cpu);

/*0x20*/
void and_rm32_r32(Pentium* cpu);

void cmp_rm32_r32(Pentium* cpu);

/*0x50*/
void push_r32(Pentium* cpu);
void pop_r32(Pentium* cpu);

void jge(Pentium* cpu);
void jz(Pentium* cpu);

void xor_rm32_r32(Pentium* cpu);
void cmp_al_imm8(Pentium* cpu);

/*0x80*/
void code_80(Pentium* cpu);
void code_83(Pentium* cpu);
void mov_rm32_r32(Pentium* cpu);
void mov_r8_rm8(Pentium* cpu);
void mov_r32_rm32(Pentium* cpu);
void mov_seg_rm32(Pentium* cpu);

/*0xB0*/
void mov_r8_imm8(Pentium* cpu);
void mov_rm8_r8(Pentium* cpu);

/*0xA0*/
void stosb(Pentium* cpu);
void movsb(Pentium* cpu);

/*0xC0*/
void code_c1(Pentium* cpu);
void mov_rm32_imm32(Pentium* cpu);
void ret(Pentium* cpu);

/*0xE0*/
void jecxz(Pentium* cpu);
void in_al_imm8(Pentium* cpu);
void out_imm8_al(Pentium* cpu);
void short_jump(Pentium* cpu);
void ptr_jump(Pentium* cpu);

/*0xF0*/
void cld(Pentium* cpu);

/*0x0F 0x80*/
void jnz32(Pentium* cpu);
void jc32(Pentium* cpu);
void ja32(Pentium* cpu);