#ifndef _BRANCH_H
#define _BRANCH_H

#include <stdint.h>
#include "env.h"

#define BRANCH_PC_OFFSET    4

uint32_t sign_extend_24(uint32_t imm24);
int branch(struct CPUState *env, uint32_t inst);

#endif
