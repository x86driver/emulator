#ifndef _LOAD_STORE_H
#define _LOAD_STORE_H

#include "env.h"

int op_ldr(struct CPUState *env, uint32_t inst);
int op_str(struct CPUState *env, uint32_t inst);

#endif

