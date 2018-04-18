#include "stddef.h"
#include "memory.h"
void  allotpagetable(const unsigned int  page_dir_base, int index, unsigned int page_tpl_base, int attr) {
	unsigned int *p = (unsigned int *)(page_dir_base & 0xFFFFF000);
	p[index] = (unsigned int)((page_tpl_base & 0xFFFFF000) + attr);
}
void allotpage(const unsigned int page_tpl_base, int index, unsigned int page_address, int attr) {
	unsigned int *p = (unsigned int*)(page_tpl_base & 0xFFFFF000);
	p[index] = (unsigned int)((page_address & 0xFFFFF000) + attr);
}
void page_runing(const unsigned int  page_dir_base) {
	unsigned int page_dir;
	page_dir = (unsigned int)((unsigned int)page_dir_base & 0xFFFFF000);
	asm("pusha");
	asm("movl %0,%%eax"::"d"(page_dir));
	asm("movl %eax,%cr3");
	asm("movl %cr0,%eax");
	asm("orl $0x80000000,%eax");
	asm("movl %eax,%cr0");
	asm("nop");
	asm("popa");
}
void page_end() {
	asm("pusha");
	asm("movl %cr0,%eax");
	asm("andl $0x7FFFFFFF,%eax");
	asm("movl %eax,%cr0");
	asm("nop");
	asm("popa");
}
uint32_t computepage(uint32_t memorysize) {
	if (memorysize % (4 * 1024) == 0) { return memorysize / (4 * 1024); }
	else { return memorysize / (4 * 1024) + 1; }
}
