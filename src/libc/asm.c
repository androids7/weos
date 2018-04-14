#include "asm.h"
#include "stddef.h"
uint8_t inb(uint32_t port) {
	uint8_t tmp;
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("inb %dx,%al");
	asm("movb %%al,%0":"=m"(tmp));
	return tmp;
}
uint16_t inw(uint32_t port) {
	uint16_t tmp;
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("inw %dx,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint32_t intdw(uint32_t port) {
	uint32_t tmp;
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("inl %dx,%eax");
	asm("movl %%eax,%0":"=m"(tmp));
	return tmp;
}
void outb(uint32_t port, uint8_t data) {
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("movb %0,%%al":"=m"(data));
	asm("outb %al,%dx");
}
void outw(uint32_t port, uint16_t data) {
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("movw %0,%%ax":"=m"(data));
	asm("outw %ax,%dx");
}
void outdw(uint32_t port, uint32_t data) {
	asm("movl %0,%%edx"::"m"(port));
	asm("movl $0,%eax");
	asm("movl %0,%%eax":"=m"(data));
	asm("outl %eax,%dx");
}
void cli() { asm("cli"); }
void sti() { asm("sti"); }
void hlt() { asm("hlt"); }
void sti_hlt() {
	asm("sti");
	asm("hlt");
}
uint32_t get_eflags() {
	uint32_t tmp;
	asm("pushfl");
	asm("popl %eax");
	asm("movl %%eax,%0":"=m"(tmp));
	return tmp;
}
void set_eflags(uint32_t eflages) {
	asm("movl %0,%%eax":"=m"(eflages));
	asm("pushl %eax");
	asm("popfl");
}
void set_ss(uint16_t data) {
	asm("movw %0,%%ax"::"m"(data));
	asm("movw %ax,%ss");
}
void set_ds(uint16_t data) {
	asm("movw %0,%%ax"::"m"(data));
	asm("movw %ax,%ds");
}
void set_es(uint16_t data) {
	asm("movw %0,%%ax"::"m"(data));
	asm("movw %ax,%es");
}
void set_fs(uint16_t data) {
	asm("movw %0,%%ax"::"m"(data));
	asm("movw %ax,%fs");
}
void set_gs(uint16_t data) {
	asm("movw %0,%%ax"::"m"(data));
	asm("movw %ax,%gs");
}
uint16_t get_cs() {
	uint16_t tmp;
	asm("movw %cs,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint16_t get_ds() {
	uint16_t tmp;
	asm("movw %ds,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint16_t get_ss() {
	uint16_t tmp;
	asm("movw %ss,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint16_t get_es() {
	uint16_t tmp;
	asm("movw %es,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint16_t get_fs() {
	uint16_t tmp;
	asm("movw %fs,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
uint16_t get_gs() {
	uint16_t tmp;
	asm("movw %gs,%ax");
	asm("movw %%ax,%0":"=m"(tmp));
	return tmp;
}
