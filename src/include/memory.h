#ifndef MEMORY_H
#define MEMORY_H
#include "boot.h"
#include "stddef.h"
#include "descriptor.h"
#define GDT_COUNT 8192
#define PHYSICSMEMORY 1
#define VIRTUALMEMORY 2
#define USING 1
#define UNUSING 0
#define PAGE_IN_MEM 0x1
#define PAGE_WRITE_READ 0x2
#define PAGE_IS_USER 0x4
#define PAGE_IS_THROUGH 0x8
#define PAGE_IS_NOBUFFER 0x10
#define PAGE_IS_ACCESS 0x20
#define PAGE_IS_WRITE 0x40
#define PAGE_IS_4K 0x0
#define PAGE_IS_GLOBAL 0x100
#define PAGE_NOT_IN_MEM 0x0
#define PAGE_IS_READ 0x0
#define PAGE_IS_SYSTEM 0x0
#define PAGE_IS_BACK 0x0
#define PAGE_IS_BUFFER 0x0
#define PAGE_IS_NOT_ACCESS 0x0
#define PAGE_IS_NOT_WRITE 0x0
#define PAGE_IS_NOT_4K 0x80
#define PAGE_IS_NOT_GLOBAL 0x0
typedef struct __bitmap__ {
	uint32_t address;
	uint32_t size;
} BitMap;
typedef struct __descriptor_manager__ {
	Descriptor *desc;
	int count;
} DescriptorManager;
typedef struct __gdt__ {
	Descriptor desc[GDT_COUNT];
} GDT;
typedef struct __memory_pool__ {
	BitMap bitmap;
	uint32_t memorybeginaddress;
	uint32_t type;
} MemoryPool;
typedef struct __page_manager__ {
	MemoryPool physicalmemorypool;
	uint32_t pbladdress;
	char bitmap[128];
	BitMap pblbitmap;
} PageManager;
extern PageManager pg;
void CreatBitmap(BitMap *bit, uint32_t mapaddress, uint32_t size);
void SetBitmap(BitMap *bit, uint32_t index, uint8_t value);
uint8_t QueryBitmap(BitMap *bit, uint32_t index);
void modify_descriptor(Descriptor * desc, uint32_t limit, uint32_t base, uint32_t attr);
uint32_t get_gdt_base(int index);
uint32_t get_gdt_limit(int index);
uint32_t get_gdt_attr(int index);
void set_gdt(int index, uint32_t size, uint32_t base, uint32_t attr);
void gdt_init(uint32_t address);
void CreatMemoryPool(MemoryPool *pool, uint32_t  bitmapaddress, uint32_t beginaddress, uint32_t size, uint32_t type);
uint32_t QueryUnuseMemoryOne(MemoryPool *pool, uint32_t address);
uint32_t QueryUnuseMemory(MemoryPool *pool, uint32_t address, uint32_t count);
void SetMemoryPoolOne(MemoryPool *pool, uint32_t  index, uint8_t value);
void SetMemoryPool(MemoryPool *pool, uint32_t  index, uint32_t count, uint8_t value);
void allotpagetable(const unsigned int page_dir_base, int index, unsigned int page_tpl_base, int attr);
void allotpage(const unsigned int page_tpl_base, int index, unsigned int page_address, int attr);
void page_runing(const unsigned int page_dir_base);
void page_end();
uint32_t computepage(uint32_t memorysize);
uint32_t CreatPageManager(PageManager * p, uint32_t kernelbeginaddress, uint32_t kernelsize, uint32_t physicalmemorysize, ...);
void SetPage(PageManager * p, uint32_t  virturlpageaddress, uint32_t physicalpageaddress);
void OpenPageMode(PageManager * p);
void ClosePageMode();
void FlushTBL(PageManager * p);
int get_memory_size();
void memory_manager_init(int physicalmemorysize, uint32_t kernelstart, uint32_t kernelend);
void *malloc_page(int size);
void free_page(void *ptr, int size);
void SetPageOnPaging(uint32_t virturlpageaddress, uint32_t physicalpageaddress);
void ReleasePageOne(uint32_t virturlpageaddress);
void ReleasePage(uint32_t virturlpageaddress, int32_t count);
uint32_t QueryVirturAddress(uint32_t physicalpageaddress);
uint32_t QueryPhysicalAddress(uint32_t virturlpageaddress);
#endif // MEMORY_H
