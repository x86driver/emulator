#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "env.h"

void dump_mem(uint32_t *memory, unsigned int start, unsigned int size);
void dump_reg(struct CPUState *env);

#endif

