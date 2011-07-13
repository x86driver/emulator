#ifndef _LOAD_STORE_H
#define _LOAD_STORE_H

#include "env.h"

int ldst_imm(struct CPUState *env, uint32_t inst);
int ldst_reg(struct CPUState *env, uint32_t inst);

#endif

