#ifndef GATE_H
#define GATE_H
#include "stddef.h"
#include "descriptor.h"
typedef struct Gate {
	uint16_t offset1;
	uint16_t selector;
	uint16_t attr;
	uint16_t offset2;
} Gate;
#endif // GATE_H
