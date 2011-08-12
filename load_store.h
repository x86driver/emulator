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
int multi_ldst(struct CPUState *env, uint32_t inst);

uint32_t decode_imm_shift(uint32_t type, uint32_t imm5);
uint32_t shift(struct CPUState *env, uint32_t val, uint32_t type, uint32_t imm5, int update);
uint32_t shift_C(struct CPUState *env, uint32_t val, uint32_t type, uint32_t imm5, uint32_t *carry);
const char *shift_type(uint32_t type, uint32_t imm5);

#endif

