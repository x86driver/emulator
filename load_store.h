#ifndef _LOAD_STORE_H
#define _LOAD_STORE_H

#include "env.h"

enum  {
    TYPE_LSL = 0,
    TYPE_LSR,
    TYPE_ASR,
    TYPE_ROR
};

int ldst_imm(struct CPUState *env, uint32_t inst);
int ldst_reg(struct CPUState *env, uint32_t inst);

uint32_t shift(struct CPUState *env, uint32_t val, uint32_t type, uint32_t imm5);

#endif

