#include "memory.h"
#include "stddef.h"
#include "stdarg.h"
uint32_t CreatPageManager(PageManager * p, uint32_t physicalmemorysize, uint32_t kernelbeginaddress, uint32_t kernelendaddress, ...) {
	uint32_t mempagecount = computepage(physicalmemorysize);
	uint32_t membitmapzise = mempagecount / 8;
	uint32_t address_begin;
	if ((kernelendaddress & 0xFFF) == 0) { address_begin = kernelendaddress; }
	else { address_begin = (kernelendaddress + 0x1000) & 0xFFFFF000; }
	MemoryPool *ptr;
	ptr = &(p->physicalmemorypool);
	CreatMemoryPool(ptr, address_begin, 0, mempagecount, PHYSICSMEMORY);
	SetMemoryPool(ptr, kernelbeginaddress, computepage(kernelendaddress - kernelbeginaddress), 1);
	SetMemoryPool(ptr, address_begin, computepage(membitmapzise), 1);
	va_list args;
	va_start(args, kernelendaddress);
	uint32_t *tmp_begin, *tmp_end;
	for (tmp_begin = (uint32_t*)args, va_arg(args, uint32_t), tmp_end = (uint32_t*)args; *tmp_end != 0; va_arg(args, uint32_t), tmp_begin = (uint32_t*)args, va_arg(args, uint32_t), tmp_end = (uint32_t*)args) {
		uint32_t tmp;
		for (tmp = *tmp_begin & 0xFFFFF000; tmp <= *tmp_end; tmp = tmp + 0x1000) { SetMemoryPoolOne(ptr, tmp, 1); }
	}
	va_end(args);
	uint32_t pbl = QueryUnuseMemoryOne(ptr, 0);
	SetMemoryPoolOne(ptr, pbl, 1);
	p->pbladdress = pbl;
	int i;
	for (i = 0; i < 128; i++) { p->bitmap[i] = 0; }
	p->pblbitmap.address = (uint32_t)(&(p->bitmap));
	p->pblbitmap.size = 1024;
	va_start(args, kernelendaddress);
	for (tmp_begin = (uint32_t*)args, va_arg(args, uint32_t), tmp_end = (uint32_t*)args; *tmp_end != 0; va_arg(args, uint32_t), tmp_begin = (uint32_t*)args, va_arg(args, uint32_t), tmp_end = (uint32_t*)args) {
		uint32_t tmp;
		for (tmp = *tmp_begin & 0xFFFFF000; tmp <= *tmp_end; tmp = tmp + 0x1000) { SetPage(p, tmp, tmp); }
	}
	va_end(args);
	for (i = kernelbeginaddress; i <= kernelendaddress; i = i + 4096) { SetPage(p, i, i); }
	for (i = address_begin; i <= membitmapzise + address_begin; i = i + 4096) { SetPage(p, i, i); }
	SetPage(p, p->pbladdress, p->pbladdress);
	return QueryUnuseMemoryOne(ptr, 0);
}
void SetPage(PageManager * p, uint32_t virturlpageaddress, uint32_t physicalpageaddress) {
	int pblindex, ptlindex;
	pblindex = (virturlpageaddress & 0xFFFFF000) / (4 * 1024 * 1024);
	ptlindex = (virturlpageaddress & 0x003FFFFF) / (4 * 1024);
	uint32_t ptladdress;
	MemoryPool *ptr = &(p->physicalmemorypool);
	if (QueryBitmap(&(p->pblbitmap), pblindex) == 0) {
		SetBitmap(&(p->pblbitmap), pblindex, 1);
		uint32_t tmp = QueryUnuseMemoryOne(ptr, 0);
		SetMemoryPoolOne(ptr, tmp, 1);
		allotpagetable(p->pbladdress, pblindex, tmp, PAGE_IN_MEM | PAGE_WRITE_READ);
		SetPage(p, tmp, tmp);
	}
	uint32_t * pbladdresstmp = (uint32_t*)(p->pbladdress);
	ptladdress = pbladdresstmp[pblindex];
	allotpage(ptladdress, ptlindex, physicalpageaddress, PAGE_IN_MEM | PAGE_WRITE_READ);
}
void OpenPageMode(PageManager * p) { page_runing((uint32_t)p->pbladdress); }
void ClosePageMode() { page_end(); }
void FlushTBL(PageManager * p) {
	page_end();
	page_runing((uint32_t)p->pbladdress);
}
