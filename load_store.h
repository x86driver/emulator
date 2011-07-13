#ifndef _LOAD_STORE_H
#define _LOAD_STORE_H

#include "env.h"

enum SRType {
    SRType_LSL, SRType_LSR, SRType_ASR, SRType_ROR, SRType_RRX
};

int ldst_imm(struct CPUState *env, uint32_t inst);
int ldst_reg(struct CPUState *env, uint32_t inst);

#endif

