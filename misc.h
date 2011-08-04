#ifndef _MISC_H
#define _MISC_H

#include "env.h"

int misc_reg_inst(struct CPUState *env, uint32_t inst);
int mrs_reg(struct CPUState *env, uint32_t inst);

#endif

