#include "stdio.h"
#include "memory.h"
GDT gdt_table = { 0 };
MemoryPool virtualmemorypool = { 0 };
PageManager pg = { 0 };
uint32_t physicalmemoryaddress = 0;
uint32_t virtualmemoryaddress = 0;
int get_memory_size() { return (int)(global_multiboot_ptr->mem_upper * 1024); }
void memory_manager_init(int physicalmemorysize, uint32_t kernelstart, uint32_t kernelend) {
	gdt_init((uint32_t)&gdt_table);
	uint32_t tmp = CreatPageManager(&pg, physicalmemorysize, kernelstart, kernelend, 0, 0xFFFFF, 0, 0);
	CreatMemoryPool(&virtualmemorypool, tmp, 0, 1024 * 1024, VIRTUALMEMORY);
	SetMemoryPool(&virtualmemorypool, kernelstart, computepage(tmp - kernelstart), 1);
	SetMemoryPool(&virtualmemorypool, 0, computepage(0x100000), 1);
	SetMemoryPool(&virtualmemorypool, tmp, computepage(1024 * 1024 / 8), 1);
	SetMemoryPool(&pg.physicalmemorypool, tmp, computepage(1024 * 1024 / 8), 1);
	uint32_t tmpaddress;
	for (tmpaddress = tmp; tmpaddress < tmp + computepage(1024 * 1024 / 8) * 0x1000; tmpaddress = tmpaddress + 0x1000) { SetPage(&pg, tmpaddress, tmpaddress); }
	OpenPageMode(&pg);
}
void *malloc_page(int size) {
	uint32_t tmp = QueryUnuseMemory(&virtualmemorypool, 0, size);
	int i;
	for (i = 0; i < size; i++) {
		uint32_t pytmp = QueryUnuseMemoryOne(&(pg.physicalmemorypool), 0);
		SetPageOnPaging(tmp + i * 4 * 1024, pytmp);
	}
	return (void *)tmp;
}
void free_page(void *ptr, int size) {
	if (((uint32_t)ptr & 0xFFFFF000) == 0) {}
	else {
		uint32_t address = (uint32_t)ptr;
		ReleasePage(address, size);
	}
}
void SetPageOnPaging(uint32_t virturlpageaddress, uint32_t physicalpageaddress) {
	int pblindex, ptlindex;
	pblindex = (virturlpageaddress & 0xFFFFF000) / (4 * 1024 * 1024);
	ptlindex = (virturlpageaddress & 0x003FFFFF) / (4 * 1024);
	uint32_t pbl_address;
	MemoryPool *ptr = &(pg.physicalmemorypool);
	pbl_address = pg.pbladdress;
	if (QueryBitmap(&(pg.pblbitmap), pblindex) == 0) {
		SetBitmap(&(pg.pblbitmap), pblindex, 1);
		uint32_t tmp = QueryUnuseMemoryOne(ptr, 0);
		SetMemoryPoolOne(ptr, tmp, 1);
		uint32_t tmpvitrl = QueryUnuseMemoryOne(&virtualmemorypool, 0);
		SetMemoryPoolOne(&virtualmemorypool, tmpvitrl, 1);
		SetPageOnPaging(tmpvitrl, tmp);
		allotpagetable(pg.pbladdress, pblindex, tmp, PAGE_IN_MEM | PAGE_WRITE_READ);
		allotpage(tmpvitrl, ptlindex, physicalpageaddress, PAGE_IN_MEM | PAGE_WRITE_READ);
		SetMemoryPoolOne(ptr, physicalpageaddress, 1);
		SetMemoryPoolOne(&virtualmemorypool, virturlpageaddress, 1);
	}
	else {
		uint32_t * tmppbladdress = (uint32_t*)pbl_address;
		uint32_t virtual_address = QueryVirturAddress(tmppbladdress[pblindex] & 0xFFFFF000);
		allotpage(virtual_address, ptlindex, physicalpageaddress, PAGE_IN_MEM | PAGE_WRITE_READ);
		SetMemoryPoolOne(ptr, physicalpageaddress, 1);
		SetMemoryPoolOne(&virtualmemorypool, virturlpageaddress, 1);
	}
}
void ReleasePageOne(uint32_t virturlpageaddress) {
	int pblindex, ptlindex;
	pblindex = (virturlpageaddress & 0xFFFFF000) / (4 * 1024 * 1024);
	ptlindex = (virturlpageaddress & 0x003FFFFF) / (4 * 1024);
	uint32_t *address_pbl = (uint32_t*)pg.pbladdress;
	uint32_t vitrual_address = QueryVirturAddress(address_pbl[pblindex] & 0xFFFFF000);
	uint32_t tmpaddress = ((uint32_t*)vitrual_address)[ptlindex] & 0xFFFFF000;
	allotpage(vitrual_address, ptlindex, 0, 0);
	MemoryPool * physicalpool = &(pg.physicalmemorypool);
	SetMemoryPoolOne(physicalpool, tmpaddress, 0);
	SetMemoryPoolOne(&virtualmemorypool, virturlpageaddress, 0);
	int i;
	for (i = 0; i < 1024; i++) {
		int test = 0;
		uint32_t * ptl_address_tmp = (uint32_t*)(address_pbl[i] & 0xFFFFF000);
		int j;
		for (j = 0; j < 1024; j++) {
			if ((ptl_address_tmp[j] & 0xFFFFF000) != 0) {
				test = 1;
				break;
			}
		}
		if ((test == 0) && !QueryBitmap(&pg.pblbitmap, i)) {
			uint32_t *tmp = (uint32_t*)(pg.pbladdress);
			if ((tmp[i] & 0xFFFFF000) != 0) {
				ReleasePageOne(tmp[i] & 0xFFFFF000);
				SetBitmap(&pg.pblbitmap, i, 0);
				allotpagetable(pg.pbladdress, i, 0, 0);
			}
		}
	}
}
void ReleasePage(uint32_t virturlpageaddress, int32_t count) {
	uint32_t tmp = virturlpageaddress;
	int i;
	for (i = 0; i < count; i++) { ReleasePageOne(tmp + i * 4096); }
}
uint32_t QueryVirturAddress(uint32_t physicalpageaddress) {
	if (physicalpageaddress == 0) {
		printk("your physical page address=%d is error!", physicalpageaddress);
		return 0;
	}
	else {
		int pblindex = 0, ptlindex = 0;
		uint32_t pbl_address, ptl_address;
		BitMap *ptr = &(pg.pblbitmap);
		pbl_address = pg.pbladdress;
		uint32_t * tmppbl = (uint32_t*)(pbl_address & 0xFFFFF000);
		int i, j;
		for (i = 0; i < 1024; i++) {
			if (QueryBitmap(ptr, i) == 0) { continue; }
			ptl_address = tmppbl[i] & 0xFFFFF000;
			uint32_t * tmpptl = (uint32_t*)ptl_address;
			for (j = 0; j < 1024; j++) {
				if ((tmpptl[j] & 0xFFFFF000) == physicalpageaddress) {
					pblindex = i;
					ptlindex = j;
					break;
				}
			}
		}
		return pblindex * 1048576 * 4 + ptlindex * 1024 * 4;
	}
}
uint32_t QueryPhysicalAddress(uint32_t virturlpageaddress) {
	int pblindex, ptlindex;
	pblindex = (virturlpageaddress & 0xFFFFF000) / (4 * 1024 * 1024);
	ptlindex = (virturlpageaddress & 0x003FFFFF) / (4 * 1024);
	printk("pblindex: %d, ptlindex: %d  ", pblindex, ptlindex);
	uint32_t pbl_address, ptl_address;
	pbl_address = pg.pbladdress;
	printk("pbl address :  0x%08X\n", pbl_address);
	if (QueryBitmap(&(pg.pblbitmap), pblindex) == 0) {
		printk("the virturl page address is not allocation!");
		return 0;
	}
	else {
		uint32_t * tmp = (uint32_t*)pbl_address;
		ptl_address = tmp[pblindex] & 0xFFFFF000;
		printk("the pagetable address is: 0x%08X\n", ptl_address);
		tmp = (uint32_t*)ptl_address;
		return tmp[ptlindex] & 0xFFFFF000;
	}
}
