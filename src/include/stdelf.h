#ifndef STDELF_H
#define STDELF_H
#include "boot.h"
#include "stddef.h"
#define ELF32_ST_TYPE(i) ((i) & 0xf)
typedef	struct __elf_section_header__ {
	uint32_t name;
	uint32_t type;
	uint32_t flags;
	uint32_t addr;
	uint32_t offset;
	uint32_t size;
	uint32_t link;
	uint32_t info;
	uint32_t addralign;
	uint32_t entsize;
} elf_section_header_t;
typedef struct __elf_symbol__ {
	uint32_t name;
	uint32_t value;
	uint32_t size;
	uint8_t info;
	uint8_t other;
	uint16_t shndx;
} elf_symbol_t;
typedef struct __elf__ {
	elf_symbol_t *symtab;
	uint32_t symtabsz;
	const char *strtab;
	uint32_t strtabsz;
} elf_t;
elf_t elf_from_multiboot(Multiboot *mb);
const char *elf_lookup_symbol(uint32_t addr, elf_t *elf);
#endif // STDELF_H
