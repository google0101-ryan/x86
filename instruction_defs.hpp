#include "hw/cpu.hpp"

/*0x00*/
void add_rm32_r32(Pentium* cpu);
void add_al_imm8(Pentium* cpu);
void push_es(Pentium* cpu);
void pop_es(Pentium* cpu);
void or_rm16_r16(Pentium* cpu);
void or_rm32_r32(Pentium* cpu);
void les(Pentium* cpu);
void push_cs(Pentium* cpu);

/*0x10*/
void pop_ds(Pentium* cpu);
void adc_r32_rm32(Pentium* cpu);
void push_ss(Pentium* cpu);
void sbb_rm32_r32(Pentium* cpu);
void push_ds(Pentium* cpu);

/*0x20*/
void sub_r32_rm32(Pentium* cpu);
void and_ax_imm16(Pentium* cpu);
void and_rm32_r32(Pentium* cpu);

void cmp_rm32_r32(Pentium* cpu);
void xor_rm8_r8(Pentium* cpu);

/*0x50*/
void push_r32(Pentium* cpu);
void pop_r32(Pentium* cpu);
void push_r16(Pentium* cpu);
void pop_r16(Pentium* cpu);

void jno(Pentium* cpu);
void jge(Pentium* cpu);
void jz(Pentium* cpu);
void jg(Pentium* cpu);

void xor_rm32_r32(Pentium* cpu);
void xor_rm16_r16(Pentium* cpu);
void xor_eax_imm32(Pentium* cpu);
void cmp_al_imm8(Pentium* cpu);

/*0x80*/
void code_80(Pentium* cpu);
void code_83(Pentium* cpu);
void mov_rm32_r32(Pentium* cpu);
void mov_rm16_r16(Pentium* cpu);
void mov_r8_rm8(Pentium* cpu);
void mov_r32_rm32(Pentium* cpu);
void mov_seg_rm32(Pentium* cpu);
void mov_rm32_seg(Pentium* cpu);
void mov_rm16_seg(Pentium* cpu);

/*0x90*/
void xchg_r32_r32(Pentium* cpu);

/*0xB0*/
void mov_r8_imm8(Pentium* cpu);
void mov_rm8_r8(Pentium* cpu);
void mov_r32_imm32(Pentium* cpu);
void mov_r16_imm16(Pentium* cpu);
void mov_seg_rm16(Pentium* cpu);

/*0xA0*/
void mov_eax_moffs32(Pentium* cpu);
void mov_moffs8_al(Pentium* cpu);
void mov_moffs16_ax(Pentium* cpu);
void stosb(Pentium* cpu);
void stosd(Pentium* cpu);
void movsb(Pentium* cpu);
void movsd(Pentium* cpu);

/*0xC0*/
void code_c1(Pentium* cpu);
void mov_rm32_imm32(Pentium* cpu);
void ret(Pentium* cpu);
void code_c1_16(Pentium* cpu);
void mov_rm8_imm8(Pentium* cpu);

/*0xD0*/
void code_dd(Pentium* cpu);

/*0xE0*/
void loop(Pentium* cpu);
void jecxz(Pentium* cpu);
void in_eax_imm8(Pentium* cpu);
void in_al_imm8(Pentium* cpu);
void out_imm8_al(Pentium* cpu);
void short_jump(Pentium* cpu);
void near_jump(Pentium* cpu);
void ptr_jump(Pentium* cpu);
void call_rel32(Pentium* cpu);
void call_rel16(Pentium* cpu);

/*0xF0*/
void cld(Pentium* cpu);
void cli(Pentium* cpu);
void code_f6(Pentium* cpu);
void code_fe(Pentium* cpu);
void code_ff(Pentium* cpu);

/*0x0F 0x80*/
void jnz32(Pentium* cpu);
void jc32(Pentium* cpu);
void ja32(Pentium* cpu);