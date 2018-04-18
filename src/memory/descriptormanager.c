#include "asm.h"
#include "stddef.h"
#include "memory.h"
GDT *gdt;
void modify_descriptor(Descriptor * desc, uint32_t limit, uint32_t base, uint32_t attr) {
	desc->limit1 = limit & 0xFFFF;
	desc->base1 = base & 0xFFFF;
	desc->base2 = base >> 16 & 0xFF;
	desc->attrandlimit2 = (limit >> 8 & 0x0F00) | (attr & 0xF0FF);
	desc->base3 = base >> 24 & 0xFF;
}
void set_gdt(int index, uint32_t size, uint32_t base, uint32_t attr) {
	Descriptor * tmp;
	tmp = (Descriptor*)gdt;
	tmp = tmp + index;
	modify_descriptor(tmp, size, base, attr);
}
uint32_t get_gdt_base(int index) {
	uint64_t gdt_operand = 0;
	asm("sgdt %0":"=m"(gdt_operand));
	uint32_t tmppp = (uint32_t)(gdt_operand >> 16);
	Descriptor * tmp = (Descriptor *)tmppp;
	tmp = tmp + index;
	return 	(tmp->base3 << 24) | (tmp->base2 << 16) | tmp->base1;
}
uint32_t get_gdt_limit(int index) {
	uint64_t gdt_operand = 0;
	asm("sgdt %0":"=m"(gdt_operand));
	uint32_t tmppp = (uint32_t)(gdt_operand >> 16);
	Descriptor * tmp = (Descriptor *)tmppp;
	tmp = tmp + index;
	return 	((tmp->attrandlimit2 & 0x0F00) << 8) | tmp->limit1;
}
uint32_t get_gdt_attr(int index) {
	uint64_t gdt_operand = 0;
	asm("sgdt %0":"=m"(gdt_operand));
	uint32_t tmppp = (uint32_t)(gdt_operand >> 16);
	Descriptor * tmp = (Descriptor *)tmppp;
	tmp = tmp + index;
	return tmp->attrandlimit2 & 0xF0FF;
}
void gdt_init(uint32_t address) {
	gdt = (GDT *)address;
	int i = 0;
	for (; i < GDT_COUNT; i++) { set_gdt(i, 0x0, 0x0, 0); }
	int d_tmp;
	d_tmp = get_cs() >> 3;
	set_gdt(d_tmp, get_gdt_limit(d_tmp), get_gdt_base(d_tmp), get_gdt_attr(d_tmp));
	d_tmp = get_ds() >> 3;
	set_gdt(d_tmp, get_gdt_limit(d_tmp), get_gdt_base(d_tmp), get_gdt_attr(d_tmp));
	uint32_t tmp = (uint32_t)gdt;
	uint64_t gdt_operand = ((sizeof(GDT) - 1) | ((uint64_t)tmp << 16));
	asm("lgdt %0"::"m"(gdt_operand));
}
