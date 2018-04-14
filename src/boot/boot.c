#include "boot.h"
#include "stddef.h"
#include "stdio.h"
typedef struct __multiboot_header__ {
	uint64_t magic;
	uint64_t flags;
	uint64_t checksum;
} MultibootHeader;
MultibootHeader mbr __attribute__((section(".init.text"))) = {
	.magic = 0x1BADB002,
	.flags = 0x00000003,
	.checksum = -(0x1BADB002 + 0x00000003),
};
uint64_t magic;
Multiboot *multiboot_ptr;
void boot_entry(void) {
	asm volatile ("movl %%eax,%0" : "=m"(magic));
	if (magic != 0x2BADB002) {
		printk("Error: Cannot loading kernel\n");
		return;
	}
	asm volatile ("movl %%ebx,%0" : "=m"(multiboot_ptr));
	clear();
	asm volatile ("cli");
	printk("Booting...");
	kernel_entry();
	while (1) {}
}
