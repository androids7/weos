#include "string.h"
#include "memory.h"
#include "stddef.h"
void CreatBitmap(BitMap *bit, uint32_t mapaddress, uint32_t size) {
	bit->address = (uint32_t)mapaddress;
	bit->size = size;
	memset((void*)mapaddress, 0, size / 8);
}
void SetBitmap(BitMap *bit, uint32_t index, uint8_t value) {
	uint32_t index_x, index_y;
	index_x = index / 8;
	index_y = index % 8;
	uint8_t * tmp;
	uint8_t tmp_value;
	uint8_t tmptest;
	tmp = (uint8_t*)bit->address;
	tmp_value = tmp[index_x];
	if (value == 0) {
		tmptest = 1 << index_y;
		tmptest = tmptest ^ 0xFF;
		tmp_value = tmp_value & tmptest;
	}
	else {
		tmptest = 1 << index_y;
		tmp_value = tmp_value | tmptest;
	}
	tmp[index_x] = tmp_value;
}
uint8_t QueryBitmap(BitMap *bit, uint32_t index) {
	uint32_t index_x, index_y;
	index_x = index / 8;
	index_y = index % 8;
	uint8_t value;
	uint8_t *tmp;
	tmp = (uint8_t *)bit->address;
	value = tmp[index_x];
	value = (value & (1 << index_y)) >> index_y;
	return value;
}
