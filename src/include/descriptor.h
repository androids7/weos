#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include "stddef.h"
#include "boot.h"
#define DA_32 0x4000   
#define	DA_16 0x0000   
#define DA_LIMIT_4K 0x8000
#define DA_LIMIT_byte 0x0000	
#define DA_DPL0	0x00
#define DA_DPL1	0x20
#define DA_DPL2	0x40
#define DA_DPL3	0x60
#define DA_DR 0x90	
#define DA_DRW 0x92	
#define DA_DRWA	0x93	
#define DA_C 0x98
#define DA_CR 0x9A
#define DA_CCO 0x9C
#define DA_CCOR	0x9E
#define DA_LDT 0x82
#define DA_TaskGate 0x85
#define DA_386TSS 0x89
#define DA_386CGate 0x8C
#define DA_386IGate 0x8E
#define DA_386TGate 0x8F
#define SA_RPL0	0
#define SA_RPL1	1
#define SA_RPL2	2
#define SA_RPL3	3
#define SA_TIG 0
#define SA_TIL 4
typedef struct Descriptor {
	uint16_t limit1;
	uint16_t base1;
	uint8_t base2;
	uint16_t attrandlimit2;
	uint8_t base3;
} Descriptor;
#endif // DESCRIPTOR_H
