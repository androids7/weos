#include "stddef.h"
#include "memory.h"
#include "stdio.h"
#include "stdarg.h"
void CreatMemoryPool(MemoryPool *pool, uint32_t  bitmapaddress, uint32_t  beginaddress, uint32_t size, uint32_t type) {
	CreatBitmap(&(pool->bitmap), bitmapaddress, size);
	pool->memorybeginaddress = beginaddress;
	pool->type = type;
}
uint32_t QueryUnuseMemoryOne(MemoryPool *pool, uint32_t address) {
	uint32_t addresstmp = 0;
	uint32_t size = pool->bitmap.size;
	if ((uint32_t)address >= (uint32_t)pool->memorybeginaddress && (uint32_t)address <= ((uint32_t)pool->memorybeginaddress + pool->bitmap.size * 4 * 1024)) {
		int tmp;
		tmp = -1;
		uint32_t tmp_index = ((uint32_t)address - (uint32_t)pool->memorybeginaddress) / (4 * 1024);
		int i;
		for (i = tmp_index; i < size;) {
			uint8_t value = QueryBitmap(&pool->bitmap, i);
			if (value == 0) {
				tmp = i;
				break;
			}
			i++;
		}
		if (tmp == -1) {
			printk("don't find unusememory!");
			return (uint32_t)addresstmp;
		}
		else { addresstmp = (uint32_t)((int)pool->memorybeginaddress + tmp * 4 * 1024); }
	}
	else { printk("Your Address is NOT in this pool,cheak code!"); }
	return (uint32_t)addresstmp;
}
uint32_t QueryUnuseMemory(MemoryPool *pool, uint32_t address, uint32_t count) {
	uint32_t addresstmp = 0;
	uint32_t size = pool->bitmap.size;
	// uint32_t addresstmp1;
	uint32_t i, j, indextmp1;
	uint8_t valuetmp;
	if ((uint32_t)address >= (uint32_t)pool->memorybeginaddress && (uint32_t)address <= ((uint32_t)pool->memorybeginaddress + pool->bitmap.size * 4 * 1024)) {
		indextmp1 = ((uint32_t)address - (uint32_t)pool->memorybeginaddress) / (4 * 1024);
		for (i = indextmp1; i < size; i++) {
			if ((i + count) > size) {
				printk("QueryUnuse bad!");
				return (uint32_t)addresstmp;
			}
			valuetmp = 0;
			for (j = 0; j < count; j++) { valuetmp = valuetmp | QueryBitmap(&pool->bitmap, i + j); }
			if (valuetmp == 0) {
				addresstmp = (uint32_t)((uint32_t)pool->memorybeginaddress + i * 4 * 1024);
				break;
			}
		}
	}
	else {
		printk("QueryAddress:0x%08X\n", address);
		printk("Your Address is NOT in this pool,cheak code!\n");
		while (1) {}
	}
	return (uint32_t)addresstmp;
}
void SetMemoryPoolOne(MemoryPool *pool, uint32_t index, uint8_t value) {
	uint32_t poolbeginaddress = (uint32_t)pool->memorybeginaddress;
	uint32_t poolendaddress = (uint32_t)(((int)pool->memorybeginaddress + pool->bitmap.size * 4 * 1024));
	if (poolendaddress == 0) { poolendaddress = 0xFFFFFFFF; }
	if ((uint32_t)index >= poolbeginaddress && (uint32_t)index <= poolendaddress) {
		uint32_t tmp = ((uint32_t)index - (uint32_t)pool->memorybeginaddress) / (4 * 1024);
		SetBitmap(&pool->bitmap, tmp, value);
	}
	else {
		printk("pool is in 0x%08X-0x%08X\n", poolbeginaddress, poolendaddress);
		printk("SetAddress:0x%08X\n", index);
		printk("Your Address is NOT in this pool,cheak code!\n");
		while (1) {}
	}
}
void SetMemoryPool(MemoryPool *pool, uint32_t  index, uint32_t count, uint8_t value) {
	int i;
	for (i = 0; i < count; i++) { SetMemoryPoolOne(pool, (uint32_t)((int)index + i * 4 * 1024), value); }
}
